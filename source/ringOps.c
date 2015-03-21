#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
//
#include <ifaddrs.h>
//
#include "ringOps.h"
#include "network.h"

int FDsocket;
int ID_ID,ID_Port;
char ID_IP;

int Message_ID(ringStruct* node,char * request)
{
	char msg[128], cmd[128];
	int ID;

	if(sscanf(request,"%s %d",cmd,&ID)!=2)
	{
		printf("Bad Message (ID)\n");
		return 1;
	}

	if(strcmp(cmd,"ID") == 0)
	{
		node->NEWfd = FDsocket;
		memset(msg,0,128);
		if(node->succiID == -1 && node->prediFD == -1)
		{
			sprintf(msg,"SUCC %d %s %d\n",node->myID, node->myIP, node->myPort);
			printf("%s",msg);
			sendTCP(msg, FDsocket);
			printf("1\n");
		}
		else
		{
			sprintf(msg,"QRY %d %d\n",node->myID,ID);
			printf("%s",msg);
			sendTCP(msg, node->succiFD);
			printf("2\n");
		}
		printf("Succi: %d \t Predi: %d\n",node->succiID,node->prediID);
		printf("Succi FD: %d \t Predi FD: %d\n",node->succiFD,node->prediFD);
		printf("Succi TCP: %d \t Predi TCP: %d\n",node->succiPort,node->prediPort);

		return 0;
	}
	return 1;
}

int Message_NEW(ringStruct* node, char* request)
{
	char  cmd[128], msg[128];
	int  Port, ID;
	char IP[128];
	socketStruct tmp;
	if(sscanf(request,"%s %d %s %d",cmd,&ID,IP,&Port) != 4)
	{
		printf("Bad Message (NEW)\n");
		return 1;
	}

	if(strcmp(cmd,"NEW") == 0)
	{
		if(node->succiID == -1 && node->prediFD == -1)
		{
			tmp = setupSocket(IP,Port,'T');
			node->prediID = ID;
			strcpy(node->prediIP,IP);
			node->prediPort = Port;
			node->prediFD = FDsocket;
			node->succiID = ID;
			strcpy(node->succiIP,IP);
			node->succiPort = Port;
			node->succiFD = tmp.socketFD;

			printf("3\n");
			printf("Succi: %d \t Predi: %d\n",node->succiID,node->prediID);
			printf("Succi FD: %d \t Predi FD: %d\n",node->succiFD,node->prediFD);
			printf("Succi TCP: %d \t Predi TCP: %d\n",node->succiPort,node->prediPort);
		    return 0;
		}
		else
		{
			if(node->succiID == -1)
			{
				tmp = setupSocket(IP,Port,'T');
				node->succiID = ID;
				strcpy(node->succiIP,IP);
				node->succiPort = Port;
				node->succiFD = tmp.socketFD;
				printf("3\n");
			    return 0;
			}
			else
			{
				if(node->prediFD == -1)
				{
					node->prediID = ID;
					strcpy(node->prediIP,IP);
					node->prediPort = Port;
					node->prediFD = FDsocket;
					printf("3\n");
				}
				else
				{
					memset(msg,0,128);
					sprintf(msg,"CON %d %s %d",ID, IP, Port);
					sendTCP(msg,node->prediFD);
					close(node->prediFD);
					node->prediID = ID;
					strcpy(node->prediIP,IP);
					node->prediPort = Port;
					node->prediFD = FDsocket;
					memset(msg,0,128);
					sprintf(msg,"CON %d %s %d\n",ID, IP, Port);
					printf("4\n");
				}
			}
			printf("Succi: %d \t Predi: %d\n",node->succiID,node->prediID);
			printf("Succi FD: %d \t Predi FD: %d\n",node->succiFD,node->prediFD);
			printf("Succi TCP: %d \t Predi TCP: %d\n",node->succiPort,node->prediPort);
			return 0;
		}
	}
	return 1;
}

int Message_RSP(ringStruct* node, char* request)
{
	char cmd[128], msg[128];
	int  Port, ID, Master, Destination;
	char IP[128];

	if(sscanf(request,"%s %d %d %d %s %d",cmd,&Master,&ID,&Destination,IP,&Port) != 4)
	{
		printf("Bad Message (NEW)\n");
		return 1;
	}
	sprintf(msg,"SUCC %d %s %d\n",Destination, IP, Port);
	printf("%s\n",msg);
	sendTCP(msg, FDsocket);
	printf("9\n");
	printf("Succi: %d \t Predi: %d\n",node->succiID,node->prediID);
	printf("Succi FD: %d \t Predi FD: %d\n",node->succiFD,node->prediFD);
	printf("Succi TCP: %d \t Predi TCP: %d\n",node->succiPort,node->prediPort);

	return 0;
	// REVER
}

