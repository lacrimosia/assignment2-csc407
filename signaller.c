#include	<stdlib.h>
#include	<stdio.h>
#include	<signal.h>
#include	<string.h>
#include 	<unistd.h>

int maxSignalTime;
int shouldContinue = 1;
const int MIN_TIME_IN_SECS = 2;

void sigAlarmHandler(int sig){
  printf("signaller %d signalling parent\n", getpid());
  kill(getppid(), SIGUSR1);
  alarm(rand() % maxSignalTime + 1);
}

void sigIntHandler(int sig){
	shouldContinue = 0;
	printf("signaller %d stopping\n", getpid());
}

int	main(int argc, char *argv[]){
	if(atoi(argv[1]) >= MIN_TIME_IN_SECS){
		maxSignalTime = atoi(argv[1]);
	}else{
		return(EXIT_FAILURE);
	}

	struct sigaction act;
	struct sigaction act2;

	memset(&act, '\0', sizeof(act));

	memset(&act2, '\0', sizeof(act2));

	act.sa_handler = sigAlarmHandler;
	sigaction(SIGALRM,&act,NULL);

	act2.sa_handler = sigIntHandler;
	sigaction(SIGINT,&act2,NULL);

	srand(getpid());

	alarm(rand() % maxSignalTime + 1);

	while(shouldContinue){
   	 sleep(1);
  	}

  	return(EXIT_SUCCESS);
}