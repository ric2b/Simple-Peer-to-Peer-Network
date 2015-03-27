#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <unistd.h>

#include "interface.h"
#include "ringOps.h"

int check_arguments(int argc, char **argv, char* bootIP, int * bootport, int* ringport, char * externalIP, char * option, int* DEBUG_MODE)
{
	//bootIP and bootport are the IP adress e UDP port of the starting server
	strcpy(bootIP,"tejo.tecnico.ulisboa.pt");
	*bootport = 58000;
	*ringport = 40000;
		//Ringport is the TCP server port used for establishing a TCP session in the ring

	if(argc != 1) // Se o utilizador não forneceu argumentos, usar defaults sem se queixar
	{
		if((argc > 8)  || (argc <= 1))
		{
			printf("\nProgram called with incorrect arguments.\n");
			printf("Invoke the program as the following: ./ddt -t ringport -i bootIP -p bootport\n\n");
			exit(1);
		}
	}

	while((*option = getopt(argc,argv,"t:i:p:e:h:")) != -1) // Checking the various options received on the console application
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

			case 'e':
				strcpy(externalIP, optarg); // se o utilizador quiser usar um IP público
				break;
			case 'h':
				*DEBUG_MODE = atoi(optarg);
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

int run_commands(ringStruct* node, socketStruct socket)
{
	int qryNode,joinargs, myID, ringID,succiID,succiPort;
	char userInput[64], cmd[20], succiIP[20];

	memset(userInput, 0, 64);
	strcpy(cmd,"help"); //default to help

	if(read(0,userInput,63) < 0) // stdin
	{
		printf("Error while reading from \"STDIN\"\n");
		exit(-1);
	}
	printf("[SYSTEM]: ");
	if(sscanf(userInput,"%s",cmd) != 1)
	{
		printf("Error while scanning input from \"STDIN\"\n");
		exit(-1);
	}


	if(strcmp(cmd,"exit") == 0)
	{
		if(node->myID!=-1)
			removeNode(node,socket);
		printf("You have closed the application.\n\n");
		closeSocket(socket);
		exit(0);
	}
	else if(strcmp(cmd,"leave") == 0)
	{
		if(node->myID==-1)
		{
			printf("Node is already not connected to a ring.\n");
			return -1;
		}
		else
			printf("This node (with ID %d) left ring %d.\n", node->myID,node->ringID);
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
		if(sscanf(userInput,"%s %i",cmd,&qryNode) != 2)
		{
			printf("Search format not allowed.\nPlease use : \"search node_number\"\n");
			return -1;
		}
       	searchNode(node,qryNode);
		return -1;
   	}
	else if(strcmp(cmd,"join") == 0)
	{
		joinargs=sscanf(userInput,"%s %i %i %i %s %i",cmd, &ringID, &myID, &succiID, succiIP, &succiPort);
		if(node->myID!=-1)
		{
			printf("Node already connected to a ring.\n");
			return -1;
		}
		else if(joinargs == 3 && myID >-1 && myID < 64 && ringID > 0)
		{
			node->myID = myID;
			node->ringID = ringID;
			printf("Joining ring number %i with an identifier %i.\n", (node->ringID), (node->myID));
			return Join_Ring(node,socket);
		}
		else if(joinargs==6 && ringID > 0 && myID > -1 && myID < 64 && succiID > -1 && succiID < 64 && succiPort > -1)
		{
			node->myID = myID;
			node->ringID = ringID;
			node->starter = 0;
			joinRing_KnownSucci(node,node->succiID,node->succiIP,node->succiPort);
			printf("Joining ring number %i with an identifier %i that has a succi number %i, IP adress %s and TCP number equal to %i.\n", (node->ringID), (node->myID), (node->succiID), node->succiIP, (node->succiPort));
			return -1;
  		}
		else
		{
         		printf("Your join command doesn't have the correct arguments.\n");
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
