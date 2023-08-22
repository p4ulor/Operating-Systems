#include <stdio.h>
#include <unistd.h>
#include <sys/types.h> //cuz of pid_t
#include <sys/wait.h> //cuz of waitpid()

// gcc 4_exclp_w_grep_2_way_comunication.c

int main(int argc, char* argv[]) {

    int pipefd[2];
    int pipefd_parentToChild[2];
	pipe(pipefd);
    printf("pipe[0]=%d pipe[1]=%d\n", pipefd[0], pipefd[1]);
    pipe(pipefd_parentToChild);
    printf("pipefd_parentToChild[0]=%d pipefd_parentToChild[1]=%d\n", pipefd_parentToChild[0], pipefd_parentToChild[1]);
	
    pid_t pid = fork();
	if (pid == 0) {
        printf("CHILD: pid=%d; Parent PID=%d\n", getpid(), getppid());

        //dup2(pipefd_parentToChild[0], 0); //make stdin of child point to a read edge that the parent writes to
        dup2(pipefd[1], 1); //make stdout of child point to a write pipe

        char msg[300];
		read(pipefd_parentToChild[0], msg, 300); //blocking
        printf("CHILD: parent sent message %s\n", msg);

        //ADDED - The child process executes this program w/ arguments
        execlp("grep", "grep", msg, "4_exclp_w_grep.c", NULL); //make sure a.out is this program
		puts("SHOULD NEVER GET HERE OR FURTHER!");
    }
    else if (pid == -1) puts("Out of memory O.O ?");
	else {
        printf("PARENT: pid=%d; Child PID=%d\n", getpid(), pid);

        write(pipefd_parentToChild[1], "#", 2);

        //MUST WAIT FOR CHILD PROCESS TO FINISH (try removing it)
        int childReturnedValue; //will have a specially formatted value, not the plain return value
        waitpid(pid, &childReturnedValue, 0);

        char msg[300];
		int bytesRead = read(pipefd[0], msg, 300); //blocking
        printf("PARENT: child sent message %s %d\n", msg, bytesRead);
    }

	return 0;
}