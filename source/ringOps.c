#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
//
#include <ifaddrs.h>
//
#include "ringOps.h"
#include "network.h"

int FDsocket, Port, ID, Destination, No_Novo;
char IP[128];


void Message_ID(ringStruct* node,char * request)
{
	char msg[128], cmd[128];

	if(strcmp(cmd,"ID") == 0)
	{
		if(node->succiID == -1 && node->prediFD == -1)
		{
			node->NEWfd = FDsocket;
			memset(msg,0,128);
			sprintf(msg,"SUCC %d %s %d\n",node->myID, node->myIP, node->myPort);
			printf("%s",msg);
			sendTCP(msg, FDsocket);
			printf("1\n");
			printf("Succi: %d \t Predi: %d\n",node->succiID,node->prediID);
			printf("Succi FD: %d \t Predi FD: %d\n",node->succiFD,node->prediFD);
			printf("Succi TCP: %d \t Predi TCP: %d\n",node->succiPort,node->prediPort);
			return ;
		}
		else
		{
			node->NEWfd = FDsocket;
			memset(msg,0,128);
			sprintf(msg,"QRY %d %d\n",node->myID,No_Novo);
			printf("%s",msg);
			sendTCP(msg, node->succiFD);
			printf("2\n");
			printf("Succi: %d \t Predi: %d\n",node->succiID,node->prediID);
			printf("Succi FD: %d \t Predi FD: %d\n",node->succiFD,node->prediFD);
			printf("Succi TCP: %d \t Predi TCP: %d\n",node->succiPort,node->prediPort);
			return ;
		}
	}
}

