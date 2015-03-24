#ifndef _NETWORK_H
#define _NETWORK_H

#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <net/if.h>

#include "structs.h"

#define MAX_PENDING 10

int sendUDP(char * msg, socketStruct socketCFG);
int recvUDP(char * buffer,socketStruct socketCFG);

void sendTCP(char *, int );
//void sendTCPv2(char * , int , int );
void recvTCP(char * ,socketStruct );

int listenSocket(int* );
int aceita_cliente(int , char * );
void GetIP(ringStruct* node);

socketStruct setupSocket(char * , int , char );
void closeSocket(socketStruct );

#endif
