#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
//
#include <ifaddrs.h>
//
#include "ringOps.h"
#include "network.h"
#include "triggers.h"
#include "responses.h"

int Join_Ring(ringStruct* node, socketStruct start)
{
	char msg[128],cmd[128];
	char buffer[128];
	char idIP[20];
	int startid,startTCP,ringx;
	int temp;
	socketStruct MasterNode;

	sprintf(msg,"BQRY %d",node->ringID);
	//printf("Command sent: %s\n",msg);
	//printf("Socket: %d\n",start.socketFD);

	if((temp = sendUDP(msg, start)) == -1)
		exit(1);
	if((temp = recvUDP(buffer,start)) == -1)
		exit(1);

	//printf("Command recieved: %s\n",buffer);

	if(strcmp(buffer,"EMPTY") == 0)
	{
	    GetIP(node);
	    sprintf(msg,"REG %d %d %s %d\n",node->ringID, node->myID, node->myIP, node->myPort);
	    if((temp = sendUDP(msg,start)) == -1)
	      exit(1);
	    if((temp = recvUDP(buffer,start)) == -1)
	      exit(1);

	    //printf("Temp: %s\n",buffer);
	    node->starter = 1;

	    if(strcmp(buffer,"OK") == 0)
	      return -1;
	}

	if(sscanf(buffer,"%s %d %d %s %d",cmd, &ringx,&startid,idIP,&startTCP) != 5)
	{
		printf("Bad Response from Server. Exiting...\n");
		exit(-1);
	}

	if(startid == node->myID)
	{
	    printf("Can't use identifier %d, please choose a different one and try again\n",node->myID);
		node->myID = -1;
		return 1;
	}

	//printf("IP: %s\nPort: %d\n",idIP,startTCP);
	sprintf(msg,"ID %d\n",node->myID);
	node->starter = 0;
	MasterNode = setupSocket(idIP,startTCP,'T');
	sendTCP(msg, MasterNode.socketFD);

	return MasterNode.socketFD;
}

void searchNode(ringStruct * ringData, int k)
{ // returns 0 if everything went as expected
  	char msg[128];
  	extern int startTimer;

  	if(startTimer != 0)
  	{
  		sprintf(msg,"QRY %i %i\n", ringData->myID, k);
		sendTCP(msg, ringData->succiFD);
		ringData->search_status = 1;
		return;
  	}

	if(k>=0 && k<64)
	{
		if(ringData->myID ==-1)
		{
			printf("You are not in a ring yet. Join a ring first if you want to search a node.\n");
			return;
		}
		else if(ringData->succiID == -1 && ringData->prediID == -1)
		{
			printf("Search Result: %i %s %i\n", ringData->myID, ringData->myIP, ringData->myPort);
			return;
		}
		else if(responsability(ringData->prediID,ringData->myID,k) == 1)
		{
			printf("Search Result: %i %s %i\n", ringData->myID, ringData->myIP, ringData->myPort);
			return;
		}
		else
		{
			sprintf(msg,"QRY %i %i\n", ringData->myID, k);
			Message_QRY(ringData, msg);
			ringData->search_status = 1;
			return;
		}
	}
	else
	{
		printf("The node you are searching for is not in the 0 to 63 interval!\n");
	}
}

void removeNode(ringStruct * ringData, socketStruct socketCFG)
{
	char msg[128], buffer[128];

	if(ringData->succiID == -1 && ringData->prediID == -1) // If the node is unique remove the ring from the server
		{
		    sprintf(msg,"UNR %d\n",ringData->ringID);
		    if(sendUDP(msg,socketCFG) == -1)
		        return;
		    if(recvUDP(buffer,socketCFG) == -1)
		        return;
	    	else if(strcmp(buffer,"OK")==0)
		    {
	            ringData->ringID = -1;
	            ringData->myID = -1;
	            ringData->starter=0;
				printf("Your node left the ring.\n");
	            return;
	        }
		}
	else
	{
		if(ringData->starter == 1) // Test to check if the current node is the starter node. If it is, put the next node as the starter one
		{
		    sprintf(msg,"REG %d %d %s %d\n", ringData->ringID, ringData->succiID, ringData->succiIP, ringData->succiPort);
		    if(sendUDP(msg,socketCFG) == -1)
		        return;
		    if(recvUDP(buffer,socketCFG) == -1)
		        return;
			else if(strcmp(buffer,"OK") == 0)
		    {
	            memset(msg,0,strlen(msg));
	            sprintf(msg,"BOOT\n");
	            sendTCP(msg, ringData->succiFD);
	            ringData->starter=0;
		    }
		}
		close(ringData->succiFD);
		if(ringData->prediID != ringData->succiID) // há mais que 2 nós
		{
			memset(msg,0,128);
			sprintf(msg,"CON %d %s %d\n", ringData->succiID, ringData->succiIP, ringData->succiPort);
			sendTCP(msg, ringData->prediFD);
		}
		close(ringData->prediFD);
		nodeReset(ringData);
		return;
	}
}

void showNode(ringStruct * ringData)
{
	if(ringData->ringID==-1)
	{
		printf("Not connected to a ring.\n");
	}
	else
	{
		printf("ring: %i predi:", ringData->ringID);
		(ringData->prediID == -1) ? printf("None ") : printf("%i ", ringData->prediID);
		printf("myID:");
		(ringData->myID == -1) ? printf("None ") : printf("%i ", ringData->myID);
		printf("succi:");
		(ringData->succiID == -1) ? printf("None.") : printf("%i.", ringData->succiID);
		if(ringData->starter == 1) printf(" Your node is the starter node of the ring.");
		printf("\n");
	}
	return;
}

int joinRing_KnownSucci(ringStruct * ringData, int succiID, char * succiIP, int succiPort)
{
	ringData->succiID = succiID;
  	strcpy(ringData->succiIP, succiIP);
  	ringData->succiPort = succiPort;
	ringData->succiFD = setupSocket(succiIP, succiPort, 'T').socketFD;
	
	char joinCommand[128*2]; //para aguentar com os 128 do IP + extras
	sprintf(joinCommand, "NEW %d %s %d", ringData->myID, ringData->myIP, ringData->myPort);
  	sendTCP(joinCommand, ringData->succiFD);
  	return 0;
}
