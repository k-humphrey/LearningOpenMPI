#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char* argv[]){
    /*setup*/

    int numOfProcesses, taskID, partnerID;
    long long message[numOfProcesses - 1];
    MPI_Comm comm = MPI_COMM_WORLD;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(comm, &numOfProcesses);
    MPI_Comm_rank(comm, &taskID);
    srand(time(NULL) + taskID);
    MPI_Request request;

    /*Not doing odd processes yet*/
    if(numOfProcesses % 2 != 0){
        if(taskID == 0){
        printf("I have not accounted for odd numbers of processes yet, so I'm exiting.");
        }
        MPI_Finalize();
        return 1;
    }

    /*Synchronize the start of all timers*/
    MPI_Barrier(comm);
    double start_time =  MPI_Wtime();

    /*We can set the amount of points we want to generate through the command line,
      each process has it's own circle_count variable,
      and we figure out an even number of points to generate per process*/
    long long npoints = atoll(argv[1]);
    long long circle_count = 0, total_count = 0;
    long long remainderPartition = npoints % numOfProcesses;
    long long myPartition = npoints/numOfProcesses; 
    float x, y;

    /*Deal with imbalances.. cheaply, 
    not giving it to task 0 because it already has extra work*/
    if(taskID == 1){
        myPartition += remainderPartition;
    }

    /*compute circle counts individually*/
    for(long long i = 0; i < myPartition; i++){
        /*generate two random points from 0 to 1*/
        x = (float)rand() / RAND_MAX;
        y = (float)rand() / RAND_MAX;
        if((x * x) + (y * y) <= 1.0){
            circle_count++;
        }
    }

    /*this function will take all of the data, sum it, and store it into total_count for task 0*/
    MPI_Reduce(&circle_count, &total_count, 1, MPI_INT, MPI_SUM, 0, comm);

    /*capture when each computation + send is done with*/
    double end_time =  MPI_Wtime();
    double total_time = end_time - start_time;

    /*then get the longest time and average time excluding task 0's true times*/
    double max_time, average_time;
    MPI_Reduce(&total_time, &max_time, 1, MPI_DOUBLE, MPI_MAX, 0, comm);
    MPI_Reduce(&total_time, &average_time, 1, MPI_DOUBLE, MPI_SUM, 0, comm);

    if(taskID == 0){
        float pi = 4.0 * total_count / npoints;

        /*capture task0's true end time*/
        end_time = MPI_Wtime();
        double old_total_time = total_time;
        total_time = end_time - start_time;

        /*recalculate average and check if we are the new max*/
        if(total_time > max_time){
            max_time = total_time;
        }
        average_time +=  total_time - old_total_time;
        average_time /= numOfProcesses;

        printf("Pi has been approximated to: %f\n", pi);
        printf("Max seconds: %f Average seconds: %f\n", max_time, average_time);
    }

    MPI_Finalize();
    return 0;
}