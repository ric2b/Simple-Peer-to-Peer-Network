#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "network.h"

int sendUDP(char * msg, int msg_length, socketUDP socketCFG)
{
  return sendto(socketCFG.socketFD,msg,msg_length,0,(struct sockaddr*)socketCFG.addr,socketCFG.addrlen);
}

int recvUDP(char * buffer,socketUDP socketCFG)
{
  memset(buffer,0,128);
  return recvfrom(socketCFG.socketFD,buffer,128,0,(struct sockaddr*)socketCFG.addr,&(socketCFG.addrlen));
}

socketUDP setupSocket(char * servidorArranque, int port)
{
  struct hostent *h;
  struct in_addr *a;
  int socketFD;

  socketUDP socketCFG;

  struct sockaddr_in * addr = (struct sockaddr_in *) malloc(sizeof(struct sockaddr_in));

  /* --------------------------< GetHostByName >--------------------------------- */

  if((h=gethostbyname(servidorArranque))==NULL) //substituir por bootIP
  {
    exit(-1);//error
  }

  /* --------------------------< SetupSocket >--------------------------------- */

  socketFD = socket(AF_INET,SOCK_DGRAM,0);

  if(socketFD == -1)
  {
    exit(1);
  }

  a=(struct in_addr*)h->h_addr_list[0];

  memset((void*)addr,(int)'\0',sizeof(*addr));
  addr->sin_family = AF_INET;
  addr->sin_addr = *a;
  addr->sin_port = htons(port);

  socklen_t addrlen = sizeof(*addr);

  socketCFG.socketFD = socketFD;
  socketCFG.addr = addr;
  socketCFG.addrlen = addrlen;

  return socketCFG;
}
