#include <stdio.h>
#include <stdlib.h>
//#include <string.h>

#include "ringOps.h"
#include "network.h"

typedef struct ringStruct
{	
  int ringID;
  int myID;
  char myIP[128];
  int myPort
  int succiID;
  char succiIP[128];
  int succiPort;
  int prediID;
  char * prediIP[128];
  int prediPort;
} ringStruct;

ringStruct setupListenSocket(int ringID, char * myIP, int myPort)
{
  ringStruct ringData;

  ringData.ringID = ringID;
  ringData.myID = myID;
  gethostname(ringData.myIP, 128);

}

ringStruct joinRing_KnownSucci(socketStruct ServerSocket, int ringID, int myID, int succiID, char * succiIP, int succiPort)
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