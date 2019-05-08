#include	<stdlib.h>
#include	<stdio.h>
#include	<signal.h>
#include	<string.h>

int maxSignalTime;
int shouldContinue = 1;

int	main(int argc, char *argv[]){
	
	maxSignalTime = atoi(argv[1]);
}