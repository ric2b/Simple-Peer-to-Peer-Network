#ifndef _INTERFACE_H
#define _INTERFACE_H

#include "network.h"
#include "ringOps.h"
#include "triggers.h"

int check_arguments(int argc, char **argv, char* bootIP, int * bootport, int* ringport, char * externalIP, char * option);

int run_commands(ringStruct* , socketStruct, ringStruct *);
// nao e necessario meter o nome das variaveis apenas o tipo
#endif
