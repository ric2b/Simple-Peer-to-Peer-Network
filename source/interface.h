#ifndef _INTERFACE_H
#define _INTERFACE_H

#include "network.h"
#include "ringOps.h"
#include "triggers.h"
#include "print_messages.h"


int check_arguments(int argc, char **argv, char* bootIP, int * bootport, int* ringport, char * externalIP, char * option,int*  DEBUG_MODE);

int run_commands(ringStruct* , socketStruct);
// nao e necessario meter o nome das variaveis apenas o tipo
#endif
