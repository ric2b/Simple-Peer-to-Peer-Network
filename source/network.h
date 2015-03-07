#ifndef _NETWORK_H
#define _NETWORK_H

#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

typedef struct socketUDP
{
  int socketFD;
  struct sockaddr_in * addr;
  socklen_t addrlen;
} socketUDP;

int sendUDP(char * msg, int msg_length, socketUDP socketCFG);
int recvUDP(char * buffer,socketUDP socketCFG);

socketUDP setupSocket(char * servidorArranque, int port);
void closeSocket(socketUDP socketCFG);

#endif
