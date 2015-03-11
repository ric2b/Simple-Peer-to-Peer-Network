#ifndef _RINGOPS_H
#define _RINGOPS_H

#include "network.h"

typedef struct ringStruct
{
  socketStruct ListenSocket;
  int ringID;
  int myID;
  char myIP[128];
  int myPort;
  int succiID;
  char succiIP[128];
  int succiPort;
  int prediID;
  char * prediIP[128];
  int prediPort;
  int starter;
} ringStruct;

int removeNode(ringStruct * ringData, socketStruct socketCFG, socketStruct succiPeer, socketStruct prediPeer);

int searchNode(ringStruct * ringData, socketStruct succiPeer, int k);

int showNode(ringStruct * ringData);

/*
socketStruct setupListenSocket(char * myIP, int myPort);
int joinRing_KnownSucci(ringStruct * ringData, int ringID, int myID, int succiID, char * succiIP, int succiPort);
*/
#endif
