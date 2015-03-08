#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
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
  const int       optVal = 1;
  const socklen_t optLen = sizeof(optVal);
  socketStruct socketCFG;

  struct sockaddr_in * addr = (struct sockaddr_in *) malloc(sizeof(struct sockaddr_in));

  /* --------------------------< GetHostByName >--------------------------------- */

  if(protocol == 'U')
  {
    if((h=gethostbyname(servidorArranque))==NULL) 
    {
      exit(-1);//error
    }
  }

  /* --------------------------< SetupSocket >--------------------------------- */
  if(protocol == 'U')
  {
    printf("Socket %c\n",protocol);
    socketFD = socket(AF_INET,SOCK_DGRAM,0);
  }
  else if(protocol == 'T' || protocol == 'S')
  {
    printf("Socket %c\n",protocol);
    socketFD = socket(AF_INET,SOCK_STREAM,0);
  }
  else
  {
    printf("protocolo é U ou T. S para criar servidor TCP\n");
    exit(-1);
  }
  if(socketFD == -1)
  {
    printf("Can´t create socket %x",protocol);
    exit(1);
  }
  printf("1 %s\n",strerror(errno));
  if(protocol == 'U')
    a=(struct in_addr*)h->h_addr_list[0];

  memset((void*)addr,(int)'\0',sizeof(*addr));

  addr->sin_family = AF_INET;

  if(protocol == 'T')
    addr->sin_addr.s_addr = inet_addr(servidorArranque);
  if(protocol == 'U')
    addr->sin_addr = *a;
  if(protocol == 'S')
    addr->sin_addr.s_addr = INADDR_ANY;

  addr->sin_port = htons(port);

  socketCFG.socketFD = socketFD;
  socketCFG.addr = addr;
  socketCFG.addrlen = sizeof(*addr);

  setsockopt(socketFD, SOL_SOCKET, SO_REUSEADDR, (void*) &optVal, optLen);

  printf("2 %s\n",strerror(errno));

  if(protocol == 'T')
  {
    int n = connect(socketFD,(struct sockaddr*)addr, sizeof(*addr));
    if(n==-1)
    {
      printf("Can´t establish TCP connection with %s\n", servidorArranque);
      exit(1);
    } 
  }
  if(protocol == 'S')
  {
    printf("3 %s\n",strerror(errno));
    if(bind(socketFD,(struct sockaddr*)addr,sizeof(*addr))==-1)
    {
      printf("4 %s\n",strerror(errno));
      printf("Port %d already binded\n\n",port);
      exit(1);//error
    }
    if(listen(socketFD,5)==-1)
    {
      printf("Reached maximum queue on listen\n\n");
      exit(1); //error
    }
    printf("Establish TCP connection server \n\n", servidorArranque);
  }
  printf("Finished socket creation (%c), exiting\n\n", protocol);
  return socketCFG;
}

void closeSocket(socketStruct socketCFG)
{
  free(socketCFG.addr);
  close(socketCFG.socketFD);
}


/*requestUDP* setupTCPserver(char* bootIP, int bootport, int status, char* command, int socketUDP) //rever tudo
{
  int fd,addrlen,newfd;
  struct sockaddr_in addr;
  int n,nw;
  char* ptr, buffer[128];

  if((fd = socket(AF_INET,SOCK_STREAM,0)) ==-1)
    exit(1); //error
  
  memset((void*)&addr,(int)'\0',sizeof(addr));
  addr.sin_family=AF_INET;
  addr.sin_addr.s_addr=htonl(INADDR_ANY);
  addr.sin_port=htons(9000);

  if(bind(fd,(struct sockaddr*)&addr,sizeof(addr))==-1)
    exit(1);//error

  if(listen(fd,5)==-1)
    exit(1); //error
}*/
  /*while(1)
  {
    addrlen=sizeof(addr);
    
    if((newfd=accept(fd,(struct sockaddr*)&addr,&addrlen))==-1)
      exit(1);//error

    while((n=read(newfd,buffer,128))!=0)
    {
      if(n==-1)
        exit(1);//error

      ptr=&buffer[0];

      while(n>0)
      {
        if((nw=write(newfd,ptr,n))<=0)
        exit(1);//error
        n-=nw;
        ptr+=nw;
      }
    }
    close(newfd);
  }
  close(fd); 
  exit(0); 
}*/
