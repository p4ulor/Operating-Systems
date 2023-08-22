#include <stdio.h>
#include <unistd.h>
#include <sys/types.h> //cuz of pid_t
#include <sys/wait.h> //cuz of waitpid()

// gcc 4_exclp_w_grep.c

int main(int argc, char* argv[]) {
    int pipefd[2];
	pipe(pipefd);
    printf("pipe[0]=%d pipe[1]=%d\n", pipefd[0], pipefd[1]);
	pid_t pid = fork();
	if (pid == 0) {
        printf("CHILD: pid=%d; Parent PID=%d\n", getpid(), getppid());

        //dup2(pipefd[0], 0); //make stdin of child point to a read pipe edge
        dup2(pipefd[1], 1); //make stdout of child point to a write pipe

        //ADDED - The child process executes this program w/ arguments
        execlp("grep", "grep", "#", "4_exclp_w_grep.c", NULL); //make sure a.out is this program
		puts("SHOULD NEVER GET HERE OR FURTHER!");
    }
    else if (pid == -1) puts("Out of memory O.O ?");
	else {
        printf("PARENT: pid=%d; Child PID=%d\n", getpid(), pid);

        //dup2(pipefd[0], 1); //make Parent stdout point to pipefd[1]

        char msg[300];
		read(pipefd[0], msg, 300); //blocking, use the other pipe edge to read the bytes written from the pipefd[1] edge
        printf("PARENT: child sent message %s\n", msg);

        //ADDED - The parent must call waitpid for it's child processes to finish, otherwise, the child's process will be in a state called Zombie (after it exists with a return value), because the Child is waiting for the Parent to collect it's returned value. If the Parent is terminated, the Child will be orphan, and the process 1 ( AKA process "init" ran by the Unix OS) will perform that call to terminate the child process
        int childReturnedValue; //will have a specially formatted value, not the plain return value
        waitpid(pid, &childReturnedValue, 0); //The parent will wait for the child process to finish. 0 is the "options"

        if(WIFEXITED(childReturnedValue))
            printf("PARENT %d: Child returned %d\n", getpid(), WEXITSTATUS(childReturnedValue)); //Using WEXITSTATUS because the returned value is shifted left by 8, and this operation unshifts by 8. https://stackoverflow.com/a/62635128
        else
            puts("Unexpected child termination");
    }

	return 0;
}