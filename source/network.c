#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "network.h"



/* --------------------------< UDP >--------------------------------- */

int sendUDP(char * msg, int msg_length, socketStruct socketCFG)
{
  return sendto(socketCFG.socketFD,msg,msg_length,0,(struct sockaddr*)socketCFG.addr,socketCFG.addrlen);
}

int recvUDP(char * buffer,socketStruct socketCFG)
{
  memset(buffer,0,128);
  return recvfrom(socketCFG.socketFD,buffer,128,0,(struct sockaddr*)socketCFG.addr,&(socketCFG.addrlen));
}



/* --------------------------< TCP >--------------------------------- */

void sendTCP(char * msg, int msg_length, socketStruct socketCFG)
{
  int nwritten;
  int nleft = strlen(msg);
  while(nleft > 0)
  {
    nwritten = write(socketCFG.socketFD, msg, msg_length);
    nleft -= nwritten;
    msg += nwritten;
  }
}

int recvTCP(char * buffer,socketStruct socketCFG)
{
  memset(buffer,0,128);
  return read(socketCFG.socketFD, buffer, 128);
}

/* --------------------------< Listening >--------------------------------- */

int listenSocket(int listen_port){
  
  int server_socket;
  struct sockaddr_in server_addr;
  
  /* criar socket do servidor */
  if ((server_socket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) 
  {
    perror("impossível criar socket: erro na função socket()\n");
    exit(1);
  }
    
  /* estrutura que contém o endereço IP */
  bzero((char *) &server_addr, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  server_addr.sin_port = htons(listen_port);
  
  /* associa o socket à ringport */  
  while(bind(server_socket, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0)
  {
    printf("porta %d ocupada, a tentar a próxima\n", listen_port);
    listen_port++;
    server_addr.sin_port = htons(listen_port);
  }
  
  printf("port - %d\n", listen_port);
    
  /* socket do servidor a escutar pedidos */
  if (listen(server_socket, MAX_PENDING) < 0) { // fila de espera de escuta pela parte da socket do servidor é de 128 clientes, valor definido pelo MAX_PENDING
    perror("impossível criar socket: erro na função listen()\n");
    exit(1);
  }

  char buffer[128];
  
  gethostname(buffer,128);
  printf("%s\n", buffer);

    return server_socket; // função devolve o file descriptor da socket do servidor
}

int aceita_cliente(int server_socket, char * remote_address)
{

  int client_socket; // file descriptor que irá ser associado a cada cliente
  struct sockaddr_in client_addr;
  unsigned int client_lenght = sizeof(client_addr);
  
  /* espera por ligação do cliente */
  if ((client_socket = accept(server_socket, (struct sockaddr *) &client_addr, &client_lenght)) < 0) 
  { // aceita o pedido do cliente e a cada cliente associa um file descriptor - client_socket
        perror("impossível aceitar ligação do cliente: erro na função client_socket()\n");
    exit(1);
  }
    
  sprintf(remote_address, "%s", inet_ntoa(client_addr.sin_addr));   
    
  return client_socket; // função devolve o file descriptor da socket do cliente
}

/* --------------------------< Tejo Communication >--------------------------------- */

socketStruct setupSocket(char * servidor, int port, char protocol)
{
  struct hostent *h;
  struct in_addr *a;
  int socketFD;
  int ignoreAddr = 0;

  socketStruct socketCFG;

  struct sockaddr_in * addr = (struct sockaddr_in *) malloc(sizeof(struct sockaddr_in));

  /* --------------------------< GetHostByName >--------------------------------- */

  if((h=gethostbyname(servidor))==NULL) //substituir por bootIP
  {
    ignoreAddr = 1; //
  }

  /* --------------------------< SetupSocket >--------------------------------- */
  if(protocol == 'U')
    socketFD = socket(AF_INET,SOCK_DGRAM,0);
  else if(protocol == 'T')
    socketFD = socket(AF_INET,SOCK_STREAM,0);
  else
  {
    printf("protocolo é U (USP), T (TCP)\n");
    exit(-1);
  }

  if(socketFD == -1)
  {
    printf("erro a criar o socket\n");
    exit(1);
  }

  if(ignoreAddr == 0)
  {
    a=(struct in_addr*)h->h_addr_list[0];

    memset((void*)addr,(int)'\0',sizeof(*addr));
    addr->sin_family = AF_INET;
    addr->sin_addr = *a;
    addr->sin_port = htons(port);

    socketCFG.addr = addr;
    socketCFG.addrlen = sizeof(*addr);
  }

  socketCFG.socketFD = socketFD;
  
  if(protocol == 'T')
  {
    int n = connect(socketFD,(struct sockaddr*)addr, sizeof(*addr));
    if(n==-1) exit(1);
  }
  
  return socketCFG;
}

void closeSocket(socketStruct socketCFG)
{
  free(socketCFG.addr);
  close(socketCFG.socketFD);
}