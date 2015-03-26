#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

#include "print_messages.h"

void message_handler(int debug_mode, int message_id,char* message,ringStruct* node, int nb)
{
	if(debug_mode == 1)
	{
		if(message_id == 1)
			printf("Finished processing unknown message\n");
		if(message_id == 2)
		{
			printf("Predi Funct\n");
			printf("Finished processing predi\n");
		}
		if(message_id == 3)
		{
			printf("Succi Funct\n");
			printf("Finished processing succi\n");
		}
		if(message_id == 4)
		{
			printf("Add Node Funct\n");
			printf("Finished processing Add Node\n");	
		}
		if(message_id == 5 || message_id == 6 || message_id == 7 || message_id == 8)
		{
			printf("Succi: %d \t Predi: %d\n",node->succiID,node->prediID);
			printf("Succi FD: %d \t Predi FD: %d\n",node->succiFD,node->prediFD);
			printf("Succi TCP: %d \t Predi TCP: %d\n",node->succiPort,node->prediPort);	
		}
		if(message_id == 9)
		{
			printf("My responsability\n");
			printf("Sending to Master %s",message);
		}
		if(message_id == 10)
			printf("Sending to Succi %s",message);
		if(message_id == 11)
			printf("DISTANCE: %d\n", nb);
		if(message_id == 12)
			printf("A analisar: %s\n",message);	
		if(message_id == 13)
			printf("porta %d ocupada, a tentar a próxima\n", nb);
	}
	return;
}