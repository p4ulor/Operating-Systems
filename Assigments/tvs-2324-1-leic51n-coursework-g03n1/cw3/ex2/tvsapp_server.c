#include <systemd/sd-daemon.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <fcntl.h> //for file flags

#include <pthread.h>
#include "tvsapp_lib.h"

#define BUFSIZE 4096

char *cmd[16];
int cmd_size;

void terror(char *msg) {
	perror(msg);
	pthread_exit((void *)(intptr_t)1);
}

void splitbuff(char buff[]){
	char *ptr = strtok(buff, " ");
    int i = 0;
	while (ptr != NULL){
		cmd[i] = ptr;
        i++;
        ptr = strtok(NULL, " ");
	}
    cmd[i] = NULL;
    cmd_size = i;
}

void * process_connection(void * ptr) {
	int conn_fd = (int)(intptr_t)ptr;

    char instructions_cmd[128];	//Command to execute(reset | inc | dec | stop | start | status)
	char bash_cmd[] = "bash";
    char server_dir[] = "/opt/isel/tvs/tvsapp/bin";
	
    //  Messages to send to the client
	char reset_msg[]  = "Reset Executed";
	char inc_msg[]    = "Inc Executed";
	char dec_msg[]    = "Dec Executed";
	char stop_msg[]   = "Stop Executed";
    char start_msg[]  = "Start Executed";
    //char status_msg[] = "Status Executed";
	char quit_msg[]   = "End server";

	char buf[BUFSIZE];
	int len;

    FILE* commandOutputPointer = NULL;
    char commandOutputBuffer[BUFSIZE];

	for (;;) {
		len = read(conn_fd, buf, BUFSIZE);
		if (len < 0) {
			terror("ERROR reading from socket");
		}
		buf[len] = 0;
		printf("server received %d bytes: %s\n", len, buf);

        splitbuff(buf);
        if((strncmp(cmd[0], "tvsctl", 6) == 0 && cmd[1] != NULL)) {

            if ((strncmp(cmd[1], "reset", 5) == 0)) {
                sprintf(instructions_cmd, "%s %s/%s %s %s", bash_cmd, server_dir, RESET, cmd[2], cmd[3]); //cmd[2] = scale | cms[3] = base 
                system(instructions_cmd);
                len = write(conn_fd, reset_msg, strlen(reset_msg)+1);
            }

            if ((strncmp(cmd[1], "inc", 3) == 0)) {
                sprintf(instructions_cmd, "%s %s/%s %s", bash_cmd, server_dir, INC, cmd[2]);             //cmd[2] = delta
                system(instructions_cmd);
                len = write(conn_fd, inc_msg, strlen(inc_msg)+1);
            }

            if ((strncmp(cmd[1], "dec", 3) == 0)) {
                sprintf(instructions_cmd, "%s %s/%s %s", bash_cmd, server_dir, DEC, cmd[2]);             //cmd[2] = delta
                system(instructions_cmd);
                len = write(conn_fd, dec_msg, strlen(dec_msg)+1);
            }

            if ((strncmp(cmd[1], "stop", 4) == 0)) {
                sprintf(instructions_cmd, "%s %s/%s %s", bash_cmd, server_dir, STOP, cmd[2]);            //cmd[2] = db
                system(instructions_cmd);
                len = write(conn_fd, stop_msg, strlen(stop_msg)+1);
            }

            if ((strncmp(cmd[1], "start", 5) == 0)) {
                sprintf(instructions_cmd, "%s %s/%s", bash_cmd, server_dir, START);
                system(instructions_cmd);
                len = write(conn_fd, start_msg, strlen(start_msg)+1);
            }

            if ((strncmp(cmd[1], "status", 6) == 0)) {
                sprintf(instructions_cmd, "%s %s/%s", bash_cmd, server_dir, STATUS);
                //len = write(conn_fd, instructions_cmd, strlen(instructions_cmd)+1);
                //system(instructions_cmd);

                commandOutputPointer = popen(instructions_cmd, "r"); //https://stackoverflow.com/a/1583259 https://pubs.opengroup.org/onlinepubs/9699919799/functions/popen.html
                
                while (fgets(commandOutputBuffer, sizeof(commandOutputBuffer), commandOutputPointer) != NULL) {
                    len = write(conn_fd, commandOutputBuffer, strlen(commandOutputBuffer));
                    
                    if (len == -1) {
                        len = write(conn_fd, "error", strlen("error")+1);
                        exit(EXIT_FAILURE);
                    }
                }

                //len = write(conn_fd, status_msg, strlen(status_msg)+1);
            }

            memset(commandOutputBuffer, 0, sizeof(commandOutputBuffer)); //Clear command output buffer
            memset(instructions_cmd, 0, sizeof(instructions_cmd)); //Clear Instruction Command
        } else {
            if (strncmp(buf, "stop", 4) == 0 && buf[4] < ' ') {
                puts(":: EXIT ::");
                exit(0);
            }
        }

        if(commandOutputPointer==NULL){
            if (pclose(commandOutputPointer) == -1) {
                perror("pclose");
                exit(EXIT_FAILURE);
            }
        }
        

		if (len == 0 || (strncmp(buf, "quit", 4) == 0 && buf[4] < ' ')) {
            len = write(conn_fd, quit_msg, strlen(quit_msg)+1);
			close(conn_fd);
			break;
		}

		if (len < 0) {
			terror("ERROR writing to socket");
		}
		
	}

	return NULL;
}

void error(char *msg) {
	perror(msg);
	exit(EXIT_FAILURE);
}

void configLogFile(){
    int file = open("logFile.txt", O_APPEND | O_WRONLY | O_CREAT, 0x777);
    if(0 > file){
        perror("open logFile failed");
        exit(EXIT_FAILURE);
    }

    if(dup2(file, STDOUT_FILENO) == -1){
        perror("dup2 failed for stdout");
        exit(EXIT_FAILURE);
    }
}

int main(int argc, char * argv[]) {
	puts(":: START ::");

    FILE *greet = fopen("greet.txt", "w");
    fprintf(greet, "Hello, %s!\n", "name");

    configLogFile();
    printf("Hello logfile");

	int nfds = sd_listen_fds(0); //The 0 parameter indicates that it will listen for file descriptors passed to the service
	if (nfds != 1) {
        error("Zero or more than one file descriptors! Panic.\n");
	}

	int main_fd = SD_LISTEN_FDS_START;

	puts(":: LISTENING ::");

	for (;;) {

		struct sockaddr_un cli_addr;
		unsigned int cli_addr_len = sizeof cli_addr;

		int conn_fd = accept(main_fd, (struct sockaddr *)&cli_addr, &cli_addr_len);
		if (conn_fd < 0) {
			close(main_fd);
			error("ERROR on accept");
		}

		printf("server established connection with client\n");

		pthread_t thread;
		pthread_create(&thread, NULL, process_connection, (void *)(intptr_t)conn_fd);
		pthread_detach(thread);
	}

	close(main_fd);

	puts(":: END ::");
	return 0;
}