#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "ringOps.h"
#include "network.h"

int removeNode(ringStruct * ringData, socketStruct socketCFG, socketStruct succiPeer, socketStruct prediPeer)
{
  //getting out with return 0, everything went fine
  //getting out with return 1, something went wrong
  char msg[128], buffer[128];
  if(ringData.succiID == -1 && ringData.prediID == -1) // If the node is unique remove the ring from the server
  {
    sprintf(msg,"UNR %i\n",ringData.ringID);
    if(sendUDP(msg,strlen(msg),socketCFG) == -1)
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
    if(ringData.starter == 1) // Test to check if the current node is the starter node. If it is, put the next node as the starter one
    {
      sprintf(msg,"REG %i %i %s %i\n", ringData.ringID, ringData.succiID, ringData.succiIP, ringData.succiPort);
      if(sendUDP(msg,strlen(msg),socketCFG) == -1)
        return 1;
      if(recvUDP(buffer,socketCFG) == -1)
        return 1;
      else if(strcmp(buffer,"OK")==0)
      {
        memset(msg,0,strlen(msg));
        sprintf(msg,"BOOT\n");
        sendTCP(msg, strlen(msg), succiPeer);
        closeSocket(succiPeer);
        memset(msg,0,strlen(msg));
        sprintf(msg,"CON %i %s %i\n", ringData.succiID, ringData.succiIP, ringData.succiPort);
        sendTCP(msg, strlen(msg), prediPeer);
        closeSocket(prediPeer);
        return 0;
      }
      else // Default Case
      {
        closeSocket(succiPeer);
        memset(msg,0,strlen(msg));
        sprintf(msg,"CON %i %s %i\n", ringData.succiID, ringData.succiIP, ringData.succiPort);
        sendTCP(msg, strlen(msg), prediPeer);
        closeSocket(prediPeer);
        return 0;
      }
    }
    else
      return 1;
  }
  return 1;
}

int searchNode(ringStruct * ringData, socketStruct succiPeer, int k)
{
  char msg[128], buffer[128], qryIP[30], cmd[10];
  int qryID, qryTCP;

  if( k > ringData.prediID && k <= ringData.succiID) // put thing of the distance, this is wrong now!!!!
  {
    printf("%i %s %i", ringData.succiID, ringData.succiIP, ringData.succiPort);
    exit(0);
  }
  else
  {
    sprintf(msg,"QRY %i %i\n", ringData.myID, k);
    sendTCP(msg, strlen(msg), succiPeer);
    recvTCP(buffer, succiPeer);

    sscanf(buffer,"%s %i %i %i %s %i", cmd, &ringx, &ringID, idIP, &startTCP) // YOU LEFT SESSION OF CODING HERE! COMEBACK! DONT LEAVE ME!

    printf("%i %s %i", qryID, qryIP, qryTCP);

    return 0;
  }
  return 1;
}

int showNode(ringStruct * ringData)
{
  printf("%i %i %i %i", ringData.ringID, ringData.myID, ringData.succiID, ringData.prediID);
  return 0;
}

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
