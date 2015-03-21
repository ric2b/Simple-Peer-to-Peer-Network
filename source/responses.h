#ifndef _RESPONSES_H
#define _RESPONSES_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ifaddrs.h>
#include "ringOps.h"
#include "network.h"


void readPredi(char* msg,ringStruct* myNode);

void readSucci(char* msg,ringStruct* myNode);


#endif
