#ifndef _NETWORK_H
#define _NETWORK_H

#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

typedef struct socketStruct
{
  int socketFD;
  struct sockaddr_in * addr;
  socklen_t addrlen;
} socketStruct;

int sendUDP(char * , int , socketStruct );
int recvUDP(char * ,socketStruct );

void sendTCP(char * , int , socketStruct );
int recvTCP(char * ,socketStruct );

socketStruct setupSocket(char * , int , char );
void closeSocket(socketStruct );

#endif
