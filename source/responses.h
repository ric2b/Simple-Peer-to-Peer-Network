#ifndef _RESPONSES_H
#define _RESPONSES_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ifaddrs.h>
#include "ringOps.h"
#include "network.h"

int JR_Message(char*,ringStruct*,int);

int Message_ID(ringStruct* node, char* request, int senderSocket);

int Message_NEW(ringStruct* node, char* request, int senderSocket);

int Message_RSP(ringStruct* node, char* request);

int Message_QRY(ringStruct* node, char* request);

int Message_SUCC(ringStruct* node, char* request);

int Message_CON(ringStruct* node, char* request);

int Message_BOOT(ringStruct* node, char* request);


#endif
