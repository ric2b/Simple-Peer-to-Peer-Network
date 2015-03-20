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
	char  	clientIP[128];
	int		bootport, ringport;
	char 	buffer[128];
	char	option;
	int 	listenFD = 8080;
	int  	master_socket;
	socketStruct socketCFG_UDP;
	ringStruct node;

	Node_Initialization(&node);

	check_arguments(argc, argv, bootIP, & bootport, & ringport, & option);

	listenFD = listenSocket(&ringport);
	node.myPort = ringport;

	socketCFG_UDP = setupSocket(bootIP, bootport,'U');
	GetIP(&node);

	printf("\n> ");
	fflush(stdout);

	fd_set fds;	// isto são tretas para o select
	int maxfd;
	while(1)
	{	//bloqueia no select até haver algo para ler num dos sockets que estão em fds

		FD_ZERO(&fds);
		FD_SET(listenFD, &fds); //adiciona o socket de escuta a fds
		FD_SET(STDIN, &fds);  // stdin
		FD_SET(node.prediFD,&fds);
		FD_SET(node.succiFD,&fds);
		FD_SET(master_socket,&fds);
		maxfd = (listenFD > STDIN) ? listenFD : STDIN; //calcular maxfd
		maxfd = (node.prediFD > maxfd) ? node.prediFD : maxfd; //calcular maxfd
		maxfd = (node.succiFD > maxfd) ? node.succiFD : maxfd; //calcular maxfd
		maxfd = (master_socket > maxfd) ? master_socket : maxfd; //calcular maxfd
		//printf("Waiting to select...\n");
		if (select(maxfd+1, &fds, NULL, NULL, NULL) > 0)
		{
			memset(buffer,0,128);

			/* Comando do Utilizador*/
			if(FD_ISSET(STDIN, &fds))
			{
				master_socket = run_commands(&node, socketCFG_UDP, &node);
			}

			/* Mensagem de desconhecido */
			if(FD_ISSET(listenFD, &fds))
			{
				printf("Servidor: %d\n",listenFD);
				int nodeFD = aceita_cliente(listenFD, clientIP); // cria um novo socket de comunicação para o nó cliente
				read(nodeFD, buffer, 128);
				if(JR_Message(buffer,&node,nodeFD) == 1)
				{
					printf("A fechar socket!\n");
					close(nodeFD); // fecha o file descriptor do nó cliente
				}
				printf("Finished processing\n");
			}

			/* Mensagem do Predi*/
			if(FD_ISSET(node.prediFD,&fds))
			{
				read(node.prediFD, buffer, 128);
				printf("Predi Funct\n");
				if(JR_Message(buffer,&node,node.prediFD) == 1)
				{
					printf("A fechar predi socket!\n");
					close(node.prediFD); // fecha o file descriptor do nó cliente
				}
				printf("Finished processing predi\n");				
			}

			/* Mensagem do Succi*/
			if(FD_ISSET(node.succiFD,&fds))
			{
				read(node.succiFD, buffer, 128);

				printf("Succi Funct\n");
				if(JR_Message(buffer,&node,node.succiFD) == 1)
				{
					printf("A fechar succi socket!\n");
					close(node.succiFD); // fecha o file descriptor do nó cliente
				}
				printf("Finished processing succi\n");	
				
			}
			/* Mensagem para No Mestre - Comunicaçao inicial */
			if(FD_ISSET(master_socket,&fds))
			{
				read(master_socket, buffer, 128);

				printf("Add Node Funct\n");
				if(JR_Message(buffer,&node,master_socket) == 1)
				{
					printf("A fechar master socket!\n");
					close(master_socket); // fecha o file descriptor do nó cliente
				}
				printf("Finished processing Add Node\n");	
				
			}
		}
	}
  exit(0);
}
