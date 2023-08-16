#include <stdio.h>
#include <unistd.h> //for pipe(), close()
#include <sys/types.h> //cuz of pid_t
#include <sys/wait.h> //cuz of waitpid()

//gcc 1_pipe.c

int main(int argc, char* argv[]) {
    int pipefd[2];
	pipe(pipefd); //if no other file descp are set, the array will be [3, 4]
	
	pid_t pid = fork(); //pid==0 for the Child, pid==Child_PID for the Parent
	if (pid == 0) {
        printf("CHILD: pid=%d; Parent PID=%d\n", getpid(), getppid());

        close(pipefd[0]);
        puts("Will say hi to parent");
		sleep(2);
		write(pipefd[1], "Hi parent\n", 10);
    }
    else if (pid == -1) puts("Out of memory O.O ?");
	else {
        printf("PARENT: pid=%d; Child PID=%d\n", getpid(), pid);

        close(pipefd[1]); // fechar a ponta que não vai usar
		printf("PARENT: waiting for message from child\n");
		char msg[11];
		read(pipefd[0], msg, 10); //blocking
        printf("PARENT: child sent message %s\n", msg);
    }

	return 0;
}