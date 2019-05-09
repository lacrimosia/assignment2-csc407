#include	<stdlib.h>
#include	<stdio.h>
#include	<signal.h>
#include	<string.h>
#include 	<unistd.h>
//#include    "signaller.c"

int counter = 0;
int shouldContinue = 1;
int resetTimeInSec;  // read into command line
int signalTimeInSec; // read into command line

// constants
const int MIN_TIME_IN_SECS = 2;

void sigAlarmHandler(int sig){
  counter = 0;
  // TODO
  // look at resetting 
  // resetting to receive SIGALARM resetTimeInSec 
  resetTimeInSec = 0;
  printf("Resetting \n");
}

void sigUSR1Handler(int sig){
	counter += 1;
	printf("Got %d \n", counter);

	if(counter >= 3){
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
	
	
	//TODO
	// correctly installing 2 signal handlers
	// set up the struct
	// install the signal handler
	struct sigaction act;
	memset(&act, '\0', sizeof(act));
	act.sa_handler = sigAlarmHandler;
	sigaction(SIGALRM,&act,NULL);
	//signal(SIGALRM, sigAlarmHandler);
	act.sa_handler = sigUSR1Handler;
	sigaction(SIGUSR1,&act,NULL);
	

	const int BUFFER_LEN = 64;
	char buffer[BUFFER_LEN];

	snprintf(buffer,BUFFER_LEN,"%d",signalTimeInSec);

	// TODO
	// making 3 children processes which use execl to call signaller with 
	// signaltimeInSec

	int processes[3];
	int i=0;

	for(i;i<3;i++) // loop will run n times (n=5) 
    { 
        if(fork() == 0) 
        { 
            //printf("[son] pid %d from [parent] pid %d\n",getpid());  
            exit(0);
        }else{
        	printf("signaller %d signalling parent\n", getpid());
        }
    } 
	//for(i; i<3; i++){
		//fork();
		// if (fork() == 0){
		// 	//child
		// 	execl("signaller.c", buffer);
		// }else{
		// 	processes[0] = getpid();
		// 	printf("signaller %d signalling parent\n", processes[i]);
		// }
		
		// fork();
		// processes[1] = getpid();
		// printf("signaller %d signalling parent\n", processes[i]);

		// fork();
		// processes[2] = getpid();
		// printf("signaller %d signalling parent\n", processes[i]);
	//}
	//printf("[son] pid %d from [parent] pid %d\n",getpid(),getppid()); 
	

	alarm(resetTimeInSec);

	while(shouldContinue){
    	sleep(1);
  	}	

  	// when shouldContinue is 0 or false, send SIGINT to all children
  	int a = 0;
  	for(a; a<3; a++){
  		printf("signaller %d stopping\n", processes[a]);
		kill(processes[a], SIGINT);
	}
  	
  	return(EXIT_SUCCESS);
}