#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>

//#include "interface.h"

int check_arguments(int argc, char **argv, char* bootIP, int * bootport, int* ringport, char * option)
{
  	//bootIP and bootport are the IP adress e UDP port of the starting server

  	//Ringport is the TCP server port used for establishing a TCP session in the ring

	if((argc % 2 == 0) || (argc > 7)  || (argc <= 1))
	{
		printf("\nThe program doesn't have enough arguments or it has more arguments than it can handle.\n\n");
		printf("Invoke the program as the following: ./ddt -t ringport -i bootIP -p bootport\n\n");
		exit(1);
	}

	while((*option = getopt(argc,argv,"t:i:p:")) != -1) // Checking the various options received on the console application
	{
		 switch(* option)
		 {
			case 't':
				*ringport = atoi(optarg); // Get the argument corresponding to this case and converts it from the string format to integer type
				break;

			case 'i':
				bootIP = optarg;
				break;

			case 'p':
				*bootport = atoi(optarg);
				break;

			default:
				printf("The expected arguments don't have the proper format. Application Aborted.\n");
				exit(2);
		 }
	}
	return 0;
}

int run_commands(char * userInput, char * cmd, char * succiIP, int * identifier, int * ringx, int* succi, int * succiTCP)
{
   printf("> ");
   fgets(userInput,63,stdin);

   sscanf(userInput,"%s",cmd);

   if(strcmp(cmd,"exit") == 0)
	{
		printf("\nYou have closed the application.\n\n");
		return 1;
   }
	else if(strcmp(cmd,"leave") == 0)
	{
           printf("\nYou removed your node from the current ring.\n\n");
   }
	else if(strcmp(cmd,"show") == 0)
	{
         printf("\nShowing ring number, node identifier and predi/succi identifiers.\n\n");
   }
	else	if(strcmp(cmd,"search") == 0)
	{
       sscanf(userInput,"%s %i",cmd, identifier);
       //corrigir o caso para o qual o utilizador nao insere o argumento

       if(* identifier > -1 && * identifier < 64)
		 {
           printf("\nSearching the identifier and localization of the node responsible for the identifier %i.\n\n", * identifier);
       }
		 else
		 {
           printf("\nYou didn't specify an identifier k or it isn't on the specified interval.\n\n");
    	 }
   }
	else if(strcmp(cmd,"join") == 0)
	{
		if(sscanf(userInput,"%s %i %i %i %s %i",cmd, ringx, identifier, succi, succiIP, succiTCP) ==3)
		{
			printf("Joining ring number %i with an identifier %i.\n", * ringx, * identifier);
		}

		else if(* ringx > 0 && * identifier > -1 && * identifier < 64 && * succi > -1 && * succi < 64 && * succiTCP > -1)
		{
      	printf("\nJoining ring number %i with an identifier %i that has a succi number %i, IP adress %s and TCP number equal to %i.\n\n", * ringx, * identifier, * succi, succiIP, * succiTCP);
  		}
		else
		{
         printf("\nYour joining command doesn't have the correct arguments.\n\n");
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
   }
	else
	{
         printf("\nThe command you have inserted is non existent.\n");
         printf("\nType 'help' to show the available commands.\n\n");
   }
	return 0;
}
