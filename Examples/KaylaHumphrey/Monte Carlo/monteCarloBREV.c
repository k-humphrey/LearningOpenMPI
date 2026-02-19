/*Kayla Humphrey
Revised Monte Carlo Blocking Program*/
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char** argv){

    //MPI initialization
    MPI_Init(&argc, &argv);
    MPI_Comm comm = MPI_COMM_WORLD;

    // set global MPI variables (those faux shared variables)
    int GLOBAL_PROCESSES;
    MPI_Comm_size(comm, &GLOBAL_PROCESSES);
    long GLOBAL_NPOINTS = atol(argv[1]); //really we should broadcast this from rank 0 for other use cases

    //synchronize and start local timer for all processes
    MPI_Barrier(comm);
    double local_start_time = MPI_Wtime();

    //set MPI local variables
    int local_rank;
    MPI_Comm_rank(comm, &local_rank);
    long local_circle_count = 0;
    long local_partition = GLOBAL_NPOINTS / GLOBAL_PROCESSES;

    //calculate remainder and distribute it evenly
    int GLOBAL_REMAINDER = GLOBAL_NPOINTS % GLOBAL_PROCESSES;
    if(local_rank > 0  && local_rank <= GLOBAL_REMAINDER){
        local_partition = local_partition + 1;
    }
    
    //do Monte Carlo Approximation
    srand48(time(NULL) + local_rank);
    for(int i = 0; i < local_partition; i++){
        double local_x = drand48();
        double local_y = drand48();
        if((local_x * local_x) + (local_y * local_y) <= 1.0){
            local_circle_count = local_circle_count + 1;
        }
    }

    //send to process 0
    if(local_rank != 0){
        MPI_Send(&local_circle_count, 1, MPI_LONG, 0, 1, comm);
    }
    
    //approximate pi in rank 0
    double pi;
    if(local_rank == 0){
        long local_message;
        long total_circle_count = local_circle_count; //get task 0's local_circle_count

        //read all circle counts, and add them to the total
        for(int i = 1; i < GLOBAL_PROCESSES; i++){
            MPI_Recv(&local_message, 1, MPI_LONG, i, 1, comm, MPI_STATUS_IGNORE);
            total_circle_count = total_circle_count + local_message;
        }
        pi = 4.0 * total_circle_count / GLOBAL_NPOINTS;
    }

    //reduce to find the maximum time, and the average time.
    double max_time, total_average_time;
    double local_end_time = MPI_Wtime();
    double local_total_time = local_end_time - local_start_time;
    MPI_Reduce(&local_total_time, &max_time, 1, MPI_DOUBLE, MPI_MAX, 0, comm);
    MPI_Reduce(&local_total_time, &total_average_time, 1, MPI_DOUBLE, MPI_SUM, 0, comm);

    //let rank 0 print results and make the sum an average.
    if(local_rank == 0){
        //take the sum of the times, and make them into the average
        total_average_time = total_average_time / GLOBAL_PROCESSES;
        //print results
        printf("Pi has been approximated to: %f\n", pi);
        printf("Max seconds: %f Average seconds: %f\n", max_time, total_average_time);
    }
    
    MPI_Finalize();
    return 0;
}