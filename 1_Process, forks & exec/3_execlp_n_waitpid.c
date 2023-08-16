#include <stdio.h>
#include <unistd.h>
#include <sys/types.h> //cuz of pid_t
#include <sys/wait.h> //cuz of waitpid()

//gcc 3_execlp_n_waitpid.c

int main(int argc, char* argv[]) {
    //ADDED
    if(argc==2 && *argv[1]=='1') {
        printf("Detected running prog w/ args %s \n", argv[1]);
        return 10;
    }
	
	pid_t pid = fork(); //pid==0 for the Child, pid==Child_PID for the Parent
	if (pid == 0) {
        printf("CHILD: pid=%d; Parent PID=%d\n", getpid(), getppid());

        //ADDED - The child process executes this program w/ arguments
        execlp("./a.out", "a", "1", NULL); //make sure a.out is this program
		puts("SHOULD NEVER GET HERE OR FURTHER!");
    }
    else if (pid == -1) puts("Out of memory O.O ?");
	else {
        printf("PARENT: pid=%d; Child PID=%d\n", getpid(), pid);

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