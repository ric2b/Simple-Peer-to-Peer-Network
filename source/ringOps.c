#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
//#include <string.h>

#include "ringOps.h"
#include "network.h"

#define MAXPENDING 10

ringStruct setupListenSocket()
{
  ringStruct ringInitial;

  ringInitial.ListenSocket = setupSocket("NULL", 0, 'T');

  if(listen(ringInitial.ListenSocket.socketFD, MAXPENDING) != 0)
  {
    printf("erro a fazer listen\n");
    exit(-1);
  }
  
  getsockname(ringInitial.ListenSocket.socketFD, (struct sockaddr *) ringInitial.ListenSocket.addr, &(ringInitial.ListenSocket.addrlen));
  
  if(bind(ringInitial.ListenSocket.socketFD, (struct sockaddr *) ringInitial.ListenSocket.addr, ringInitial.ListenSocket.addrlen) < 0)
  {
    printf("erro a fazer bind\n");
    exit(-1);
  }

  ringInitial.ringID = -1;
  ringInitial.myID = -1;
  gethostname(ringInitial.myIP, 128);
  ringInitial.myPort = ntohs(ringInitial.ListenSocket.addr->sin_port);

  return ringInitial;
}

/*
ringStruct joinRing_KnownSucci(ringStruct ringInitial, int ringID, int myID, int succiID, char * succiIP, int succiPort)
{
  ringStruct ringData;
  
  ringData.succiID = succiID;
  strcpy(ringData.succiIP, succiIP);
  ringData.succiPort = succiPort;

  ret=bind(fd,(struct sockaddr*)&addr,sizeof(addr));

  char joinCommand[128*2]; //para aguentar com os 128 do IP + extras
  sprintf(joinCommand, "NEW %d %s %d", myID, )

  void sendTCP(char * msg, int msg_length, socketStruct socketCFG);
  int recvTCP(char * buffer,socketStruct socketCFG);



  return ringData;
}
*/