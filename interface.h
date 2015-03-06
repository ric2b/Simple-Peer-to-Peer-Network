#ifndef _INTERFACE_H
#define _INTERFACE_H

#include "network.h"

int check_arguments(int , char **, char* , int * , int* , char * );
int run_commands(char * , char * , char * , int* , int * , int * , int* , int * , int , char* ,int, requestUDP* );

#endif
