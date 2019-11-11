#include <iostream>
#include <memory>
#include <mpi/mpi.h>
#include <time.h>

/*
 * The length of a passed array can also be determined using the MPI_Probe
 * function. MPI_Probe can query a message without actually receiving it.
 * MPI_Probe will block, waiting for a message with a given tag and sender,
 * then retrieve the status of the message. Then MPI_Recv can be used to
 * retrieve the message using an array of the length calculated using the
 * status.
 */
int main() {
 int worldSize, worldRank;

  MPI_Comm_size(MPI_COMM_WORLD, &worldSize);
  MPI_Comm_rank(MPI_COMM_WORLD, &worldRank);

  int lengthOfArray;

  if (worldRank == 0) {
    const int MAN_LENGTH{100};
    int array[MAN_LENGTH];

     // Pick a random number to decide how long our message will be
    srand(static_cast<unsigned int>(time(NULL)));
    lengthOfArray = static_cast<int>(rand() / (float)RAND_MAX * MAN_LENGTH);
    MPI_Send(array, lengthOfArray, MPI_INT, 1, 0, MPI_COMM_WORLD);
    std::cout << "Process 0 sent " << lengthOfArray
              << " numbers to process 1.\n";

  } else if (worldRank == 1) {
    MPI_Status status;

    // Probe for incoming message and get the status
    MPI_Probe(0, 0, MPI_COMM_WORLD, &status);

    // Get the count from the status
    MPI_Get_count(&status, MPI_INT, &lengthOfArray);

    // Allocate some memory to put the message buffer into
    auto arrayBuff = std::make_unique<int>(new int[lengthOfArray]);
    MPI_Recv(arrayBuff.get(), lengthOfArray, MPI_INT, 0, 0, MPI_COMM_WORLD,
             MPI_STATUS_IGNORE);

    std::cout << "Process 1 received " << lengthOfArray << " numbers.\n"
              << "Message source: " << status.MPI_SOURCE << "\n"
              << "Message tag:    " << status.MPI_TAG << "\n";
  }
  return 0;
}