#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>

#include "network.h"

void process_join(int ring, int identifier, int ringport, socketStruct start)
{
	char msg[128];
	char buffer[128];
	char localmachine[128];
	char cmd[10],idIP[20];
	int startid,startTCP,ringx;
	struct hostent *h;
	struct in_addr *a;
	int temp;
	socketStruct PeerTCP;


	sprintf(msg,"BQRY %d\n",ring);
	printf("Command sent: %s\n",msg);
	printf("Socket: %d\n",start.socketFD);
	if((temp = sendUDP(msg,strlen(msg),start)) == -1)
		exit(1);
	if((temp = recvUDP(buffer,start)) == -1)
		exit(1);
	
	printf("Command recieved: %s\n",buffer);

	if(strcmp(buffer,"EMPTY") == 0)
	{
		if(gethostname(localmachine,128) == -1)
		{
			printf("\nError during hostname query\n\n");
			exit(1);
		}
	  if((h=gethostbyname(localmachine))==NULL)
	    {
		    
		    exit(1);//error
	    }
		printf("Hostname: %s\n",localmachine);
		a = (struct in_addr*)h->h_addr_list[0];
	
	  sprintf(msg,"REG %d %d %s %d\n",ring, identifier,inet_ntoa(*a), ringport);
	  printf("%s\n",msg);
	  
	  if((temp = sendUDP(msg,strlen(msg),start)) == -1)
		exit(1);
	  if((temp = recvUDP(buffer,start)) == -1)
		exit(1);

	  printf("Temp: %s\n",buffer);
	  
	  if(strcmp(buffer,"OK") == 0)
	  	return;
	}
	else
	{
		if(sscanf(buffer,"%s %d %d %s %d",cmd,&ringx,&startid,idIP,&startTCP) != 5)
		{
			printf("Bad Response from start server\n");
			exit(1);
		}
		else
		{
			if(startid == identifier)
			{
				printf("Can't use identifier %d, please choose a different one: ",identifier);
				scanf("%d",&identifier);
			}
			PeerTCP = setupSocket(idIP, startTCP, 'T');
		}
	}
   
}
