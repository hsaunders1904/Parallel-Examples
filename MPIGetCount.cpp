/*
 * In MPISendReceive we showed how to send messages of known length. MPI also
 * supports dynamic length messages - with a few additional calls.
 */
#include <iostream>
#include <mpi/mpi.h>
#include <time.h>

/*
 * This example uses MPI_Get_Count to measure the size of a received message.
 * We create an array of the maximum size we expect, we then send out an array
 * of integers of random length. The receiver copies the retrieved array into
 * our maximum sized array, then uses MPI_Get_count to query the status that
 * was passed with the message. This calculates the length of the passed array
 * using the size of the array in bytes and the type that was passed.
 */
int main() {
  const int MAX_LENGTH{100};
  int numbers[MAX_LENGTH], arrayLength;
  int worldSize, worldRank;

  MPI_Init(NULL, NULL);
  MPI_Comm_size(MPI_COMM_WORLD, &worldSize);
  MPI_Comm_rank(MPI_COMM_WORLD, &worldRank);

  if (worldRank == 0) {
    // Pick a random number to decide how long our message will be
    srand(static_cast<unsigned int>(time(NULL)));
    arrayLength = static_cast<int>(rand() / (float)RAND_MAX * MAX_LENGTH);

    // Send the number of integers to process 1
    MPI_Send(numbers, arrayLength, MPI_INT, 1, 0, MPI_COMM_WORLD);

    std::cout << "Process 0 sent " << arrayLength
              << " numbers to process 1.\n";
  } else if (worldRank == 1) {
    // Receive at most MAX_NUMBERS from process 0
    MPI_Status status;
    MPI_Recv(numbers, MAX_LENGTH, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);

    // After receiving, check the status to determine how many numbers were
    // actually received
    MPI_Get_count(&status, MPI_INT, &arrayLength);

    std::cout << "Process 1 received " << arrayLength << " numbers.\n"
              << "Message source: " << status.MPI_SOURCE << "\n"
              << "Message tag:    " << status.MPI_TAG << "\n";
  }
  MPI_Finalize();
}
