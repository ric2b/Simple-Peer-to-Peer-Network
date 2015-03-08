#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "interface.h"
#include "network.h"
#include "ringOps.h"

int main(int argc, char **argv)
{
	char  	bootIP[1024];
	int		bootport, ringport;
	char 	userInput[64], cmd[20],succiIP[70], buffer[128];
	char	option;
	int 	exitProgram, identifier, ringx, succi, succiTCP;
	int 	listenFD = 8080;

	//By default, bootIP="tejo.ist.utl.pt" and bootport=58000
	strcpy(bootIP,"tejo.ist.utl.pt");
	bootport = 58000;
	check_arguments(argc, argv, bootIP, & bootport, & ringport, & option);


	char clientIP[128];
	listenFD = listenSocket(ringport);	

	fd_set fds;	// isto são tretas para o select
	FD_ZERO(&fds);
	FD_SET(listenFD, &fds); //adiciona o socket de escuta a fds

	while(1)
	{	//bloqueia no select até haver algo para ler num dos sockets que estão em fds
		if (select(listenFD+1, &fds, NULL, NULL, NULL) > 0) {

			int nodeFD = aceita_cliente(listenFD, clientIP); // cria um novo socket de comunicação para o nó cliente
			
			memset(buffer,0,128); // the usual stuff
			read(nodeFD, buffer, 128);
			write(nodeFD, "OK", 2);
			printf("%s: %s", clientIP, buffer);
			
			close(nodeFD); // fecha o file descriptor do nó cliente
		}
	}

	do
	{
		ringx = 0;
   		identifier = -1;
   		succi = -1;
   		succiTCP = -1;
		memset(succiIP,0,70);
	} while(0 == run_commands(userInput, cmd, succiIP, & exitProgram, & identifier, & ringx, & succi, & succiTCP));

	//closeSocket(socketCFG);
  	exit(0);
}
