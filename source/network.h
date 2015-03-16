#ifndef _NETWORK_H
#define _NETWORK_H

#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define MAX_PENDING 10


typedef struct socketStruct
{
  int socketFD;
  struct sockaddr_in * addr;
  socklen_t addrlen;
} socketStruct;


int sendTCP(char * msg, int msg_length, socketStruct socketCFG);
int recvTCP(char * buffer,socketStruct socketCFG);

void sendTCP(char * , int , socketStruct );
void sendTCPv2(char * , int , int );
int recvTCP(char * ,socketStruct );



int listenSocket(int* );
int aceita_cliente(int , char * );



socketStruct setupSocket(char * , int , char );
void closeSocket(socketStruct );

#endif
