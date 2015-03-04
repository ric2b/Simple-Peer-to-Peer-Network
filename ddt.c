#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "configuration.h"

int main(int argc, char **argv)
{

	char * bootIP;
	int	bootport, ringport, option;
	char userInput[64], cmd[20],succiIP[70];
	int exitProgram,identifier,ringx,succi,succiTCP;

	//By default, bootIP="tejo.ist.utl.pt" and bootport=58000
	bootIP="tejo.ist.utl.pt\0";
	bootport=58000;

	check_arguments(argc, argv, bootIP, &bootport, &ringport, &option);

	printf("\n");

	printf("Welcome to your favorite p2p client! You have chosen the following specifications: \n");

	printf("\n");

	printf("\tSelected ringport: %i\n",ringport);
	printf("\tSelected bootIP: %s\n",bootIP);
	printf("\tSelected bootport: %i\n",bootport);

	printf("\n");

	printf("Type 'help' to show the available commands.\n");

	printf("\n");

	exitProgram = 0;

	while(exitProgram == 0)
	{
      ringx = 0;
      identifier = -1;
      succi = -1;
      succiTCP = -1;
      printf(">");
      fgets(userInput,63,stdin);

      sscanf(userInput,"%s",cmd);

      if(strcmp(cmd,"exit") == 0)
		{
        exitProgram = 1;
        printf("\n");
        printf("You have closed the application.\n");
        printf("\n");
      }
		else if(strcmp(cmd,"leave") == 0)
		{
              printf("\n");
              printf("You removed your node from the current ring.\n");
              printf("\n");
      }
		else if(strcmp(cmd,"show") == 0)
		{
            printf("\n");
            printf("Showing ring number, node identifier and predi/succi identifiers.\n");
            printf("\n");
      }
		else	if(strcmp(cmd,"search") == 0)
		{
          sscanf(userInput,"%s %i",cmd,&identifier);
          //corrigir o caso para o qual o utilizador nao insere o argumento

          if(identifier > -1 && identifier < 64)
			 {
              printf("\n");
              printf("Searching the identifier and localization of the node responsible for the identifier %i.\n",identifier);
              printf("\n");
          }
			 else
			 {
              printf("\n");
              printf("You didn't specify an identifier k or it isn't on the specified interval.\n");
              printf("\n");
       	 }
      }
		else if(strcmp(cmd,"join") == 0)
  		{
   		if(sscanf(userInput,"%s %i %i %i %s %i",cmd,&ringx,&identifier,&succi,succiIP,&succiTCP) ==3)
			{
				printf("Joining ring number %i with an identifier %i.\n", ringx, identifier);
			}
     		
			else if(ringx > 0 && identifier > -1 && identifier < 64 && succi > -1 && succi < 64 && succiTCP > -1)
			{
         	printf("\n");
         	printf("Joining ring number %i with an identifier %i that has a succi number %i, IP adress %s and TCP number equal to %i.\n",ringx,identifier,succi,succiIP,succiTCP);
         	printf("\n");
     		}
			else
			{
            printf("\n");
            printf("Your joining command doesn't have the correct arguments.\n");
            printf("\n");
     		}
      }
		else if(strcmp(cmd,"help") == 0)
		{
            printf("\n");
            printf("> join x i\n");
            printf("> join x i succi succi.IP succ i.TCP\n");
            printf("> leave\n");
            printf("> show \n");
            printf("> search k\n");
            printf("> exit\n");
            printf("\n");
      }
		else
		{
            printf("\n");
            printf("The command you have inserted is non existent.\n");
            printf("\n");
            printf("Type 'help' to show the available commands.\n");
            printf("\n");
      }			             
   }
  	exit(0);
}
