#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "interface.h"
//#include "network.h"
#include "ringOps.h"

#define STDIN 0

int main(int argc, char **argv)
{
	char  	bootIP[1024];
	int		bootport, ringport;
	char 	userInput[64], cmd[20],succiIP[70], buffer[128];
	char	option;
	int 	exitProgram, identifier, ringx, succi, succiTCP;
	int 	listenFD = 8080;
	socketStruct socketCFG_UDP;

	check_arguments(argc, argv, bootIP, & bootport, & ringport, & option);
	char clientIP[128];
	listenFD = listenSocket(&ringport);
	socketCFG_UDP = setupSocket(bootIP, bootport,'U');
	fd_set fds;	// isto são tretas para o select
	int maxfd;
	while(1)
	{	//bloqueia no select até haver algo para ler num dos sockets que estão em fds
		FD_ZERO(&fds);
		FD_SET(listenFD, &fds); //adiciona o socket de escuta a fds
		FD_SET(STDIN, &fds); // stdin
		maxfd = (listenFD > STDIN) ? listenFD : STDIN; //calcular maxfd
		//printf("Waiting to select...\n");
		if (select(maxfd+1, &fds, NULL, NULL, NULL) > 0) {
			memset(buffer,0,128);
			if(FD_ISSET(STDIN, &fds))
			{
				run_commands(userInput, cmd, succiIP, & exitProgram, & identifier, & ringx, & succi, & succiTCP, ringport, socketCFG_UDP);
				//read(STDIN, buffer, 128);
				//printf("teclado: %s", buffer);
			}
			if(FD_ISSET(listenFD, &fds))
			{
				int nodeFD = aceita_cliente(listenFD, clientIP); // cria um novo socket de comunicação para o nó cliente
				// the usual stuff
				read(nodeFD, buffer, 128);
				//write(nodeFD, "OK", 2);
				printf("Peer received: %s\n", buffer);
				close(nodeFD); // fecha o file descriptor do nó cliente
			}
		}
		//printf("Looping...\n");
	}

	/*do
	{
		ringx = 0;
   		identifier = -1;
   		succi = -1;
   		succiTCP = -1;
		memset(succiIP,0,70);
	} while(0 == run_commands(userInput, cmd, succiIP, & exitProgram, & identifier, & ringx, & succi, & succiTCP));
	*/
	//closeSocket(socketCFG);
  	exit(0);
}
