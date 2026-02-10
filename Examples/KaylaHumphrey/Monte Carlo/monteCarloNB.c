#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char* argv[]){
    /*setup*/
    int numOfProcesses, taskID, resultLength, partnerID, message[numOfProcesses - 1];
    MPI_Comm comm = MPI_COMM_WORLD;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(comm, &numOfProcesses);
    MPI_Comm_rank(comm, &taskID);
    srand(time(NULL) + taskID);
    MPI_Request request[numOfProcesses - 1];

    /*Not doing odd processes yet*/
    if(numOfProcesses % 2 != 0){
        if(taskID == 0){
        printf("I have not accounted for odd numbers of processes yet, so I'm exiting.");
        }
        MPI_Finalize();
        return 1;
    }

    /*We can set the amount of points we want to generate through the command line,
      each process has it's own circle_count variable,
      and we figure out an even number of points to generate per process*/
    int npoints = atoi(argv[1]);
    int circle_count = 0;
    int remainderPartition = npoints % numOfProcesses;
    int myPartition = npoints/numOfProcesses; 
    float x, y;

    /*handle the remainder for now by assigning it to task 0*/
    if(taskID == 0){
        myPartition += remainderPartition;
    }
    /*compute circle counts individually*/
    for(int i = 0; i < myPartition; i++){
        /*generate two random points from 0 to 1*/
        x = (float)rand() / RAND_MAX;
        y = (float)rand() / RAND_MAX;
        if((x * x) + (y * y) <= 1.0){
            circle_count++;
        }
    }

    /*send data to task 0*/
    if(taskID != 0){
        MPI_Isend(&circle_count, 1, MPI_INT, 0, 1, comm, &request[taskID - 1]);
    }

    

    /*now we need to combine circle_count to get our approximation*/
    if(taskID == 0){
        /*spawn off all of the recieving at once, then wait for all to be recieved*/
        for(int i = 1; i < numOfProcesses; i++){
            MPI_Irecv(&message[i - 1], 1, MPI_INT, i, 1, comm, &request[i - 1]);
        }
        MPI_Waitall(numOfProcesses - 1, request, MPI_STATUS_IGNORE);
        /*combine counts from all processes*/
        int total_count = circle_count; 
        for(int i = 0; i < numOfProcesses - 1; i++){
            total_count += message[i];   
        }
        float pi = 4.0 * total_count / npoints;
        printf("Pi has been approximated to: %f\n", pi);
    }

    MPI_Finalize();
    return 0;
}