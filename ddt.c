#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "interface.h"
#include "network.h"

int main(int argc, char **argv)
{
	char *bootIP;
	int	bootport, ringport;
	char 	userInput[64], cmd[20],succiIP[70];
	char	option;
	int 	exitProgram, identifier, ringx, succi, succiTCP;
	requestUDP* startUDP;
	
	bootIP = (char*) malloc(128*sizeof(char));

	bootIP = (char*) check_arguments(argc, argv, bootIP, & bootport, & ringport, & option);

	printf("\nWelcome to your favorite p2p client! You have chosen the following specifications: \n\n");
	
	printf("\tSelected ringport: %i\n",ringport);
	printf("\tSelected bootIP: %s\n",bootIP);
	printf("\tSelected bootport: %i\n\n",bootport);

	printf("Type 'help' to show the available commands.\n\n");
	
	startUDP = createUDP(bootIP, bootport, 0, NULL,0);

	do
	{
		ringx = 0;
   		identifier = -1;
   		succi = -1;
   		succiTCP = -1;
		memset(succiIP,0,70);	
	} while(0 == run_commands(userInput, cmd, succiIP, & exitProgram, & identifier, & ringx, & succi, & succiTCP, bootport, bootIP, ringport, startUDP));	

  	exit(0);
}
