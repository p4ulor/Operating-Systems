#include <stdio.h>
#include <unistd.h>
#include <sys/types.h> //cuz of pid_t

//gcc 2_fork_n_child.c

int main() {
    int value = 10;
    int amIParent = 0;
	
	printf("PARENT: pid=%d\n", getpid());
	puts("PARENT: fork()");
	
	pid_t pid = fork(); //pid==0 for the Child, pid==Child_PID for the Parent
	if (pid == 0) {
        printf("CHILD: pid=%d; Parent PID=%d. And I changed value\n", getpid(), getppid());
        value += 10;
    }
    else if (pid == -1) puts("Out of memory O.O ?");
	else {
        amIParent = 1;
        sleep(2);
        printf("PARENT: pid=%d; Child PID=%d\n", getpid(), pid);
    }

    char* process = amIParent ? "parent" : "child";
    printf("I, %s w/ PID=%d see value at (virtual) address %p with %d\n", process, getpid(), &value, value); //but the value themselves live in different physical addresses
	
	return 0;
}