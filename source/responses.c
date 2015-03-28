#include "responses.h"
#include "triggers.h"
#include "network.h"

int FDsocket;

int Message_ID(ringStruct* node,char * request, int senderSocket)
{
	char msg[128], cmd[128];
	int ID;

	if(sscanf(request,"%s %d",cmd,&ID) != 2)
	{
		printf("Bad Message (ID)\n");
		return 1;
	}

	if(strcmp(cmd,"ID") == 0)
	{
		node->NEWfd = senderSocket;
		memset(msg,0,128);
		if((node->succiFD == -1 && node->prediFD == -1) || responsability(node->prediID,node->myID,ID))
		{
			sprintf(msg,"SUCC %d %s %d\n",node->myID, node->myIP, node->myPort);
			sendTCP(msg, senderSocket);
		}
		else
		{
			sprintf(msg,"QRY %d %d\n",node->myID,ID);			
			sendTCP(msg, node->succiFD);
		}
		message_handler(DEBUG_MODE,5,msg,node,0);
		return 0;
	}
	return 1;
}

int Message_NEW(ringStruct* node, char* request, int senderSocket)
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
		if(node->succiID == -1 && node->prediID == -1)
		{
			node->prediID = ID;
			strcpy(node->prediIP,IP);
			node->prediPort = Port;
			node->prediFD = senderSocket;
			tmp = setupSocket(IP,Port,'T');
			node->succiID = ID;
			strcpy(node->succiIP,IP);
			node->succiPort = Port;
			node->succiFD = tmp.socketFD;
			memset(msg,0,128);
			sprintf(msg,"NEW %d %s %d\n", node->myID, node->myIP, node->myPort);
			sendTCP(msg,node->succiFD);
			message_handler(DEBUG_MODE,6,NULL,node,0);
		    return 0;
		}
		else
		{
			if(node->prediID == -1)
			{
				node->prediID = ID;
				strcpy(node->prediIP,IP);
				node->prediPort = Port;
				node->prediFD = senderSocket;
			}
			else
			{
				memset(msg,0,128);
				sprintf(msg,"CON %d %s %d\n",ID, IP, Port);
				sendTCP(msg,node->prediFD);
				close(node->prediFD);
				node->prediID = ID;
				strcpy(node->prediIP,IP);
				node->prediPort = Port;
				node->prediFD = FDsocket;
			}
			message_handler(DEBUG_MODE,7,NULL,node,0);
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
	extern int keepRunning;
	extern socketStruct server;

	//printf("RSP. n=%d\n", n);
	if(sscanf(request,"%s %d %d %d %s %d",cmd,&Master,&ID,&Destination,IP,&Port) != 6)
	{
		printf("Bad Message (RSP)\n");
		return 1;
	}

	if(ID == 70 || ID == 71)
	{
		printf("received broken ring message: %s\n", request);
		
		if(node->prediFD == -1)
		{
			printf("BROKEN RING IS NOW HOPEFULLY FIXED!\n");
			socketStruct tmp = setupSocket(IP, Port, 'T');
			sprintf(msg,"CON %d %s %d\n",node->myID, node->myIP, node->myPort);
			sendTCP(msg, tmp.socketFD);
			close(tmp.socketFD);
			
			if(ID == 70 && node->starter == 0) // The starter node wasn't found during traversal, so it was the failed node 
			{
				sprintf(msg,"REG %d %d %s %d\n",node->ringID, node->myID, node->myIP, node->myPort);
				
				if((sendUDP(msg, server)) == -1)
					exit(1);
				if((recvUDP(msg, server)) == -1)
					exit(1);				

				if(strcmp(msg,"OK") == 0)
					node->starter = 1;
				else
				{
					printf("Bad response from server\n");
					keepRunning = 0;
				}
			}
			
		}
		else
		{
			if(node->starter == 1) // This is the starter node, mark on the message that the starter node is still on the ring
			{
				sprintf(msg,"RSP %d 71 %d %s %d\n",Master, Destination, IP, Port);
				sendTCP(msg,node->prediFD);
			}
			else
				sendTCP(request,node->prediFD);
		}
		return 0;
	}


	if(node->search_status == 0)
	{
		if(node->myID == Master)
		{
			sprintf(msg,"SUCC %d %s %d\n",Destination, IP, Port);
			sendTCP(msg, node->NEWfd);
			message_handler(DEBUG_MODE,8,msg,node,0);
		}
		else
		{
			sendTCP(request,node->prediFD);
		}
	}
	else
	{
		if(node->myID == Master)
		{
				printf("Search Result: %i %s %i\n", Destination, IP, Port);
				node->search_status = 0;
		}
		else
			sendTCP(request,node->prediFD);
	}
	return 0; //REVER
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
		printf("Something went wrong with socket creation\n");
		return 1;
	}
	else
	{
		if(responsability(node->prediID,node->myID,ID))
		{
			memset(msg,0,128);
			sprintf(msg,"RSP %d %d %d %s %d \n",Master, ID,node->myID,node->myIP,node->myPort);
			sendTCP(msg, node->prediFD);
			message_handler(DEBUG_MODE,9,msg,node,0);
		}
		else
		{
			sendTCP(request, node->succiFD);
			message_handler(DEBUG_MODE,10,request,node,0);
		}
		message_handler(DEBUG_MODE,8,msg,node,0);
		return 0;
	 }
	 return 1;
}

