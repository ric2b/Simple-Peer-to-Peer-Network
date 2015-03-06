#ifndef _NETWORK_H
#define _NETWORK_H

typedef struct requestUDP
{
  char request[128];
  int fdUDP;
}requestUDP;

typedef struct requestTCP
{
  char requestTCP[128];
  int fdTCP;
}requestTCP;


requestUDP* createUDP(char* , int , int , char* , int );
requestTCP* TCPconnection(char* ,int);

#endif
