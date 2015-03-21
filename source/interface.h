#ifndef _INTERFACE_H
#define _INTERFACE_H

#include "network.h"
#include "ringOps.h"
#include "triggers.h"

int check_arguments(int , char **, char* , int * , int* , char * );

int run_commands(ringStruct* , socketStruct, ringStruct *);
// nao e necessario meter o nome das variaveis apenas o tipo
#endif
