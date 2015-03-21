#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
//
#include <ifaddrs.h>
//
#include "ringOps.h"
#include "network.h"


void nodeReset(ringStruct * node)
{
    node->ringID = -1;
    node->myID = -1;
    node->succiID = -1;
    strcpy(node->succiIP,"\0");
    node->succiPort = -1;
    node->succiFD = -1;
     node->prediID = -1;
    strcpy(node->prediIP,"\0");
    node->prediPort = -1;
    node->prediFD = -1;
}

void GetIP(ringStruct* node)
{
	char localmachine[128];
	struct hostent *h;
	struct in_addr *a;

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
	strcpy(node->myIP,inet_ntoa(*a));
}

void Node_Initialization(ringStruct* node)
{
	node->ringID = -1;
	node->myID = -1;
	strcpy(node->myIP,"\0");
	node->myPort = -1;
	node->succiID = -1;
	strcpy(node->succiIP,"\0");
	node->succiPort = -1;
	node->succiFD = -1;
	node->prediID = -1;
	strcpy(node->prediIP,"\0");
	node->prediPort = -1;
	node->prediFD = -1;
}

int distance(int k, int l)
{ // if it returns -1 something wrong occured
  int res;
  if(l >= k)
    res = l-k;
  else if(l < k)
    res = 64 + l - k;
  else
    res = -1;
  printf("DISTANCE: %d\n", res);
  return res;
}

int responsability(int predi, int i, int k)
{ // returns 1 if i is responsible for k, 0 otherwise.
  if(distance(k,i) < distance(k,predi))
    return 1;
  else
    return 0;
}
