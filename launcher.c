#include	<stdlib.h>
#include	<stdio.h>
#include	<signal.h>
#include	<string.h>
#include 	<unistd.h>

int counter = 0;
int shouldContinue = 1;
int resetTimeInSec; 
int signalTimeInSec;
const int MIN_TIME_IN_SECS = 2;

void sigAlarmHandler(int sig){
  counter = 0;
  alarm(resetTimeInSec);
  printf("Resetting \n");
}

void sigUSR1Handler(int sig){
	counter += 1;
	if(counter < 3){
		printf("Got %d \n", counter);
	}else if(counter >= 3){
		shouldContinue = 0;
		printf("Got %d Have reached the limit!\n", counter);
	}
}

int	main(int argc, char *argv[]){

	if(argc != 3){
		printf("Usage: launcher <resetTime> <signalTime>\n");
		return(EXIT_FAILURE);
	}

	if(atoi(argv[1]) >= MIN_TIME_IN_SECS){
	 	resetTimeInSec = atoi(argv[1]);
	}else{
		printf("resetTime must be %d or greater.\n", MIN_TIME_IN_SECS);
		return(EXIT_FAILURE);
	}

	if(atoi(argv[2]) >= MIN_TIME_IN_SECS){
	 	signalTimeInSec = atoi(argv[2]);
	}else{
		printf("signalTimeInSec must be %d or greater.\n", MIN_TIME_IN_SECS);
		return(EXIT_FAILURE);
	}
	
	struct sigaction act;
	memset(&act, '\0', sizeof(act));

	struct sigaction act2;
	memset(&act2, '\0', sizeof(act2));
	
	act.sa_handler = sigAlarmHandler;
	sigaction(SIGALRM,&act,NULL);
	act2.sa_handler = sigUSR1Handler;
	sigaction(SIGUSR1,&act2,NULL);
	

	const int BUFFER_LEN = 64;
	char buffer[BUFFER_LEN];

	snprintf(buffer,BUFFER_LEN,"%d",signalTimeInSec);

	int numChildren = 3;
	int processes[numChildren];
	int i;
	for(i=0;i<numChildren;i++){
		processes[i] = fork();

		if(processes[i] < 0){
			exit(EXIT_FAILURE);
		}else if(processes[i] > 0){
			// parent
		}else{
			// child
			execl("./signaller","signaller",buffer,NULL);
			exit(EXIT_SUCCESS);
		}
	}

	alarm(resetTimeInSec);
	while(shouldContinue){
    	sleep(1);
  	}

  	int a;
	for(a=0;a<numChildren;a++){
		int status;
		// Send SIGINT
		kill(processes[a], SIGINT);
		wait(&status);
	}
  	return(EXIT_SUCCESS);
}