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


int removeNode(ringStruct * ringData, socketStruct socketCFG, socketStruct succiPeer, socketStruct prediPeer)
{
  //getting out with return 0, everything went fine
  //getting out with return 1, something went wrong

  char msg[128], cmd[10], idIP[30], buffer[128];
  int problem, ringx, ringID, startTCP;

  // If the node is unique remove the ring from the server

  if(ringData.myID == ringData.succiID && ringData.succiID == ringData.prediID)
  {
    sprintf(msg,"UNR %i\n",ringData.ringID); //check if an /n is necessary!

    if((problem = sendUDP(msg,strlen(msg),socketCFG)) == -1)
    {
      exit(1);
    }
    if((problem = recvUDP(buffer,socketCFG)) == -1)
    {
      exit(1);
    }
    else if(strcmp(buffer,"OK")==0)
    {
      exit(0);
    }
    else
    {
      exit(1);
    }
  }
  else
  {

  // Test to check if the current node is the starter node. If it is, put the next node as the starter one

    sprintf(msg,"BQRY %i\n",ringData.ringID);

    if((problem = sendUDP(msg,strlen(msg),socketCFG)) == -1)
      exit(1);

    if((problem = recvUDP(buffer,socketCFG)) == -1)
    {
      exit(1);
    }
    else if(sscanf(buffer,"%s %i %i %s %i", cmd, &ringx, &ringID, idIP, &startTCP) == 5 && strcmp(cmd,"BRSP") == 0 && ringID == ringData.myID)
    {
      memset(buffer,0,strlen(buffer));
      memset(msg,0,strlen(msg));

      sprintf(msg,"REG %i %i %s %i\n", ringData.ringID, ringData.succiID, ringData.succiIP, ringData.succiPort);

      if((problem = sendUDP(msg,strlen(msg),socketCFG)) == -1)
        exit(1);

      if((problem = recvUDP(buffer,socketCFG)) == -1)
      {
        exit(1);
      }
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

        exit(0);
      }

      // Default Case

      else
      {
        closeSocket(succiPeer);

        memset(msg,0,strlen(msg));

        sprintf(msg,"CON %i %s %i\n", ringData.succiID, ringData.succiIP, ringData.succiPort);

        sendTCP(msg, strlen(msg), prediPeer);

        closeSocket(prediPeer);

        exit(0);
      }
    }
    else
    {
      exit(1);
    }
  }
  exit(1);
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

    exit(0);
  }
  exit(1);
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
