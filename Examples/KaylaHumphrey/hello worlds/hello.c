#include "mpi.h"
#include <stdio.h>


int main(int argc, char* argv[]){
    int numOfProcesses, taskID, resultLength;
    char name[MPI_MAX_PROCESSOR_NAME];
    MPI_Comm comm = MPI_COMM_WORLD;
    printf("A process has started the program for the first time.\n");

    /* 
       initialize MPI passing command line arguments to each process,
       set communicator and learned how many processes we have access to,
       and learned the current processes taskID/rank,
       Also got the processor name
    */
    MPI_Init(&argc, &argv);
    MPI_Comm_size(comm, &numOfProcesses);
    MPI_Comm_rank(comm, &taskID);
    MPI_Get_processor_name(&name, &resultLength);
    /*
        Now I can use taskID to dictate what my processes do.
        taskID 0 is the main thread, sometimes called master. It is the first to start.
        any other taskIDs are other processes than main.
    */
    if(taskID == 0){
        printf("I am the First Process!\n");
        MPI_Get_processor_name(&name, &resultLength);
        printf("My processor is %s\n", name);
        printf("My taskID is %d\n", taskID);
        printf("I can see that there are %d available processes\n\n", numOfProcesses);

    }
    else{
        printf("I am process: %d and my processor name is: %s\n", taskID, name);
        printf("I can communicate with %d other processes\n\n", numOfProcesses);
    }

    /* finalize MPI's environment*/
    MPI_Finalize();
}