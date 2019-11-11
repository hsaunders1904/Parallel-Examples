#include <iostream>
#include <map>
#include <mpi/mpi.h>
#include <string>
#include <vector>
/*
 * https://mpitutorial.com/tutorials/point-to-point-communication-application-random-walk/
 * In this example we implement a random walk using MPI.
 *
 * Given a min, max and random walker W, make W take S random walks of
 * arbitrary length to the right. If the process goes out of bounds, it wraps
 * back around. W can only move one unit to the right or left at a time.
 *
 * Our first task is to split the domain across processes. The random walk has
 * a one-dimensional domain of size `max - min + 1`. Assuming that walkers can
 * only take integer-sized steps, ew can partition our domain into near
 * equal-sized chunks across processes. For example if `min=0` and `max=20`,
 * with 4 processes, the domain can be split as follows:
 *
 *    min 0 4   5 9   10 14  15 20 max  (domain)
 *       |   | |   |  |   |  |   |
 *         0     1      2      3        (process)
 *
 * Once the domain has been partitioned, the application can initialize the
 * walkers. A walker will take S walks with a random total walk size. For
 * example, if the walker takes a walk of size six on process 0, the execution
 * of the walker will go like this:
 *
 *    1) The walker starts taking incremental steps. When it hits value four it
 *       has reached the end of the bounds of process zero. Process zero now
 *       has to communicate the walker to process one.
 *
 *    2) Process one receives the walker and continues walking until it has
 *       reached its total walk size of six. The walker can then proceed on a
 *       new random walk.
 */

typedef struct {
  int location;
  int numStepsLeftInWalk;
} Walker;

/*
 * Split the domain into even chunks, giving any remainder to the last process.
 * We assume the domain size is not smaller than the world size.
 */
std::map<std::string, int> decomposeDomain(const int &domainSize,
                                           const int &worldRank,
                                           const int &worldSize) {
  if (worldSize > domainSize) {
    // Assume the domain size is greater than the world size
    MPI_Abort(MPI_COMM_WORLD, 1);
  }
  std::map<std::string, int> subdomain;
  subdomain.insert({"start", domainSize / worldSize * worldRank});
  subdomain.insert({"size", domainSize / worldSize});
  // int subdomainStart = domainSize / worldSize*worldRank;
  // int subdomainSize = domainSize/worldSize;
  if (worldRank == worldSize - 1) {
    // Give remainder to last process
    // subdomainSize += domainSize % worldSize;
    subdomain["size"] += domainSize % worldSize;
  }
  return subdomain;
}

/*
 * Initialize the walkers. We take the subdomain bounds and add walkers to an
 * incomingWalkers vector.
 */
void initializeWalkers(const int &numWalkersPerProcess, const int &maxWalkSize,
                       std::map<std::string, int> &subdomain,
                       std::vector<Walker> *incomingWalkers) {
  Walker walker;
  for (int i = 0; i < numWalkersPerProcess; ++i) {
    // Initialize walkers in the middle of the subdomain
    walker.location = subdomain["start"];
    walker.numStepsLeftInWalk =
        static_cast<int>(rand() / (float)RAND_MAX * maxWalkSize);
        incomingWalkers->push_back(walker);
  }
}

/*
 * After intialisation we can progress the walkers. The `walk` function is
 * responsible for progressing the walker unit it has finished its walk. If it
 * goes out of local bounds, it's added to the outgoingWalkers vector.l
 */
void walk(Walker *walker, std::map<std::string, int> &subdomain,
          const int &domainSize, std::vector<Walker> *outgoingWalkers) {
  while (walker->numStepsLeftInWalk > 0) {
    if (walker->location == subdomain["start"] + subdomain["size"]) {
      // Take care of the case when the walker is at the end of the domain by
      // wrapping it around to the beginning
      if (walker->location == domainSize)
        walker->location = 0;
      outgoingWalkers->push_back(*walker);
      break;
    } else {
      walker->numStepsLeftInWalk--;
      walker->location++;
    }
  }
}

/*
 * This function sends a list of outgoing walkers.
 */
