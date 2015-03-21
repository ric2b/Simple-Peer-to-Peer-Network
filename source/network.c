#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <assert.h>

#include "network.h"


/* --------------------------< UDP >--------------------------------- */

int sendUDP(char * msg, socketStruct socketCFG)
{
  return sendto(socketCFG.socketFD,msg, strlen(msg),0,(struct sockaddr*)socketCFG.addr,socketCFG.addrlen);
}

int recvUDP(char * buffer,socketStruct socketCFG)
{
  memset(buffer,0,128);
  return recvfrom(socketCFG.socketFD,buffer,128,0,(struct sockaddr*)socketCFG.addr,&(socketCFG.addrlen));
}

/* --------------------------< TCP >--------------------------------- */
void sendTCP(char * msg, int socket)
{
  int nwritten;
  int nleft = strlen(msg);
  while(nleft > 0)
  {
    nwritten = write(socket, msg, strlen(msg));
    nleft -= nwritten;
    msg += nwritten;

    if(nwritten == -1)
    {
      printf("erro a enviar com sendTCP\n");
    }
  }
}

void recvTCP(char * buffer, socketStruct socketCFG)
{
  int nreceived;
  int nleft = 128;

  memset(buffer,0,128);
  while(nleft > 0)
  {
      nreceived = read(socketCFG.socketFD, buffer, nleft);
      nleft -= nreceived;
      buffer += nreceived;
      if(nreceived == -1)
      {
          printf("erro a receber com sendTCP\n");
          exit(-1);
      }
      else if(nreceived == 0) // acabou de receber
      {
          break;
      }
  }
}

/* --------------------------< Listening >--------------------------------- */
int listenSocket(int* listen_port)
{
  int listen_socket;
  struct sockaddr_in server_addr;
  const int       optVal = 1;
  const socklen_t optLen = sizeof(optVal);
  /* criar socket do servidor */
  if ((listen_socket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
  {
    perror("impossível criar socket: erro na função socket()\n");
    exit(1);
  }
  /* estrutura que contém o endereço IP */
  bzero((char *) &server_addr, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  server_addr.sin_port = htons(*listen_port);
  setsockopt(listen_socket, SOL_SOCKET, SO_REUSEADDR, (void*) &optVal, optLen);
  /* associa o socket à ringport */
  while(bind(listen_socket, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0)
  {
    printf("porta %d ocupada, a tentar a próxima\n", *listen_port);
    (*listen_port)++;
    server_addr.sin_port = htons(*listen_port);
  }
  printf("port - %d\n", *listen_port);
  /* socket do servidor a escutar pedidos */
  if (listen(listen_socket, MAX_PENDING) < 0) { // fila de espera de escuta pela parte da socket do servidor é de 128 clientes, valor definido pelo MAX_PENDING
    perror("impossível criar socket: erro na função listen()\n");
    exit(1);
  }
  /*
  char buffer[128];
  gethostname(buffer,128);
  printf("%s\n", buffer);
  */
  return listen_socket; // função devolve o file descriptor da socket do servidor
}

int aceita_cliente(int server_socket, char * remote_address)
{
  int client_socket; // file descriptor que irá ser associado a cada cliente
  struct sockaddr_in client_addr;
  socklen_t client_lenght = sizeof(client_addr);
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

socketStruct setupSocket(char * destinatario, int port, char protocol)
{
  struct hostent *h;
  struct in_addr *a;
  int socketFD;
  int ignoreAddr = 0;
  const int       optVal = 1;
  const socklen_t optLen = sizeof(optVal);
  socketStruct socketCFG;

  struct sockaddr_in * addr = (struct sockaddr_in *) malloc(sizeof(struct sockaddr_in));

  /* --------------------------< GetHostByName >--------------------------------- */

  if((h=gethostbyname(destinatario))==NULL) //substituir por bootIP
  {
	printf("gethostbyname falhou na setupSocket, a continuar na mesma\n");
    ignoreAddr = 1; //
  }

  /* --------------------------< SetupSocket >--------------------------------- */
  if(protocol == 'U')
    socketFD = socket(AF_INET,SOCK_DGRAM,0);
  else if(protocol == 'T')
    socketFD = socket(AF_INET,SOCK_STREAM,0);
  else
  {
    printf("protocolo é U (USP) ou T (TCP)\n");
    exit(-1);
  }

  if(socketFD == -1)
  {
    printf("erro a criar o socket\n");
    exit(-1);
  }
  
  socketCFG.socketFD = socketFD;

  setsockopt(socketFD, SOL_SOCKET, SO_REUSEADDR, (void*) &optVal, optLen);

  if(ignoreAddr != 1) // se gethostbyname falhou, dnão mexer no endereco
  {
    a=(struct in_addr*)h->h_addr_list[0];

    memset((void*)addr,(int)'\0',sizeof(*addr));
    addr->sin_family = AF_INET;
    addr->sin_addr = *a;
    addr->sin_port = htons(port);

    socketCFG.addr = addr;
    socketCFG.addrlen = sizeof(*addr);
  }

  

  if(protocol == 'T')
  {
    int n = connect(socketFD,(struct sockaddr*)addr, sizeof(*addr));
    if(n==-1)
    {
      printf("Erro no connect a %c\n", protocol);
      exit(1);
    }
  }

  return socketCFG;
}

void closeSocket(socketStruct socketCFG)
{
  free(socketCFG.addr);
  close(socketCFG.socketFD);
}
