Synchronization operation. Creates a barrier synchronization in a group. Each task, when reaching the MPI_Barrier call, blocks until all tasks in the group reach the same MPI_Barrier call. Then all tasks are free to proceed.

MPI_Barrier (comm)
MPI_BARRIER (comm,ierr)

Collective computation operation. Applies a reduction operation on all tasks in the group and places the result in one task. Diagram here

MPI_Reduce (&sendbuf,&recvbuf,count,datatype,op,root,comm)
MPI_REDUCE (sendbuf,recvbuf,count,datatype,op,root,comm,ierr)
MPI_SUM 	sum 	integer, float 

srand

https://apurvanakade.github.io/Monte-Carlo-Methods/chapters/estimation/estimating_pi.html

PI = 4.0*circle_count/npoints