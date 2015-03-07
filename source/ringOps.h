#ifndef _RINGOPS_H
#define _RINGOPS_H

#include "network.h"

typedef struct ringStruct
{	
  int ringID;
  int myID;
  int succiID;
  char * succiIP;
  int succiPort;
  int prediID;
  char * prediIP;
  int prediPort;
} ringStruct;

ringStruct joinRing_KnownSucci(socketStruct ServerSocket, int ringID, int myID, int succiID, char * succiIP, int succiPort);

#endif