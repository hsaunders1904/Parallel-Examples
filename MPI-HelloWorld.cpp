#include <iostream>
#include <mpi/mpi.h>

int main() {
  // Initialize the MPI environment
  /*
   * MPI_Init constructs all of MPI's global and internal variables.
   * For example, a communicator is formed around all of the processes
   * that were spawned and unique ranks are assigned to each process.
   */
  MPI_Init(NULL, NULL);

  // Get the number of processes
  /*
   * MPI_Comm_size returns the size of the communicator. In this
   * example, MPI_COMM_WORLD (which MPI constructs) encloses all of the
   * processes in the job, so this call should return the amount of
   * processes that were requested for the job on the command line.
   */
  int worldSize;
  MPI_Comm_size(MPI_COMM_WORLD, &worldSize);

  // Get the rank of the process
  /*
   * MPI_Comm_rank returns the rank of a process in a communicator.
   * Each process inside of a communicator is assigned an incremental
   * rank starting from 0. The ranks of the processes are primarily
   * used for identification purposes when sending and receiving
   * messages.
   */
  int worldRank;
  MPI_Comm_rank(MPI_COMM_WORLD, &worldRank);

  // Get the name of the processor
  /*
   * MPI_Get_processor_name obtains the actual name of the processor on
   * which the process is executing.
   */
  char processorName[MPI_MAX_PROCESSOR_NAME];
  int nameLen;
  MPI_Get_processor_name(processorName, &nameLen);

  std::cout << "Hello, world, from processor " << processorName << ", rank "
            << worldRank << " out of " << worldSize << " processors\n";

  // Finalize the MPI environment
  /*
   * MPI_Finalize cleans up the MPI environment. No more MPI calls can
   * be made after this call.
   */
  MPI_Finalize();
}