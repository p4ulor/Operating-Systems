#include <stdio.h>
#include <unistd.h> //for pipe(), close()
#include <sys/types.h> //cuz of pid_t
#include <sys/wait.h> //cuz of waitpid()

//gcc 1_pipe.c

int main(int argc, char* argv[]) {
    int pipefd[2]; // pipefd[0] for reads; pipefd[1] for writes
	pipe(pipefd); //if no other file descp are set, the array will be [3, 4]. 
	
	pid_t pid = fork(); //pid==0 for the Child, pid==Child_PID for the Parent
	if (pid == 0) {
        printf("CHILD: pid=%d; Parent PID=%d\n", getpid(), getppid());

        close(pipefd[0]); // close the edge we're not gonna use (not really required in some cases)
        puts("Will say hi to parent");
		sleep(2);

        dup2(pipefd[1], 1); //duplicate stdout of child to the write index of the pipe (in my mind I understand it better if its worded this way, but the more accurate thing to say is: make pipefd[1] points to 1)
        write(1, "Hi parent", 10); //makes use of prev instruction
        //write(pipefd[1], "Hi parent", 10); //could run without the need of dup2
    }
    else if (pid == -1) puts("Out of memory O.O ?");
	else {
        printf("PARENT: pid=%d; Child PID=%d\n", getpid(), pid);

        close(pipefd[1]); //close other edge (not really required in some cases)
		printf("PARENT: waiting for message from child\n");

        //MUST WAIT FOR CHILD PROCESS TO FINISH (try removing it)
        int childReturnedValue; //will have a specially formatted value, not the plain return value
        waitpid(pid, &childReturnedValue, 0); //modify childReturnedValue with the proper value

        //Read from the read pipe edge, which contains bytes from the write edge
        //dup2(pipefd[0], 0); //not needed
		char msg[10];
        read(pipefd[0], msg, 10); //blocking
        printf("PARENT: child sent message %s. With return value = %d\n", msg, childReturnedValue);
    }

	return 0;
}