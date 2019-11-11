#include <iostream>
#include <mpi/mpi.h>

/*
  MPI's send and receive calls operate in the following manner:

  - Process A decides a message needs to be sent to process B.
  - A packs up all necessary data into a buffer for process B.
      These buffers are often called envelopes
  - The communication device (often a network) is responsible for
    routing the message to the proper location (defined by rank).
  - Process B must acknowledge it wants to receive the envelope.
  - Once it does this, the data has been transmitted.
  - Process A is told that the data has been transmitted and may go
    back to work.

  In some cases A might have many different types of message to send
  to B. So B can differentiate between messages, message IDs can be
  specified. These are known as tags.

  When process B only requests a message with a certain tag number,
  messages with different tags will be buffered by the network until
  B is ready for them.
*/

/* MPI Send and Receive functions

  MPI_Send(
    void *data,
    int count,
    MPI_Datatype datatype,
    int destination,
    int tag,
    MPI_Comm communicator
  );

  MPI_Recv(
    void *data,
    int count,
    MPI_Datatype datatype,
    int destination,
    int tag,
    MPI_Comm communicator,
    MPI_STATUS *status
  );

*/

/*
 * In this function, process 0 initializes a number and sends it to process 1.
 * Process 1 then prints the number.
 */
int main() {

  MPI_Init(NULL, NULL);

  //  Find out rank and size
  int worldRank;
  int worldSize;
  MPI_Comm_rank(MPI_COMM_WORLD, &worldRank);
  MPI_Comm_size(MPI_COMM_WORLD, &worldSize);

  int number;
  if (worldRank == 0) {
    number = 123;
    MPI_Send(
      &number,       // The data we're sending (buffer)
      1,             // Number of elements in the buffer
      MPI_INT,       // Type of buffer elements
      1,             // Rank of receiving process
      0,             // The message tag
      MPI_COMM_WORLD // The communicator
      );
  } else if (worldRank == 1) {
    int myNumber;
    MPI_Recv(
      &myNumber,        // Buffer to insert data into
      1,                // Number of elements to put in the output buffer
      MPI_INT,          // The data type
      0,                // Rank of the sending process
      0,                // The message tag
      MPI_COMM_WORLD,   // The communicator
      MPI_STATUS_IGNORE // Status variable (we don't output a status here)
    );
    std::cout << "Process 1 received number " << myNumber
              << " from process 0\n";
  }

  MPI_Finalize();
  return 0;
}