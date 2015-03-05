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
/*
int send(void)
{
  int n, addrlen;
  char buffer[128];



/* --------------------------< SendingToUDP >--------------------------------- */

  
/*
  n=sendto(fd,"BQRY 69\n",7,0,(struct sockaddr*)&addr,sizeof(addr));

  if(n==-1)
  {
  exit(2);
  }

/* ----------------------------< ReceivingFromUDP >------------------------------- */

/*  addrlen = sizeof(addr);

  n = recvfrom(fd,buffer,128,0,(struct sockaddr*)&addr,&addrlen);

  if(n==-1)
  {
    exit(3);//error
  }


  write(1,"> [server]: ",12);//stdout
  write(1,buffer,n);
  printf("\n");

  close(fd);
  exit(4);


}
*/