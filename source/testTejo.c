#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>

#include "network.h"

int main(int argc, char ** argv)
{
  int n;
  char buffer[128];
  char host[128];  
  char message[128];
  char mode[128];
  int port;

  socketUDP socketUDPcfg;

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
    if(strcmp("UDP", mode) == 0)
    {

      socketUDPcfg = setupSocket(host, port);     

      printf("> ");
      fgets(message, 128, stdin);
      n=sendUDP(message,strlen(message),socketUDPcfg);
      if(n==-1)
      {
        printf("erro a enviar\n");
        exit(2);
      }

      n = recvUDP(buffer, socketUDPcfg);
      if(n==-1)
      {
        printf("erro a receber\n");
        exit(3);//error
      }

      printf("[server]: %s\n", buffer);
    }

    else if(strcmp("TCP", mode) == 0)
    {

    }
    
    else
    {
      printf("modo inválido, TCP ou UDP\n");
      exit(-1);
    }
  }

  exit(4);
}
