#include "mpi.h"
#include <stdio.h>


int main(int argc, char* argv[]){
    int numOfProcesses, taskID, resultLength, partnerID;
    char name[MPI_MAX_PROCESSOR_NAME];
    MPI_Comm comm = MPI_COMM_WORLD;

    /* 
       initialize MPI passing command line arguments to each process,
       set communicator and learned how many processes we have access to,
       and learned the current processes taskID/rank,
       Also got the processor name (hostname)
    */
    MPI_Init(&argc, &argv);
    MPI_Comm_size(comm, &numOfProcesses);
    MPI_Comm_rank(comm, &taskID);
    MPI_Get_processor_name(&name, &resultLength);
    if(taskID < (numOfProcesses/2)){
        partnerID = (numOfProcesses/2) + taskID;
    }
    else{
        partnerID = taskID - (numOfProcesses/2);
    }
    /*
        Now I can use taskID to dictate what my processes do.
        taskID 0 is the main thread, sometimes called master. It is the first to start.
        any other taskIDs are other processes than main.
    */
    if(taskID == 0){
        printf("I am the Main Process!\n");
        printf("My Host Name is %s\n", name);
        printf("My taskID is %d\n", taskID);
        printf("I can see that there are %d available processes\n\n", numOfProcesses);
        //need to send and recieve
    }
    else{
        /*The 'workers'*/
        //need to send and recieve
    }

    /* finalize MPI's environment*/
    MPI_Finalize();
}