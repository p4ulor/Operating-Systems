
#include <systemd/sd-daemon.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>

#include <pthread.h>

#define BUFSIZE 1024

void terror(char *msg) {
	perror(msg);
	pthread_exit((void *)(intptr_t)1);
}

void * process_connection(void * ptr) {
	int conn_fd = (int)(intptr_t)ptr;

	char buf[BUFSIZE];
	int len;

	while (1) {
		len = read(conn_fd, buf, BUFSIZE);
		if (len < 0) {
			terror("ERROR reading from socket");
		}
		buf[len] = 0;
		printf("server received %d bytes: %s\n", len, buf);
/*
		if (len == 0 || (strncmp(buf, "quit", 4) == 0 && buf[4] < ' ')) {
			write(conn_fd,"QUIT?",6);
			close(conn_fd);
			break;
		}

		len = write(conn_fd, buf, strlen(buf)+1);
		if (len < 0) { 
			terror("ERROR writing to socket");
		}
*/
		char isNotValid = 1;
		// buffer size for command has to be ./com(5) plus the space plus the port, so 5+1+4
		if (strncmp(buf, "ADD", 3) == 0 && buf[3] == ' ') {
			strtok(buf," ");
			char *port = strtok(NULL," ");
			char command[100];
			sprintf(command,"/media/sf_TVS/tvs-2223-1-leic52d-g01/se3/ex5/add.sh %s",port);
			system(command);
			isNotValid = 0;
		}
		
		if (strncmp(buf, "DEL", 3) == 0 && buf[3] == ' ') {
			strtok(buf," ");
			char *port = strtok(NULL," ");
			char command[100];
			sprintf(command,"/media/sf_TVS/tvs-2223-1-leic52d-g01/se3/ex5/del.sh %s",port);
			system(command);
			isNotValid = 0;
		}

		if (strncmp(buf, "START", 5) == 0 && buf[5] < ' ') {
			system("/media/sf_TVS/tvs-2223-1-leic52d-g01/se3/ex5/start.sh");
			isNotValid = 0;
		}

		if (strncmp(buf, "STOP", 4) == 0 && buf[4] < ' ') {
			system("/media/sf_TVS/tvs-2223-1-leic52d-g01/se3/ex5/stop.sh");
			isNotValid = 0;
		}

		if (isNotValid) {
			write(conn_fd, "START\t\t- Start's all services.", 31);
			write(conn_fd, "STOP\t\t- Stop's all services.", 29);
			write(conn_fd, "ADD [port]\t\t- Add a new service.", 33);
			write(conn_fd, "DEL [port]\t\t- delete the especified service.", 45);
		}
	}

	return NULL;	
}

void error(char *msg) {
	perror(msg);
	exit(1);
}

int main(int argc, char * argv[]) {
	puts(":: START ::");
	
	int nfds = sd_listen_fds(0);
	if (nfds != 1) {
		fprintf(stderr, "ERROR %d file descriptors found.\n", nfds);
		exit(1);
	}

	puts(":: ACCEPTING ::");

	int main_fd = SD_LISTEN_FDS_START;

	while (1) {

		struct sockaddr_un cli_addr;
		unsigned int cli_addr_len = sizeof cli_addr;

		int conn_fd = accept(main_fd, (struct sockaddr *)&cli_addr, &cli_addr_len);
		if (conn_fd < 0) {
			error("ERROR on accept");
		}

		printf("server established connection with client\n");

		pthread_t thread;
		pthread_create(&thread, NULL, process_connection, (void *)(intptr_t)conn_fd);
		pthread_detach(thread);
	}
	
	puts(":: END ::");
	return 0;
}