void GetIP(ringStruct* node)
{
	//char localmachine[128];
	//struct hostent *h;
	//struct in_addr *a;
	char addr[128];
	//struct sockaddr
	//sa_data

	char addressOutputBuffer[INET6_ADDRSTRLEN];
	struct ifaddrs *interfaceArray;
	if(getifaddrs(&interfaceArray) != 0)
	{
		printf("erro na getifaddrs\n");
		exit(-1);
	}

	struct ifaddrs * aux;
	for(aux = interfaceArray; aux != NULL; aux = aux->ifa_next)
    {

		if(aux->ifa_addr->sa_family == AF_INET && strcmp(aux->ifa_name, "lo") != 0)
		{
			strcpy(addr, inet_ntop(aux->ifa_addr->sa_family, aux, addressOutputBuffer, sizeof(addressOutputBuffer)));
			printf("Self Address on %s:  %s \n",aux->ifa_name, addr);
			break;
		}
	}
	strcpy(node->myIP, addr);

	freeifaddrs(interfaceArray);
/*
	if(gethostname(localmachine,128) == -1)
    {
      printf("\nError during hostname query\n\n");
      exit(1);
    }
	if((h=gethostbyname(localmachine))==NULL)
    {
      printf("\nError during hostname query\n\n");
      exit(1);//error
    }

  printf("Hostname: %s\n",localmachine);
  a = (struct in_addr*)h->h_addr_list[0];

  printf("internet address: %s (%08lX)\n", inet_ntoa(*a), (long unsigned int)ntohl(a->s_addr));
*/
  //strcpy(node->myIP,inet_ntoa(*a));
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
	char cmd[128], msg[128], ip[128];
	int no_arrq, no_novo, no_dest, tcp;
	printf("A analisar: %s",request);

	FDsocket = nodeFD;

	if(sscanf(request,"%s %d %d %d %s %d",cmd,&no_arrq,&no_novo,&no_dest,ip,&tcp)!= 5)
	{
		//printf("Comando 1: %s %d\n ",cmd,(int)strlen(cmd));
		if(sscanf(request,"%s %d %s %d",cmd,&no_novo,ip,&tcp)!= 4)
		{
  			//printf("Comando 2: %s %d\n ",cmd,(int)strlen(cmd));
			if(sscanf(request,"%s %d %d",cmd,&no_arrq,&no_novo)!=3)
			{
				if(sscanf(request,"%s %d",cmd,&no_novo)!= 2)
				{
						printf("Bad Message 5\n");
						return 1;
				}
				else
				{
						/*if(strcmp(cmd,"ID") == 0)
						{
							if(node->succiID == -1 && node->prediFD == -1)
							{
								node->NEWfd = nodeFD;
								memset(msg,0,128);
								sprintf(msg,"SUCC %d %s %d\n",node->myID, node->myIP, node->myPort);
								printf("%s",msg);
								sendTCP(msg, nodeFD);
								printf("1\n");
								printf("Succi: %d \t Predi: %d\n",node->succiID,node->prediID);
								printf("Succi FD: %d \t Predi FD: %d\n",node->succiFD,node->prediFD);
								printf("Succi TCP: %d \t Predi TCP: %d\n",node->succiPort,node->prediPort);
									return 0;
							}
							else
							{
								node->NEWfd = nodeFD;
								memset(msg,0,128);
								sprintf(msg,"QRY %d %d\n",node->myID,no_novo);
								printf("%s",msg);
								sendTCP(msg, node->succiFD);
								printf("2\n");
								printf("Succi: %d \t Predi: %d\n",node->succiID,node->prediID);
								printf("Succi FD: %d \t Predi FD: %d\n",node->succiFD,node->prediFD);
								printf("Succi TCP: %d \t Predi TCP: %d\n",node->succiPort,node->prediPort);
								return 0;
							}
						}
						else
						{
							printf("Bad Message 4\n");
							return 1;
						}*/
				}
			 }
			 else
			 {
				 if(node->succiFD == -1 || node->prediFD == -1)
				 {
					printf("Big Shet I Guess\n");
				 }
				 else
				 {
					if(responsability(node->prediID,node->myID,no_novo))
					{
						printf("My responsability\n");
						memset(msg,0,128);
						if(no_arrq == node->myID){
							memset(msg,0,128);
							sprintf(msg,"SUCC %d %s %d\n",node->myID, node->myIP, node->myPort);
							printf("Socket %d with %s",node->NEWfd,msg);
							sendTCP(msg, node->NEWfd);
							printf("7\n");
							printf("Succi: %d \t Predi: %d\n",node->succiID,node->prediID);
							printf("Succi FD: %d \t Predi FD: %d\n",node->succiFD,node->prediFD);
							printf("Succi TCP: %d \t Predi TCP: %d\n",node->succiPort,node->prediPort);
						}
						else
						{
							sprintf(msg,"RSP %d %d %d %s %d \n",no_arrq, no_novo,node->myID,node->myIP,node->myPort);
							printf("Sending to Master %s",msg);
							sendTCP(msg, node->prediFD);
							printf("8\n");
							printf("Succi: %d \t Predi: %d\n",node->succiID,node->prediID);
							printf("Succi FD: %d \t Predi FD: %d\n",node->succiFD,node->prediFD);
							printf("Succi TCP: %d \t Predi TCP: %d\n",node->succiPort,node->prediPort);
						}
					return 0;
					}
					else
					{
						memset(msg,0,128);
						sprintf(msg,"QRY %d %d \n",no_arrq, no_novo);
						printf("Sending to Succi %s",msg);
						sendTCP(msg, node->succiFD);
						printf("5\n");
						printf("Succi: %d \t Predi: %d\n",node->succiID,node->prediID);
						printf("Succi FD: %d \t Predi FD: %d\n",node->succiFD,node->prediFD);
						printf("Succi TCP: %d \t Predi TCP: %d\n",node->succiPort,node->prediPort);
						return 0;
					}
				 }
			}
		}
		else
		{
			if(strcmp(cmd,"NEW") == 0)
			{
				if(node->succiID == -1 && node->prediFD == -1)
				{
					node->prediID = no_novo;
					strcpy(node->prediIP,ip);
					node->prediPort = tcp;
					node->prediFD = nodeFD;
					node->succiID = no_novo;
					strcpy(node->succiIP,ip);
					node->succiPort = tcp;
					node->succiFD = nodeFD;
					printf("3\n");
					printf("Succi: %d \t Predi: %d\n",node->succiID,node->prediID);
					printf("Succi FD: %d \t Predi FD: %d\n",node->succiFD,node->prediFD);
					printf("Succi TCP: %d \t Predi TCP: %d\n",node->succiPort,node->prediPort);
				    return 0;
				}else
				{
				  node->prediID = no_novo;
				  strcpy(node->prediIP,ip);
				  node->prediPort = tcp;
				  node->prediFD = nodeFD;
				  printf("4\n");
				  printf("Succi: %d \t Predi: %d\n",node->succiID,node->prediID);
				  printf("Succi FD: %d \t Predi FD: %d\n",node->succiFD,node->prediFD);
				  printf("Succi TCP: %d \t Predi TCP: %d\n",node->succiPort,node->prediPort);
				  return 0;
				}
			}
			else
			{
				printf("Bad Message 7\n");
				return 1;
			}
		}
	}
	else
	{
		//printf("Comando 3: %s %d\n ",cmd,(int)strlen(cmd));
		if(strcmp(cmd,"RSP") == 0)
		{
				sprintf(msg,"SUCC %d %s %d\n",no_dest, ip, tcp);
				printf("%s\n",msg);
				sendTCP(msg, nodeFD);
				return 0;
		}
		else
		{
			printf("Bad Message 6\n");
			return 1;
		}
	}

	return 1;
}

