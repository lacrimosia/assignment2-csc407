#include	<stdlib.h>
#include	<stdio.h>
#include	<signal.h>
#include	<string.h>
#include 	<unistd.h>

int maxSignalTime;
int shouldContinue = 1;

void sigAlarmHandler(int sig){
  // try
  kill(getppid(), SIGUSR1);
  alarm(rand() % maxSignalTime + 1);
}

void sigUSR1Handler(int sig){
	printf("Signal %d \n", getpid());
	shouldContinue = 0;
}

int	main(int argc, char *argv[]){

	// TODO
	// check to see if we need to take in the 2 parameters from launcher
	if(argc != 2){
		printf("Usage: signaller <maxSignalTime> \n");
		return(EXIT_FAILURE);
	}

	if(atoi(argv[1]) >= MIN_TIME_IN_SECS){
		maxSignalTime = atoi(argv[1]);
	}
	

	// TODO
	// two signal handlers, one for SIGAlarm/Sigusr1, other for SIGINT

	struct sigaction act;
	memset(&act, '\0', sizeof(act));
	act.sa_handler = sigAlarmHandler;
	sigaction(SIGALRM,&act,NULL);
	act.sa_handler = sigUSR1Handler;
	sigaction(SIGUSR1,&act,NULL);

	srand(getpid());

	alarm(rand() % maxSignalTime + 1);

	while(shouldContinue){
   	 sleep(1);
  	}

  	return(EXIT_SUCCESS);
}