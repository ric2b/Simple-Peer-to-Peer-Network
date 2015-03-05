#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "network.h"

int setupSocket(char * servidorArranque, int port, struct sockaddr_in * addr)
{
  struct hostent *h;
  struct in_addr *a;
  int socketFD;

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

  return socketFD;
}