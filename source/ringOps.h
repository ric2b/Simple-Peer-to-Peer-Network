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
  int succiFD;

  int prediID;
  char prediIP[128];
  int prediPort;
  int prediFD;

  int starter;
  int NEWfd;
} ringStruct;


void GetIP(ringStruct* ); //ta aqui só para não ter problemas com includes circulares

void Join_Ring(ringStruct*, socketStruct );

void Node_Initialization(ringStruct* );

int JR_Message(char*,ringStruct*,int);

int removeNode(ringStruct * ringData, socketStruct socketCFG, socketStruct succiPeer, socketStruct prediPeer);

int distance(int k, int l);

int responsability(int predi, int i, int k);

void searchNode(ringStruct * ringData, int k);

void showNode(ringStruct * ringData);

int joinRing_KnownSucci(ringStruct * ringData, int succiID, char * succiIP, int succiPort);
/*
socketStruct setupListenSocket(char * myIP, int myPort);
*/
#endif
