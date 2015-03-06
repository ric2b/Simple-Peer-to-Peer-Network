#ifndef _NETWORK_H
#define _NETWORK_H

typedef struct requestUDP
{
  char request[128];
  int fdUDP;
}requestUDP;


requestUDP* createUDP(char* , int , int , char* , int );


#endif