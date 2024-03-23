#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

int do_cmd(const char *cmd) {
    pid_t pid = fork();

    if(pid == 0) {//Child
        execlp("sh", "sh", "-c", cmd, NULL);
        perror("execlp");
        exit(1);
    } else{ // Parent
        int res;
        waitpid(pid, &res, 0);

        if(WIFEXITED(res) && WEXITSTATUS(res) == 0) {
            return 0;
        } else {
            return -1;
        }
    }
}

int main() {
	int result = do_cmd("ls *.c");
}
