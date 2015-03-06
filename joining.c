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

void process_join(int ring, int identifier, int ringport,int bootport,char* bootIP, requestUDP* start)
{
	char msg[128];
	char buffer[128];
	char cmd[10],idIP[20];
	int startid,startTCP,ringx;
	struct hostent *h;
	struct in_addr *a;
	requestUDP* temp = NULL;
	requestTCP* temp2 = NULL;

	sprintf(msg,"BQRY %d\n",ring);
	printf("Command sent: %s\n",msg);
	printf("Socket: %d\n",start->fdUDP);
	temp = createUDP(bootIP,bootport, 1, msg, start->fdUDP);
	printf("Command recieved: %s\n",temp->request);

	if(strcmp(temp->request,"EMPTY") == 0)
	{
		if(gethostname(buffer,128) == -1)
		{
			printf("\nError during hostname query\n\n");
			exit(1);
		}
	  if((h=gethostbyname(buffer))==NULL)
	    {
		    printf("\n%s isn't a valid host name \n",bootIP);
		    exit(1);//error
	    }
		printf("Hostname: %s\n",buffer);
		a = (struct in_addr*)h->h_addr_list[0];
	
	  sprintf(msg,"REG %d %d %s %d\n",ring, identifier,inet_ntoa(*a), ringport);
	  printf("%s\n",msg);
	  
	  temp = createUDP(bootIP, bootport, 1, msg, start->fdUDP);
	  
	  printf("Temp: %s\n",temp->request);
	  
	  if(strcmp(temp->request,"OK") == 0)
	  	return;
	}
	else
	{
		if(sscanf(temp->request,"%s %d %d %s %d",cmd,&ringx,&startid,idIP,&startTCP) != 5)
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
			temp2 = (requestTCP*) TCPconnection(idIP,startTCP);
		}
	}
   
}