int Message_QRY(ringStruct*node, char* request)
{
	char cmd[128], msg[128];
	int   ID, Master;


	if(sscanf(request,"%s %d %d",cmd, &Master, &ID) != 3)
	{
		printf("Bad Message (QRY)\n");
		return 1;
	}

	if(node->succiFD == -1 && node->prediFD == -1)
	{
		printf("Big Shet I Guess\n");
		return 1;
	}
	else
	{
		if(responsability(node->prediID,node->myID,ID))
		{
			printf("My responsability\n");
			memset(msg,0,128);

			if(node->starter == 1){
				memset(msg,0,128);
				sprintf(msg,"SUCC %d %s %d\n",node->myID, node->myIP, node->myPort);
				printf("Socket %d with %s",node->NEWfd,msg);
				sendTCP(msg, node->NEWfd);
				printf("7\n");

			}
			else
			{
				sprintf(msg,"RSP %d %d %d %s %d \n",Master, ID,node->myID,node->myIP,node->myPort);
				printf("Sending to Master %s",msg);
				sendTCP(msg, node->prediFD);
				printf("8\n");
			}
		}
		else
		{
			printf("Sending to Succi %s",request);
			sendTCP(request, node->succiFD);
			printf("5\n");
		}

		printf("Succi: %d \t Predi: %d\n",node->succiID,node->prediID);
		printf("Succi FD: %d \t Predi FD: %d\n",node->succiFD,node->prediFD);
		printf("Succi TCP: %d \t Predi TCP: %d\n",node->succiPort,node->prediPort);
		return 0;
	 }
	 return 1;
}

int Message_SUCC(ringStruct*node, char* request)
{
	char cmd[128], msg[128];
	int dest_ID, dest_Port;
	char dest_IP[128];
	socketStruct Succi_Node;

	if(sscanf(request,"%s %d %s %d",cmd, &dest_ID, dest_IP, &dest_Port) != 4)
	{
		printf("Bad Message (SUCC)\n");
		return 1;
	}
	while(dest_ID == node->myID)
	{
		printf("Can't use identifier %d, please choose a different one: ",node->myID);
        scanf("%d",&(node->myID));
	}

	Succi_Node = setupSocket(dest_IP,dest_Port,'T');

	node->succiFD = Succi_Node.socketFD;
	node->succiID = dest_ID;
	strcpy(node->succiIP,dest_IP);
	node->succiPort = dest_Port;
	memset(msg,0,128);
	sprintf(msg,"NEW %d %s %d\n",node->myID,node->myIP,node->myPort);
	sendTCP(msg,Succi_Node.socketFD);
	printf("9\n");
	printf("Succi: %d \t Predi: %d\n",node->succiID,node->prediID);
	printf("Succi FD: %d \t Predi FD: %d\n",node->succiFD,node->prediFD);
	printf("Succi TCP: %d \t Predi TCP: %d\n",node->succiPort,node->prediPort);
	return 0;
}

int Message_CON(ringStruct* node, char* request)
{
	char cmd[128], msg[128];
	int dest_ID, dest_Port;
	char dest_IP[128];
	socketStruct tmp;

	if(sscanf(request,"%s %d %s %d",cmd, &dest_ID, dest_IP, &dest_Port) != 4)
	{
		printf("Bad Message (CON)\n");
		return 1;
	}
	memset(msg,0,128);
	sprintf(msg,"NEW %d %s %d\n", node->myID, node->myIP, node->myPort);
	tmp =  setupSocket(dest_IP,dest_Port,'T');
	sendTCP(msg,tmp.socketFD);
	close(node->succiFD);
	node->succiFD = tmp.socketFD;
	node->succiID = dest_ID;
	node->succiPort = dest_Port;
	strcpy(node->succiIP,dest_IP);
	printf("10\n");
	printf("Succi: %d \t Predi: %d\n",node->succiID,node->prediID);
	printf("Succi FD: %d \t Predi FD: %d\n",node->succiFD,node->prediFD);
	printf("Succi TCP: %d \t Predi TCP: %d\n",node->succiPort,node->prediPort);
	return 0;
}

int Message_BOOT(ringStruct* node, char* request)
{
        if(strcmp(request,"BOOT")==0)
                node->starter=1;
        return 0;
}

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

