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

ringStruct joinRing_KnownSucci(socketStruct startServerSocket, int succiID, char * succiIP);