int Message_SUCC(ringStruct*node, char* request)
{
	char cmd[128];
	int dest_ID, dest_Port;
	char dest_IP[128];

	if(sscanf(request,"%s %d %s %d",cmd, &dest_ID, dest_IP, &dest_Port) != 4)
	{
		printf("Bad Message (SUCC)\n");
		return 1;
	}
	if(dest_ID == node->myID)
	{
		printf("Can't use identifier %d, please choose a different one and try again\n",node->myID);
        node->myID = -1;
        return 1;
	}
	joinRing_KnownSucci(node, dest_ID, dest_IP, dest_Port);
	message_handler(DEBUG_MODE,8,request,node,0);
	return 0;
}

int Message_CON(ringStruct* node, char* request)
{
	char cmd[128], msg[128];
	int dest_ID, dest_Port;
	char dest_IP[128];

	if(sscanf(request,"%s %d %s %d",cmd, &dest_ID, dest_IP, &dest_Port) != 4)
	{
		printf("Bad Message (CON)\n");
		return 1;
	}
	close(node->succiFD);

	if(dest_ID == node->myID)
	{
		strcpy(node->succiIP,"\0");
    	node->succiPort = -1;
    	node->succiFD = -1;
    	node->succiID = -1;
    	
    	close(node->prediFD);
    	strcpy(node->prediIP,"\0");
    	node->prediPort = -1;
    	node->prediFD = -1;
    	node->prediID = -1;

		printf("This node is now alone on the ring.\n");
		return 0;
	}

	joinRing_KnownSucci(node, dest_ID, dest_IP, dest_Port);

	message_handler(DEBUG_MODE,8,msg,node,0);
	return 0;
}

int Message_BOOT(ringStruct* node, char* request)
{
    node->starter=1;
    return 0;
}

int JR_Message(char* request,ringStruct* node, int nodeFD)
{
	char cmd[128];
	int tmp;
	message_handler(DEBUG_MODE,12,request,node,0);
	FDsocket = nodeFD;
	sscanf(request,"%s",cmd);
	if(strcmp(cmd,"NEW") == 0)
	{
		tmp = Message_NEW(node,request,nodeFD);
		printf("[SYSTEM]: New predi connected.\n");
		return (tmp == 1) ? tmp : 0;
	}
	if(strcmp(cmd,"ID") == 0)
	{
		tmp = Message_ID(node,request,nodeFD);
		printf("[SYSTEM]: Request from node received.\n");		
		return (tmp == 1) ? tmp : 0;
	}
	if(strcmp(cmd,"RSP") == 0)
	{
		tmp = Message_RSP(node,request);
		printf("[SYSTEM]: Responding to Query.\n");
		return (tmp == 1) ? tmp : 0;
	}
	if(strcmp(cmd,"QRY") == 0)
	{
		tmp = Message_QRY(node,request);
		printf("[SYSTEM]: Transmitting query.\n");
		return (tmp == 1) ? tmp : 0;
	}
	if(strcmp(cmd,"SUCC") == 0)
	{
		tmp = Message_SUCC(node,request);
		printf("[SYSTEM]: Node added to the ring.\n");
		return (tmp == 0) ? tmp : 1;
	}
	if(strcmp(cmd,"CON") == 0)
	{
		tmp = Message_CON(node,request);
		printf("[SYSTEM]: Succi Node disconnected.\n");
		return (tmp == 1) ? tmp : 0;
	}
	if(strcmp(cmd,"BOOT") == 0)
	{
		tmp = Message_BOOT(node,request);
		printf("[SYSTEM]: This node is now the master node.\n");
		return (tmp == 0) ? tmp : 1;
	}
	return 1;
}
