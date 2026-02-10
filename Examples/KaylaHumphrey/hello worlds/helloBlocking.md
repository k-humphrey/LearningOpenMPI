if(taskID < (numOfProcesses/2)){
        partnerID = (numOfProcesses/2) + taskID;
}
else{
        partnerID = taskID - (numOfProcesses/2);
}
Given 8 processes, I expect this:
task 0 is partnered with task 4
task 1 is partnered with task 5
task 2 is partnered with task 6
task 3 is partnered with task 7

How do we do blocking send and recieve?
MPI_Send is going to return after the buffer is available for use, not necessarily when the message has been recieved.
So, we need to start each task with sending, and then waiting to recieve. This should ensure that there are no stalls.