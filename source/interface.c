#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <unistd.h>


//#include "network.h"
#include "interface.h"
#include "ringOps.h"

int check_arguments(int argc, char **argv, char* bootIP, int * bootport, int* ringport, char * option)
{
	//bootIP and bootport are the IP adress e UDP port of the starting server
	strcpy(bootIP,"tejo.tecnico.ulisboa.pt");
	*bootport = 58000;
	*ringport = 40000;
		//Ringport is the TCP server port used for establishing a TCP session in the ring

	if(argc != 1) // Se o utilizador não forneceu argumentos, usar defaults sem se queixar
	{
		if((argc % 2 == 0) || (argc > 7)  || (argc <= 1))
		{
			printf("\nThe program doesn't have enough arguments or it has more arguments than it can handle.\n\n");
			printf("Invoke the program as the following: ./ddt -t ringport -i bootIP -p bootport\n\n");
			exit(1);
		}
	}

	while((*option = getopt(argc,argv,"t:i:p:")) != -1) // Checking the various options received on the console application
	{
		 switch(* option)
		 {
			case 't':
				*ringport = atoi(optarg); // Get the argument corresponding to this case and converts it from the string format to integer type
				break;

			case 'i':
				strcpy(bootIP, optarg);
				break;

			case 'p':
				*bootport = atoi(optarg);
				break;

			default:
				printf("The expected arguments don't have the proper format. Application Aborted.\n");
				exit(2);
		 }
	}

	printf("\nWelcome to your favorite p2p client! You have chosen the following specifications: \n\n");
	printf("\tSelected ringport: %i\n",*ringport);
	printf("\tSelected bootIP: %s\n",bootIP);
	printf("\tSelected bootport: %i\n\n", *bootport);
	printf("Type 'help' to show the available commands.\n\n");

	return 0;
}

int run_commands(ringStruct* node, socketStruct socket, ringStruct * ringData)
{
	int qryNode;
	char userInput[64], cmd[20];

	memset(userInput, 0, 64);
	strcpy(cmd,"help"); //default to help
	read(0,userInput,63); // stdin
	printf("[SYSTEM]: ");
	sscanf(userInput,"%s",cmd);

	if(strcmp(cmd,"exit") == 0)
	{
		printf("You have closed the application.\n\n");
		exit(0);
	}
	else if(strcmp(cmd,"leave") == 0)
	{
		printf("Left ring %d\n", ringData->ringID);
		removeNode(node,socket);
		return -1;
	}
	else if(strcmp(cmd,"show") == 0)
	{
		showNode(node);
		//printf("Showing ring number, node identifier and predi/succi identifiers.\n\n");
		return -1;
	}
	else if(strcmp(cmd,"search") == 0)
	{
		sscanf(userInput,"%s %i",cmd,&qryNode);
       	searchNode(node,qryNode);
		return -1;
   	}
	else if(strcmp(cmd,"join") == 0)
	{
		if(sscanf(userInput,"%s %i %i %i %s %i",cmd, &(node->ringID), &(node->myID), &(node->succiID), node->succiIP, &(node->succiPort)) ==3 && node->myID >-1 && node->myID < 64)
		{
			printf("Joining ring number %i with an identifier %i.\n", (node->ringID), (node->myID));
			return Join_Ring(node, socket);
		}

		else if((node->ringID) > 0 && (node->myID) > -1 && (node->myID) < 64 && (node->succiID) > -1 && (node->succiID) < 64 && (node->succiPort) > -1)
		{
			joinRing_KnownSucci(ringData, node->succiID, node->succiIP, node->succiPort);
			printf("Joining ring number %i with an identifier %i that has a succi number %i, IP adress %s and TCP number equal to %i.\n\n", (node->ringID), (node->myID), (node->succiID), node->succiIP, (node->succiPort));
			return -1;
  		}
		else
		{
         		printf("Your joining command doesn't have the correct arguments.\n\n");
         		nodeReset(node);
			return -1;
  		}
	}
	else if(strcmp(cmd,"help") == 0)
	{
         printf("\n\t- join [x] [i]\n");
         printf("\t- join [x] [i] [succi] [succi.IP] [succi.TCP]\n");
         printf("\t- leave\n");
         printf("\t- show \n");
         printf("\t- search [k]\n");
         printf("\t- exit\n\n");
		 return -1;
   	}
	else
	{
         printf("The command you have inserted is non existent.\n");
         printf("Type 'help' to show the available commands.\n\n");
		 return -1;
   	}
}
