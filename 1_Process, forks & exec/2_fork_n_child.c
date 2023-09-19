#include <stdio.h>
#include <unistd.h>
#include <sys/types.h> //cuz of pid_t
#include <sys/wait.h> //cuz of wait() https://man7.org/linux/man-pages/man2/wait.2.html

//gcc 2_fork_n_child.c

int main() {
    int value = 10;
    int amIParent = 0;
	
	printf("PARENT: pid=%d\n", getpid());
	puts("PARENT: fork()");
	
	pid_t pid = fork(); //pid==0 for the Child, pid==Child_PID for the Parent
	if (pid == 0) {
        printf("CHILD: pid=%d; Parent PID=%d. Value=%d And I added 10 to value\n", getpid(), getppid(), value);
        value += 10;
        return value;
    }
    else if (pid == -1) puts("Out of memory O.O ?");
	else {
        amIParent = 1;
        sleep(2);
        printf("PARENT: pid=%d; Child PID=%d\n", getpid(), pid);

        //wait(NULL); //wait for any but only 1 child to end
        pid_t child_pid;
        int childReturn = 0;
        while ((child_pid = wait(&childReturn)) > 0); // this way, the father waits for all the child processes
        if(WIFEXITED(childReturn)){
            printf("Last child returned %d\n", WEXITSTATUS(childReturn)); //WEXITSTATUS converts the returned value into a valid one. This is because the OG returned value is bundled w/ other info, and WEXITSTATUS extracts what's our actual return value by the process
        }
    }

    char* process = amIParent ? "parent" : "child";
    printf("I, %s w/ PID=%d see value at (virtual) address %p with %d\n", process, getpid(), &value, value); //but the value themselves live in different physical addresses
	
	return 0;
}