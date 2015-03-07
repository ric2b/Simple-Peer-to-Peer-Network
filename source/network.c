#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "network.h"



/* --------------------------< UDP >--------------------------------- */

int sendUDP(char * msg, int msg_length, socketStruct socketCFG)
{
  return sendto(socketCFG.socketFD,msg,msg_length,0,(struct sockaddr*)socketCFG.addr,socketCFG.addrlen);
}

int recvUDP(char * buffer,socketStruct socketCFG)
{
  memset(buffer,0,128);
  return recvfrom(socketCFG.socketFD,buffer,128,0,(struct sockaddr*)socketCFG.addr,&(socketCFG.addrlen));
}



/* --------------------------< TCP >--------------------------------- */

void sendTCP(char * msg, int msg_length, socketStruct socketCFG)
{
  int nwritten;
  int nleft = strlen(msg);
  while(nleft > 0)
  {
    nwritten = write(socketCFG.socketFD, msg, msg_length);
    nleft -= nwritten;
    msg += nwritten;
  }
}

int recvTCP(char * buffer,socketStruct socketCFG)
{
  memset(buffer,0,128);
  return read(socketCFG.socketFD, buffer, 128);
}



/* --------------------------< SocketCreation >--------------------------------- */

socketStruct setupSocket(char * servidorArranque, int port, char protocol)
{
  struct hostent *h;
  struct in_addr *a;
  int socketFD;

  socketStruct socketCFG;

  struct sockaddr_in * addr = (struct sockaddr_in *) malloc(sizeof(struct sockaddr_in));

  /* --------------------------< GetHostByName >--------------------------------- */

  if((h=gethostbyname(servidorArranque))==NULL) //substituir por bootIP
  {
    exit(-1);//error
  }

  /* --------------------------< SetupSocket >--------------------------------- */
  if(protocol == 'U')
    socketFD = socket(AF_INET,SOCK_DGRAM,0);
  else if(protocol == 'T')
    socketFD = socket(AF_INET,SOCK_STREAM,0);
  else
  {
    printf("protocolo é U ou T\n");
    exit(-1);
  }


  if(socketFD == -1)
  {
    exit(1);
  }

  a=(struct in_addr*)h->h_addr_list[0];

  memset((void*)addr,(int)'\0',sizeof(*addr));
  addr->sin_family = AF_INET;
  addr->sin_addr = *a;
  addr->sin_port = htons(port);

  socketCFG.socketFD = socketFD;
  socketCFG.addr = addr;
  socketCFG.addrlen = sizeof(*addr);

  if(protocol == 'T')
  {
    int n = connect(socketFD,(struct sockaddr*)addr, sizeof(*addr));
    if(n==-1) exit(1);
  }

  return socketCFG;
}

void closeSocket(socketStruct socketCFG)
{
  free(socketCFG.addr);
  close(socketCFG.socketFD);
}
