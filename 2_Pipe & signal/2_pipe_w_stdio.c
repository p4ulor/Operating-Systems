#include <stdio.h>
#include <unistd.h> //for pipe(), close()
#include <sys/types.h> //cuz of pid_t
#include <sys/wait.h> //cuz of waitpid()

//gcc 2_pipe_w_stdio.c.c

int main(int argc, char* argv[]) {
    int pipefd[2];
	pipe(pipefd); //if no other file descp are set, the array will be [3, 4]
	
	pid_t pid = fork(); //pid==0 for the Child, pid==Child_PID for the Parent
	if (pid == 0) {
        printf("CHILD: pid=%d; Parent PID=%d\n", getpid(), getppid());

        close(pipefd[0]);
        puts("Will say hi to parent");
		sleep(2);

        //ALTERED - Replace stdout for pipe
        dup2(pipefd[1], 1); 
        printf("Hi parent\n");
    }
    else if (pid == -1) puts("Out of memory O.O ?");
	else {
        printf("PARENT: pid=%d; Child PID=%d\n", getpid(), pid);

        close(pipefd[1]); // fechar a ponta que não vai usar
		printf("PARENT: waiting for message from child\n");

        //ALTERED - Replace stdin for pipe
        dup2(pipefd[0], 0);
		char msg[11];
        while(fgets(msg, 11, stdin) !=NULL){ //blocking
            printf("PARENT: child sent message %s\n", msg);
        }
    }

	return 0;
}