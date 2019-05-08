#include	<stdlib.h>
#include	<stdio.h>
#include	<signal.h>
#include	<string.h>
#include 	<unistd.h>

int maxSignalTime;
int shouldContinue = 1;

void sigAlarmHandler(int sig){
  alarm(rand() % maxSignalTime + 1);
}

void sigUSR1Handler(int sig){
	printf("Signal %d \n", getpid());
	shouldContinue = 0;
}

int	main(int argc, char *argv[]){
	
	maxSignalTime = atoi(argv[1]);

	// set up the struct
	// install the signal handler
	struct sigaction act;
	memset(&act, '\0', sizeof(act));
	act.sa_handler = sigAlarmHandler;
	signal(SIGINT, sigAlarmHandler);
	act.sa_handler = sigUSR1Handler;
	signal(SIGINT, sigUSR1Handler);

	srand(getpid());

	alarm(rand() % maxSignalTime + 1);

	while(shouldContinue){
   	 sleep(1);
  	}

  	return(EXIT_SUCCESS);
}