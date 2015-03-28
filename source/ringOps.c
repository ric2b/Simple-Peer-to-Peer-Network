#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
//
#include <ifaddrs.h>
//
#include "structs.h"
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
    node->search_status = 0;
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
	node->search_status = 0;
	strcpy(node->externalIP,"\0");
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
  message_handler(DEBUG_MODE,11,NULL,NULL,res);
  return res;
}

int responsability(int predi, int i, int k)
{ // returns 1 if i is responsible for k, 0 otherwise.
  if(distance(k,i) < distance(k,predi))
    return 1;
  else
    return 0;
}

void print_ring_query(ringStruct* node)
{
    int valores[64];
    char msg[128],buffer[128];
    char cmd[128], IP[128];
    int Master,ID,Destination,Port;
    int i = 0;
    int dest = node->succiID;

    for(i=0;i<64;i++)
        valores[i] = -1;

    valores[node->myID] = node->myID;

    if(node->succiID == node->myID && node->prediID == node->myID)
        valores[node->myID] = node->myID;
    else
    {
        for(i=0;i<64;i++)
        {
            printf("%d\n",i);
            memset(msg,0,128);
            memset(buffer,0,128);
            sprintf(msg,"QRY %d %d\n",node->myID,dest);
            printf("%s",msg);
            sendTCP(msg,node->succiFD);
            read(node->succiFD,buffer,128);

            if(sscanf(buffer,"%s %d %d %d %s %d",cmd,&Master,&ID,&Destination,IP,&Port) != 6)
            {
                printf("Bad Message (RSP - in print ring)\n");
                return;
            }
            if(ID == Destination)
            {    
                valores[ID] = ID;
                dest++;
            }
            else
                dest = Destination;

            if(Destination == node->prediID)
            {
                valores[Destination] = Destination;    
                break;
            }
        }
    }
    for(i=0;i<64;i++)
        printf("Valores %d: %d\n",i,valores[i]);

    print_ring(valores);

    return;
}
