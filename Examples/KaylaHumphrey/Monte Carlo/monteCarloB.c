#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char* argv[]){
    double start_time =  MPI_Wtime();
    /*setup*/
    int numOfProcesses, taskID, resultLength, partnerID, message;
    MPI_Comm comm = MPI_COMM_WORLD;
    MPI_Status status;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(comm, &numOfProcesses);
    MPI_Comm_rank(comm, &taskID);
    srand(time(NULL) + taskID);

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
        MPI_Send(&circle_count, 1, MPI_INT, 0, 1, comm);
    }

    /*MPI barrier makes sure that all processes in the comm group finish before we recieve and calculate pi*/
    MPI_Barrier(comm);

    /*now we need to combine circle_count to get our approximation*/
    if(taskID == 0){
        for(int i = 1; i < numOfProcesses; i++){
            MPI_Recv(&message, 1, MPI_INT, i, 1, comm, &status);
            circle_count += message;
        }
        float pi = 4.0 * circle_count / npoints;
        double end_time =  MPI_Wtime();
        printf("Pi has been approximated to: %f\n", pi);
        printf("This approximation took %f seconds.\n", end_time - start_time);
    }

    MPI_Finalize();
    return 0;
}