#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "ringOps.h"
#include "network.h"

void Join_Ring(int ring, int identifier, int ringport, socketStruct start)
{
  char msg[128];
  char buffer[128];
  char localmachine[128];
  char cmd[10],idIP[20];
  int startid,startTCP,ringx;
  struct hostent *h;
  struct in_addr *a;
  int temp;
  socketStruct PeerTCP;


  sprintf(msg,"BQRY %d\n",ring);
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
  
    sprintf(msg,"REG %d %d %s %d\n",ring, identifier,inet_ntoa(*a), ringport);
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
      if(startid == identifier)
      {
        printf("Can't use identifier %d, please choose a different one: ",identifier);
        scanf("%d",&identifier);
      }
      printf("IP: %s\nPort: %d\n",idIP,startTCP);
      PeerTCP = setupSocket(idIP, startTCP, 'T');
      memset((void*)&msg,'\0',sizeof(msg));
      sprintf(msg,"ID %d", identifier);
      printf("Sending %s with %d bytes\n",msg,(int)strlen(msg));
      sendTCP(msg,strlen(msg),PeerTCP);
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