int JR_Message(char* request,ringStruct* node, int nodeFD)
{
	char cmd[128];
	int tmp;
	printf("A analisar: %s\n",request);

	FDsocket = nodeFD;
	sscanf(request,"%s",cmd);
	if(strcmp(cmd,"NEW") == 0)
	{
		tmp = Message_NEW(node,request);
		return (tmp == 1) ? tmp : 0;
	}
	if(strcmp(cmd,"ID") == 0)
	{
		tmp = Message_ID(node,request);
		return (tmp == 1) ? tmp : 0;
	}
	if(strcmp(cmd,"RSP") == 0)
	{
		tmp = Message_RSP(node,request);
		return (tmp == 1) ? tmp : 0;
	}
	if(strcmp(cmd,"QRY") == 0)
	{
		tmp = Message_QRY(node,request);
		return (tmp == 1) ? tmp : 0;
	}
	if(strcmp(cmd,"SUCC") == 0)
	{
		tmp = Message_SUCC(node,request);
		return (tmp == 0) ? tmp : 1;
	}
	if(strcmp(cmd,"CON") == 0)
	{
		tmp = Message_CON(node,request);
		return (tmp == 1) ? tmp : 0;
	}

	return 1;
}

int Join_Ring(ringStruct* node, socketStruct start)
{
	char msg[128],cmd[128];
	char buffer[128];
	char idIP[20];
	int startid,startTCP,ringx;
	int temp;
	socketStruct MasterNode;

	sprintf(msg,"BQRY %d",node->ringID);
	printf("Command sent: %s\n",msg);
	printf("Socket: %d\n",start.socketFD);

	if((temp = sendUDP(msg, start)) == -1)
		exit(1);
	if((temp = recvUDP(buffer,start)) == -1)
		exit(1);

	printf("Command recieved: %s\n",buffer);

	if(strcmp(buffer,"EMPTY") == 0)
	{
	    GetIP(node);
	    sprintf(msg,"REG %d %d %s %d\n",node->ringID, node->myID, node->myIP, node->myPort);
	    printf("%s\n",msg);

	    if((temp = sendUDP(msg,start)) == -1)
	      exit(1);
	    if((temp = recvUDP(buffer,start)) == -1)
	      exit(1);

	    printf("Temp: %s\n",buffer);
	    node->starter = 1;

	    if(strcmp(buffer,"OK") == 0)
	      return -1;
	}

	if(sscanf(buffer,"%s %d %d %s %d",cmd, &ringx,&startid,idIP,&startTCP) != 5)
	{
		printf("Bad Response from Server. Exiting...\n");
		exit(-1);
	}

	while(startid == node->myID)
	{
	    printf("Can't use identifier %d, please choose a different one: ",node->myID);
		scanf("%d",&(node->myID));
	}

	printf("IP: %s\nPort: %d\n",idIP,startTCP);
	sprintf(msg,"ID %d\n",node->myID);
	node->starter = 0;
	MasterNode = setupSocket(idIP,startTCP,'T');
	sendTCP(msg, MasterNode.socketFD);

	return MasterNode.socketFD;
}


//socketStruct setupListenSocket(char * myIP, int myPort)

void removeNode(ringStruct * ringData, socketStruct socketCFG)
{
	char msg[128], buffer[128];
	if(ringData->succiID == -1 && ringData->prediID == -1) // If the node is unique remove the ring from the server
	{
	    sprintf(msg,"UNR %i\n",ringData->ringID);
	    if(sendUDP(msg,socketCFG) == -1)
	        return;
	    if(recvUDP(buffer,socketCFG) == -1)
	        return;
	    else if(strcmp(buffer,"OK")==0)
	    {
            ringData->ringID = -1;
            ringData->myID = -1;
            ringData->starter=0;
            return;
        }
    	else
			return;
	}
	else
	{
		if(ringData->starter == 1) // Test to check if the current node is the starter node. If it is, put the next node as the starter one
		{
		    sprintf(msg,"REG %i %i %s %i\n", ringData->ringID, ringData->succiID, ringData->succiIP, ringData->succiPort);
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
		close(ringData->succiFD); //meter include relativo ao close
		memset(msg,0,strlen(msg));
		sprintf(msg,"CON %i %s %i\n", ringData->succiID, ringData->succiIP, ringData->succiPort);
		sendTCP(msg, ringData->prediFD);
		nodeReset(ringData);
		return;
	}
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

void searchNode(ringStruct * ringData, int k)
{ // returns 0 if everything went as expected
  	char msg[128];
	if(k>=0 && k<64)
	{
		if(responsability(ringData->prediID,ringData->myID,k) == 1)
		{
			printf("%i %s %i", ringData->succiID, ringData->succiIP, ringData->succiPort);
			return;
		}
		else
		{
			sprintf(msg,"QRY %i %i\n", ringData->myID, k);
			sendTCP(msg, ringData->succiFD);
			return;
		}
	}
	else
	{
		printf("The node you are searching for is not in the 0 to 63 interval!\n");
	}
}

void showNode(ringStruct * ringData)
{
	printf("ring: %i myID: %i succi: %i predi:%i\n", ringData->ringID, ringData->myID, ringData->succiID, ringData->prediID);
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
