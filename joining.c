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
	struct hostent *h;
	struct in_addr *a;
	requestUDP* temp;

	sprintf(msg,"BQRY %d",ring);

	temp = createUDP(bootIP,bootport, 1, msg, start->fdUDP);
	if(strcmp(temp->request,"EMPTY") == 0)
	{
		if(gethostname(buffer,128) == -1)
		{
			printf("\nError during hostname query\n\n");
			exit(1);
		}
	  if((h=gethostbyname(buffer))==NULL) //substituir por bootIP
	    {
		    printf("\n%s isn't a valid host name \n",bootIP);
		    exit(0);//error
		  }
		printf("Hostname: %s\n",buffer);
		a = (struct in_addr*)h->h_addr_list[0];
	
	  sprintf(msg,"REG %d %d %s %d\n",ring, identifier,inet_ntoa(*a), ringport);
	  printf("%s\n",msg);
	  temp = createUDP(bootIP, bootport, 1, msg, start->fdUDP);
	}
}
