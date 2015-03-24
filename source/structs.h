#ifndef _STRUCTS_H
#define _STRUCTS_H

#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ifaddrs.h>

typedef struct socketStruct
{
  int socketFD;
  struct sockaddr_in * addr;
  socklen_t addrlen;
} socketStruct;

typedef struct ringStruct
{
  socketStruct ListenSocket;
  int ringID;
  int myID;
  char myIP[128];
  int myPort;

  int succiID;
  char succiIP[128];
  int succiPort;
  int succiFD;

  int prediID;
  char prediIP[128];
  int prediPort;
  int prediFD;

  int starter;
  int succ_status;
  int search_status;
  int NEWfd;

} ringStruct;

#endif