void Join_Ring(ringStruct* node, socketStruct start)
{
  char msg[128];
  char buffer[128];
  char localmachine[128];
  char cmd[10],idIP[20];
  int tmpid, tmpport;
  char tmpip[128];
  int startid,startTCP,ringx;
  struct hostent *h;
  struct in_addr *a;
  int temp;
  socketStruct PeerTCP;


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
    if(gethostname(localmachine,128) == -1)
    {
      printf("\nError during hostname query\n\n");
      exit(1);
    }
    if((h=gethostbyname(localmachine))==NULL)
      {

        exit(1);//error
      }
    //printf("Hostname: %s\n",localmachine);
    a = (struct in_addr*)h->h_addr_list[0];

    sprintf(msg,"REG %d %d %s %d\n",node->ringID, node->myID, node->myIP, node->myPort);
    strcpy(node->myIP,inet_ntoa(*a));
    printf("%s\n",msg);

    if((temp = sendUDP(msg,start)) == -1)
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
      while(startid == node->myID)
      {
		    printf("Can't use identifier %d, please choose a different one: ",node->myID);
        scanf("%d",&(node->myID));
       }
      printf("IP: %s\nPort: %d\n",idIP,startTCP);

 // vai se ligar ao no de arranque
 // enviando ID i
	  node->starter = startid;
      PeerTCP = setupSocket(idIP, startTCP, 'T');
      memset((void*)&msg,'\0',sizeof(msg));
      sprintf(msg,"ID %d\n", node->myID);
      printf("Sending 1 %s",msg);
      sendTCP(msg,PeerTCP.socketFD);
      memset(buffer,0,128);

	  printf("Receive 1\n");
      recvTCP(buffer,PeerTCP);

  	  closeSocket(PeerTCP);
  	  printf("TCP received: %s\n",buffer);

      // Message after query through the ring
  	  if(sscanf(buffer,"%s %d %s %d",cmd, &tmpid, tmpip, &tmpport) != 4)
  	  {
    		printf("Bad Message 2\n");
    		exit(1);
  	  }

	     while(tmpid == node->myID)
      {
		    printf("Can't use identifier %d, please choose a different one: ",node->myID);
        scanf("%d",&(node->myID));
      }

      if(strcmp(cmd,"SUCC") != 0)
  	  {
    		printf("Bad Message 3\n");
    		exit(1);
	    }

	    memset((void*)&msg,'\0',sizeof(msg));
      // Sends message to l informing him that i'm his predi
      // l is now my succi and information is updated
      sprintf(msg,"NEW %d %s %d\n", node->myID, node->myIP, node->myPort);
      printf("Sending to %d, message %s",PeerTCP.socketFD, msg);
      PeerTCP = setupSocket(tmpip,tmpport,'T');
      sendTCP(msg,PeerTCP.socketFD);
      //printf("Sent\n");
      if(node->starter == tmpid)
      {
		node->succiFD = PeerTCP.socketFD;
		strcpy(node->succiIP,tmpip);
		node->succiID = tmpid;
		node->succiPort = tmpport;
		node->prediFD = PeerTCP.socketFD;
		strcpy(node->prediIP,tmpip);
		node->prediID = tmpid;
		node->prediPort = tmpport;
   	  }
   	  node->succiFD = PeerTCP.socketFD;
	  strcpy(node->succiIP,tmpip);
	  node->succiID = tmpid;
	  node->succiPort = tmpport;
      //memset(buffer,0,128);
      return;
    }
  }
  return;
}


//socketStruct setupListenSocket(char * myIP, int myPort)

int removeNode(ringStruct * ringData, socketStruct socketCFG, socketStruct succiPeer, socketStruct prediPeer)
{
  //getting out with return 0, everything went fine
  //getting out with return 1, something went wrong
  char msg[128], buffer[128];
  if(ringData->succiID == -1 && ringData->prediID == -1) // If the node is unique remove the ring from the server
  {
    sprintf(msg,"UNR %i\n",ringData->ringID);
    if(sendUDP(msg,socketCFG) == -1)
      return 1;
    if(recvUDP(buffer,socketCFG) == -1)
      return 1;
    else if(strcmp(buffer,"OK")==0)
      return 0;
    else
      return 1;
  }
  else
  {
    if(ringData->starter == 1) // Test to check if the current node is the starter node. If it is, put the next node as the starter one
    {
      sprintf(msg,"REG %i %i %s %i\n", ringData->ringID, ringData->succiID, ringData->succiIP, ringData->succiPort);
      if(sendUDP(msg,socketCFG) == -1)
        return 1;
      if(recvUDP(buffer,socketCFG) == -1)
        return 1;
      else if(strcmp(buffer,"OK")==0)
      {
        memset(msg,0,strlen(msg));
        sprintf(msg,"BOOT\n");
        //This part is not well done
        sendTCP(msg, succiPeer.socketFD);
        closeSocket(succiPeer);
        memset(msg,0,strlen(msg));
        sprintf(msg,"CON %i %s %i\n", ringData->succiID, ringData->succiIP, ringData->succiPort);
        sendTCP(msg, prediPeer.socketFD);
        closeSocket(prediPeer);
        return 0;
      }
      else // Default Case
      {
        closeSocket(succiPeer);
        memset(msg,0,strlen(msg));
        sprintf(msg,"CON %i %s %i\n", ringData->succiID, ringData->succiIP, ringData->succiPort);
        sendTCP(msg, prediPeer.socketFD);
        closeSocket(prediPeer);
        return 0;
      }
    }
    else
      return 1;
  }
  return 1;
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
