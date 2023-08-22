#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <bits/sigaction.h> //cuz of struct sigaction

#define __USE_POSIX = 1

// gcc 3_signal.c

volatile bool done = false; //volatile tells the compiler to now optimize reads to this variable because it will be used beyong the scope of what the compiler is aware of. In this case, the variable will be accessed by other processes

void onSignalAlarm(int signum) {
	done = true;
}

void onSignalTermination(int signum) { //makes program immune to $ <CTRL + C>
	fprintf(stderr, "Cannot br terminated\n");
}

void onSignalInterrupt(int signum) { //makes program immune to $ kill -TERM <PID>
	fprintf(stderr, "Cannot be interrupted\n");
}

int main() {
	struct sigaction actionAlarm;
	//memset(&actionAlarm, 0, sizeof (struct sigaction)); //not needed
	actionAlarm.sa_handler = onSignalAlarm;

	struct sigaction actionTermination;
	//memset(&actionTermination, 0, sizeof (struct sigaction));
	actionTermination.sa_handler = onSignalTermination;

	struct sigaction actionInterruption;
	//memset(&actionInterruption, 0, sizeof (struct sigaction));
	actionInterruption.sa_handler = onSignalInterrupt;
	
	sigaction(SIGALRM, &actionAlarm, NULL);
	sigaction(SIGTERM, &actionTermination, NULL); //set sigaction w/ the configured sa_handler for the SignalTermination
	sigaction(SIGINT, &actionInterruption, NULL);
	
	alarm(8); //trigger alarm in 8s
	
	printf("Running...\n");
	while (!done) {
		sleep(1);
	}
	printf("END\n");
	
	return 0;
}