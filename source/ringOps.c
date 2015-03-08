#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "ringOps.h"
#include "network.h"



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