#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "ringOps.h"
#include "network.h"

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
	char cmd[128], msg[128], ip[128];
	int no_arrq, no_novo, no_dest, tcp;
	printf("A analisar: %s",request);
	
	
	if(sscanf(request,"%s %d %d %d %s %d",cmd,&no_arrq,&no_novo,&no_dest,ip,&tcp)!= 5)
	{
		printf("Comando 1: %s %d\n ",cmd,(int)strlen(cmd));
		if(sscanf(request,"%s %d %s %d",cmd,&no_novo,ip,&tcp)!= 4)
		{	
  			printf("Comando 2: %s %d\n ",cmd,(int)strlen(cmd));
  			if(sscanf(request,"%s %d",cmd,&no_novo)!= 2)
  			{
    				printf("Bad Message 5\n");
    				return 1;
  			}
  			else
  			{
    				if(strcmp(cmd,"ID") == 0)
    				{
      					if(node->succiID == -1)
      					{
                    memset(msg,0,128);
      							sprintf(msg,"SUCC %d %s %d\n",node->myID, node->myIP, node->myPort);
      							printf("%s",msg);	
      							sendTCPv2(msg,strlen(msg), nodeFD);
      							return 0;
      					}
                else
                {
                    memset(msg,0,128);
                    sprintf(msg,"QRY %d %d\n",node->myID,no_novo);
                    printf("%s",msg); 
                    sendTCPv2(msg,strlen(msg), node->succiFD);
                    return 0;
                }  			
            }
    			 else
    			 {
    				  printf("Bad Message 4\n");
    				  return 1;
    			 }
  			}
		}
		else
		{			
			if(strcmp(cmd,"NEW") == 0)
			{
				node->prediID = no_novo;
				strcpy(node->prediIP,ip);
				node->prediPort = tcp;
				node->prediFD = nodeFD;
				return 0;
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
		printf("Comando 3: %s %d\n ",cmd,(int)strlen(cmd));
		if(strcmp(cmd,"RSP") == 0)
		{
				sprintf(msg,"SUCC %d %s %d\n",no_dest, ip, tcp);
				printf("%s\n",msg);	
				sendTCPv2(msg,strlen(msg), nodeFD);
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
  
    sprintf(msg,"REG %d %d %s %d\n",node->ringID, node->myID,inet_ntoa(*a), node->myPort);
    strcpy(node->myIP,inet_ntoa(*a));
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
      while(startid == node->myID)
      {
		    printf("Can't use identifier %d, please choose a different one: ",node->myID);
        scanf("%d",&(node->myID));
       }
      printf("IP: %s\nPort: %d\n",idIP,startTCP);

 // vai se ligar ao no de arranque
 // enviando ID i   

      PeerTCP = setupSocket(idIP, startTCP, 'T');
      memset((void*)&msg,'\0',sizeof(msg));
      sprintf(msg,"ID %d\n", node->myID);
      printf("Sending 1 %s",msg);
      sendTCP(msg,strlen(msg),PeerTCP);
      memset(buffer,0,128);
      
      if(recvTCP(buffer,PeerTCP)<0)
  	  {
    		printf("Can't receive message from TCP connection\n");
    		exit(1);
  	  }
  	  //closeSocket(PeerTCP);
  	  printf("TCP received: %s\n",buffer);
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
      sprintf(msg,"NEW %d %s %d\n", node->myID, node->myIP, node->myPort);
      printf("Sending 2 %s",msg);
      sendTCP(msg,strlen(msg),PeerTCP);
      printf("Sent\n");
      memset(buffer,0,128);
    }
  }
   
}

/*
socketStruct setupListenSocket(char * myIP, int myPort)
{
  socketStruct ListenSocket;

  ListenSocket = setupSocket("NULL", 0, 'T');

  if(listen(ListenSocket.socketFD, MAXPENDING) != 0)
  {
    printf("erro a fazer listen\n");
    exit(-1);
  }
  
  bzero(ListenSocket.addr, ListenSocket.addrlen);
  ListenSocket.addr->sin_family = AF_INET;
  ListenSocket.addr->sin_addr.s_addr = htonl(INADDR_ANY);
  ListenSocket.addr->sin_port = htons(myPort);
  
  if(bind(ListenSocket.socketFD, (struct sockaddr *) ListenSocket.addr, ListenSocket.addrlen) < 0)
  {
    printf("erro a fazer bind\n");
    exit(-1);
  }

  gethostname(myIP, 128);

  return ListenSocket;
}
*/
/*
int joinRing_KnownSucci(ringStruct * ringData, int ringID, int myID, int succiID, char * succiIP, int succiPort)
{
  ringData->succiID = succiID;
  strcpy(ringData->succiIP, succiIP);
  ringData->succiPort = succiPort;

  char joinCommand[128*2]; //para aguentar com os 128 do IP + extras
  sprintf(joinCommand, "NEW %d %s %d", myID, ringData->myIP, ringData->myPort);


  if(connect(ringData->ListenSocket.socketFD, (struct sockaddr*)ringData->ListenSocket.addr, ringData->ListenSocket.addrlen) == -1)
  {
    printf("erro a fazer connect na joinRing_KnownSucci\n");
    exit(-1);
  }

  //socketStruct setupSocket(succiIP, succiPort, 'T');
  void sendTCP(char * msg, int msg_length, socketStruct socketCFG);
  int recvTCP(char * buffer,socketStruct socketCFG);



  return 0;
}
*/
