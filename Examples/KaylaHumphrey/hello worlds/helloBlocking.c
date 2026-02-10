#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>


int main(int argc, char* argv[]){
    int numOfProcesses, taskID, resultLength, partnerID, message;
    char name[MPI_MAX_PROCESSOR_NAME];
    MPI_Comm comm = MPI_COMM_WORLD;
    MPI_Status status;

    /* 
       initialize MPI passing command line arguments to each process,
       set communicator and learned how many processes we have access to,
       and learned the current processes taskID/rank,
       Also got the processor name (hostname)
    */
    MPI_Init(&argc, &argv);
    MPI_Comm_size(comm, &numOfProcesses);
    MPI_Comm_rank(comm, &taskID);
    MPI_Get_processor_name(name, &resultLength);

    /* We can't do odd numbers of processes yet*/
    if(numOfProcesses % 2 != 0){
        //just have one process print out instead of all of them.
        if(taskID == 0){
        printf("I have not accounted for odd numbers of processes yet, so I'm exiting.");
        }
        MPI_Finalize();
        return 1;
    }

    /*Determine partnerID*/
    if(taskID < (numOfProcesses/2)){
        partnerID = (numOfProcesses/2) + taskID;
    }
    else if(taskID >= (numOfProcesses/2)){
        partnerID = taskID - (numOfProcesses/2);
    }
    /*
        Now we can do blocking send and recieve
    */
    MPI_Sendrecv(&taskID, 1, MPI_INT, partnerID, 1, &message, 1, MPI_INT, partnerID, 1, comm, &status);
    printf("Task %d has received message: %d\n", taskID, message);

    /* finalize MPI's environment*/
    MPI_Finalize();
}