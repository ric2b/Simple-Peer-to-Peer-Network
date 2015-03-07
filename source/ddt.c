#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "interface.h"
#include "network.h"

int main(int argc, char **argv)
{
	char * bootIP;
	int	bootport, ringport;
	char 	userInput[64], cmd[20],succiIP[70], buffer[128];
	char	option;
	int 	exitProgram, identifier, ringx, succi, succiTCP;

	//By default, bootIP="tejo.ist.utl.pt" and bootport=58000
	bootIP = "tejo.ist.utl.pt";
	bootport = 58000;

	check_arguments(argc, argv, bootIP, & bootport, & ringport, & option);

	printf("\nWelcome to your favorite p2p client! You have chosen the following specifications: \n\n");

	printf("\tSelected ringport: %i\n",ringport);
	printf("\tSelected bootIP: %s\n",bootIP);
	printf("\tSelected bootport: %i\n\n",bootport);

	printf("Type 'help' to show the available commands.\n\n");

	socketUDP socketCFG;
	socketCFG = setupSocket(bootIP, bootport);

	sendUDP("BQRY 2\n",7,socketCFG);
	recvUDP(buffer, socketCFG);
	printf("%s\n", buffer);	

	do
	{
		ringx = 0;
   		identifier = -1;
   		succi = -1;
   		succiTCP = -1;
		memset(succiIP,0,70);
	} while(0 == run_commands(userInput, cmd, succiIP, & exitProgram, & identifier, & ringx, & succi, & succiTCP));

	close(socketCFG.socketFD);
  	exit(0);
}