void sendOutgoingWalkers(std::vector<Walker> *outgoingWalkers,
                         const int &worldRank, const int &worldSize) {
  // Send the data as an array of MPI_BYTEs to the next process.
  // The last process send to process zero.
  MPI_Send((void*)outgoingWalkers->data(),
           outgoingWalkers->size()*sizeof(Walker),
           MPI_BYTE,
           (worldRank + 1) % worldSize,
           0,
           MPI_COMM_WORLD);

  // Clear the outgoing walkers
  outgoingWalkers->clear();
}

/*
 * Receive incoming walkers. We use MPI_Probe since we do not know beforehand
 * how many walkers we're receiving.
 */
void receiveIncomingWalkers(std::vector<Walker> *incomingWalkers,
                            const int &worldRank, const int &worldSize) {
  // Receive from the process before you. If you are process zero, receive from
  // the last process
  int incomingRank;
  if (worldRank == 0)
    incomingRank = worldSize - 1;
  else
    incomingRank = worldRank - 1;

  MPI_Status status;
  MPI_Probe(incomingRank, 0, MPI_COMM_WORLD, &status);

  // Resize your incoming walker buffer based on how much data is being
  // received
  int incomingWalkersSize;
  MPI_Get_count(&status, MPI_BYTE, &incomingWalkersSize);
  incomingWalkers->resize(incomingWalkersSize/sizeof(Walker));

  MPI_Recv((void*)incomingWalkers->data(),
              incomingWalkersSize,
              MPI_BYTE,
              incomingRank,
              0,
              MPI_COMM_WORLD,
              MPI_STATUS_IGNORE);
}

int main(int argc, char* argv[]) {
  if (argc < 4) {
    std::cerr << "Usage: randomWalk domainSize maxWalkSize "
              << "numWalkersPerProc\n";
    return 1;
  }

  int domainSize{std::stoi(argv[1])};
  int maxWalkSize{std::stoi(argv[2])};
  int numWalkersPerProc{std::stoi(argv[3])};

  MPI_Init(NULL, NULL);
  int worldSize;
  MPI_Comm_size(MPI_COMM_WORLD, &worldSize);
  int worldRank;
  MPI_Comm_rank(MPI_COMM_WORLD, &worldRank);

  srand(time(NULL)*worldRank);
  std::map<std::string, int> subdomain;
  std::vector<Walker> incomingWalkers, outgoingWalkers;

  // Find your part of the domain
  subdomain = decomposeDomain(domainSize, worldRank, worldSize);

  // Initialize walkers in your subdomain
  initializeWalkers(numWalkersPerProc, maxWalkSize, subdomain,
                    &incomingWalkers);

  std::cout << "Process " << worldRank << " initiated " << numWalkersPerProc
            << " walkers in subdomain " << subdomain["start"] << " - "
            << subdomain["start"] + subdomain["size"] - 1 << "\n";

  // Determine the maximum amount of sends and receives needed to complete all
  // walks
  int maximumSendsRecvs{maxWalkSize/(domainSize/worldSize) + 1};
  for (int m = 0; m < maximumSendsRecvs; ++m) {
    // Process all incoming walkers
    for (std::size_t i = 0; i < incomingWalkers.size(); ++i)
      walk(&incomingWalkers[i], subdomain, domainSize, &outgoingWalkers);

    std::cout << "Process " << worldRank << " sending  "
              << outgoingWalkers.size() << " outgoing walkers to process "
              << (worldRank + 1) % worldSize << "\n";

    if (worldRank % 2 == 0) {
      // Send all outgoing walkers to the next (even) process
      sendOutgoingWalkers(&outgoingWalkers, worldRank, worldSize);

      // Receive all the new incoming walkers
      receiveIncomingWalkers(&incomingWalkers, worldRank, worldSize);
    } else {
      // Receive all the new incoming walkers
      receiveIncomingWalkers(&incomingWalkers, worldRank, worldSize);

      // Send all outgoing walkers
      sendOutgoingWalkers(&outgoingWalkers, worldRank, worldSize);
    }

    std::cout << "Process " << worldRank << " received "
              << incomingWalkers.size() << " incoming walkers\n";
  }

  std::cout << "Process " << worldRank << " done\n";
  MPI_Finalize();
}
