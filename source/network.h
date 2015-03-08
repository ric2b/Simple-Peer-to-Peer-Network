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

int sendUDP(char * msg, int msg_length, socketStruct socketCFG);
int recvUDP(char * buffer,socketStruct socketCFG);

void sendTCP(char * msg, int msg_length, socketStruct socketCFG);
int recvTCP(char * buffer,socketStruct socketCFG);



int listenSocket(int listen_port);

int aceita_cliente(int server_socket, char * remote_address);



socketStruct setupSocket(char * servidor, int port, char protocol);
void closeSocket(socketStruct socketCFG);

#endif
