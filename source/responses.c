#include "responses.h"

int FDsocket;

int Message_ID(ringStruct* node,char * request, int senderSocket)
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
		node->NEWfd = senderSocket;
		memset(msg,0,128);
		if((node->succiFD == -1 && node->prediFD == -1) || responsability(node->prediID,node->myID,ID))
		{
			sprintf(msg,"SUCC %d %s %d\n",node->myID, node->myIP, node->myPort);
			printf("%s",msg);
			sendTCP(msg, senderSocket);
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
		if(node->succiID == -1 && node->prediFD == -1)
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

			printf("3\n");
			printf("Succi: %d \t Predi: %d\n",node->succiID,node->prediID);
			printf("Succi FD: %d \t Predi FD: %d\n",node->succiFD,node->prediFD);
			printf("Succi TCP: %d \t Predi TCP: %d\n",node->succiPort,node->prediPort);
		    return 0;
		}
		else
		{
			if(node->prediFD == -1)
			{
				node->prediID = ID;
				strcpy(node->prediIP,IP);
				node->prediPort = Port;
				node->prediFD = senderSocket;
				printf("3\n");
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
				printf("4\n");
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

	if(sscanf(request,"%s %d %d %d %s %d",cmd,&Master,&ID,&Destination,IP,&Port) != 6)
	{
		printf("Bad Message (RSP)\n");
		return 1;
	}
	
	if(node->search_status == 0)
	{
		if(Master == node->myID)
		{
			sprintf(msg,"SUCC %d %s %d\n",Destination, IP, Port);
			printf("%s\n",msg);
			sendTCP(msg, node->NEWfd);
			printf("9\n");
			printf("Succi: %d \t Predi: %d\n",node->succiID,node->prediID);
			printf("Succi FD: %d \t Predi FD: %d\n",node->succiFD,node->prediFD);
			printf("Succi TCP: %d \t Predi TCP: %d\n",node->succiPort,node->prediPort);
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
			sprintf(msg,"RSP %d %d %d %s %d \n",Master, ID,node->myID,node->myIP,node->myPort);
			printf("Sending to Master %s",msg);
			sendTCP(msg, node->prediFD);
			printf("8\n");
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
		printf("Can't use identifier %d, please choose a different one and try again\n",node->myID);
        node->myID = -1;
        return 1;
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
    node->starter=1;
    return 0;
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
		tmp = Message_NEW(node,request,nodeFD);
		return (tmp == 1) ? tmp : 0;
	}
	if(strcmp(cmd,"ID") == 0)
	{
		tmp = Message_ID(node,request,nodeFD);
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
	if(strcmp(cmd,"BOOT") == 0)
	{
		tmp = Message_BOOT(node,request);
		return (tmp == 0) ? tmp : 1;
	}
	return 1;
}
