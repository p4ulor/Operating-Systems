#include <stdio.h>
#include <unistd.h>
#include <sys/types.h> //cuz of pid_t
#include <sys/wait.h> //cuz of waitpid()

#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>

int main(){

    int file = open("logFile.txt", O_APPEND | O_WRONLY | O_CREAT, 0777);
    if( 0 > file ){
        perror("open logFile failed");
        exit(EXIT_FAILURE);
    }

    if(dup2(file, STDOUT_FILENO) == -1){
        perror("dup2 failed");
        exit(EXIT_FAILURE);
    }

    printf("Program wrote to logfile");

    return 0;
}
