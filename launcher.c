#include	<stdlib.h>
#include	<stdio.h>
#include	<signal.h>
#include	<string.h>

int counter = 0;
int shouldContinue = 1;
int resetTimeInSec;  // read into command line
int signalTimeInSec; // read into command line

// constants
#define MIN_TIME_IN_SECS = 2;

void sigAlarmHandler(int sig){
  counter = 0;
  resetTimeInSec = 0;
}

void sigUSR1Handler(int sig){
	counter += 1;
	if(counter >= 3)
		shouldContinue = 0;
}

int	main(int argc, char *argv[]){

	// set up the struct
	struct sigaction act;
	memset(&act, '\0', sizeof(act));
	act.sa_handler = sigAlarmHandler;
	act.sa_handler = sigUSR1Handler;

	// install the signal handler
	signal(SIGINT, sigAlarmHandler);
	signal(SIGINT, sigUSR1Handler);

	// making 3 child processes
	// put the processes into the array
	int processes[3];
	int i=0;
	for(i; i<3; i++){
		fork();
		processes[i] = getpid();
	}
	//printf("[son] pid %d from [parent] pid %d\n",getpid(),getppid()); 

	const int BUFFER_LEN = 64;
	char buffer[BUFFER_LEN];

	resetTimeInSec = atoi(argv[1]);
	signalTimeInSec = atoi(argv[2]);

	snprintf(buffer,BUFFER_LEN,"%d",signalTimeInSec);

	alarm(resetTimeInSec);

	while  (shouldContinue){
    	sleep(1);
  	}	

  	return(EXIT_SUCCESS);
}