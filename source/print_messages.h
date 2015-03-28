#ifndef _PRINT_MESSAGES_H
#define _PRINT_MESSAGES_H

#include "structs.h"

void message_handler(int debug_mode, int message_id,char* message,ringStruct* node, int nb);
void print_ring(int* valores);

#endif