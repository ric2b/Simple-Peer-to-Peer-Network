#ifndef _INTERFACE_H
#define _INTERFACE_H

int check_arguments(int argc, char **argv, char* bootIP, int * bootport, int* ringport, char * option);
int run_commands(char * userInput, char * cmd, char * succiIP, int * exitProgram, int * identifier, int * ringx, int* succi, int * succiTCP);

#endif
