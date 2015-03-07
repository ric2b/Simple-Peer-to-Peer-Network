#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "network.h"
#define buffersize 128

int main(int argc, char ** argv)
{
  int port;
  char buffer[buffersize], host[buffersize], message[buffersize], mode[buffersize];
  socketStruct socketcfg;

  if(argc != 4)
  {
    printf("Introduza o host, a porta e o modo [TCP/UDP]\n");
    exit(-1);
  }

  strcpy(host, argv[1]);
  sscanf(argv[2], " %d ", &port);
  strcpy(mode, argv[3]); 

  while(1)
  {
    printf("> ");
    fgets(message, buffersize, stdin);
    /* --------------------------< UDP >--------------------------------- */
    if(strcmp("UDP", mode) == 0)
    {
      socketcfg = setupSocket(host, port, 'U');
      sendUDP(message,strlen(message),socketcfg);
      recvUDP(buffer, socketcfg);
    }
    /* --------------------------< TCP >--------------------------------- */
    else if(strcmp("TCP", mode) == 0)
    {
      socketcfg = setupSocket(host, port, 'T');      
      sendTCP(message, strlen(message), socketcfg);
      recvTCP(buffer, socketcfg);
    }
    else
    {
      printf("modo inválido, TCP ou UDP\n");
      exit(-1); 
    }
    printf("[server]: %s\n", buffer);
  }

  exit(0);
}