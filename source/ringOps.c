#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "ringOps.h"
#include "network.h"

<<<<<<< HEAD
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


  sprintf(msg,"BQRY %d",ring);
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
      sprintf(msg,"ID %d\n", identifier);
      printf("Sending %s with %d bytes\n",msg,(int)strlen(msg));
      sendTCP(msg,strlen(msg),PeerTCP);
    }
  }
   
}

/*
socketStruct setupListenSocket(char * myIP, int myPort)
=======
int removeNode(ringStruct * ringData, socketStruct socketCFG, socketStruct succiPeer, socketStruct prediPeer)
>>>>>>> origin/removeNode
{
  //getting out with return 0, everything went fine
  //getting out with return 1, something went wrong
  char msg[128], buffer[128];
  if(ringData->succiID == -1 && ringData->prediID == -1) // If the node is unique remove the ring from the server
  {
    sprintf(msg,"UNR %i\n",ringData->ringID);
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
    if(ringData->starter == 1) // Test to check if the current node is the starter node. If it is, put the next node as the starter one
    {
      sprintf(msg,"REG %i %i %s %i\n", ringData->ringID, ringData->succiID, ringData->succiIP, ringData->succiPort);
      if(sendUDP(msg,strlen(msg),socketCFG) == -1)
        return 1;
      if(recvUDP(buffer,socketCFG) == -1)
        return 1;
      else if(strcmp(buffer,"OK")==0)
      {
        memset(msg,0,strlen(msg));
        sprintf(msg,"BOOT\n");
        //This part is not well done
        sendTCP(msg, strlen(msg), succiPeer);
        closeSocket(succiPeer);
        memset(msg,0,strlen(msg));
        sprintf(msg,"CON %i %s %i\n", ringData->succiID, ringData->succiIP, ringData->succiPort);
        sendTCP(msg, strlen(msg), prediPeer);
        closeSocket(prediPeer);
        return 0;
      }
      else // Default Case
      {
        closeSocket(succiPeer);
        memset(msg,0,strlen(msg));
        sprintf(msg,"CON %i %s %i\n", ringData->succiID, ringData->succiIP, ringData->succiPort);
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

int distance(int k, int l)
{ // if it returns -1 something wrong occured
  int res;
  if(l >= k)
    res = l-k;
  else if(l < k)
    res = 64 + l - k;
  else
    res = -1;
  return res;
}

int responsability(int predi, int succi, int k)
{ // returns 1 if succi is responsible for k, 0 otherwise.
  if(distance(k,succi) < distance(k,predi))
    return 1;
  else
    return 0;
}

int searchNode(ringStruct * ringData, socketStruct succiPeer, int k)
{ // returns 0 if everything went as expected
  char msg[128], buffer[128], cmd[10], qryIP[40];
  int asked, queried, qryID, qryTCP;
  if(responsability(ringData->prediID,ringData->succiID,k) == 1)
  {
    printf("%i %s %i", ringData->succiID, ringData->succiIP, ringData->succiPort);
    return 0;
  }
  else
  {
    sprintf(msg,"QRY %i %i\n", ringData->myID, k);
    sendTCP(msg, strlen(msg), succiPeer);
    recvTCP(buffer, succiPeer);
    sscanf(buffer,"%s %i %i %i %s %i", cmd, &asked, &queried, &qryID, qryIP, &qryTCP);
    if(strcmp(cmd,"RSP") == 0 && asked == ringData->myID && queried == k)
    {
      printf("%i %s %i", qryID, qryIP, qryTCP);
      return 0;
    }
    else
      return 1;
  }
  return 1;
}

int showNode(ringStruct * ringData)
{
  printf("%i %i %i %i", ringData->ringID, ringData->myID, ringData->succiID, ringData->prediID);
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
