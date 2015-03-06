#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>

typedef struct requestUDP
{
  char request[128];
  int fdUDP;
}requestUDP;

requestUDP* createUDP(char* bootIP, int bootport, int status, char* command, int socketUDP)
{
  struct hostent *h;
  struct in_addr *a;
  struct sockaddr_in addr;
  int fd, n;
  socklen_t addrlen;
  char buffer[128];
  requestUDP* response;

  // 0 inicializaçao do socket para o servidor de arranque
  // 1 envio de uma mensagem para o servidor
  // pode ser melhorado no futuro

  response = (requestUDP*) malloc(sizeof(requestUDP));
/* --------------------------< GetHostByName >--------------------------------- */

  if((h=gethostbyname(bootIP))==NULL) //substituir por bootIP
  {
    printf("\n%s isn't a valid host name \n",bootIP);
    exit(0);//error
  }

  a=(struct in_addr*)h->h_addr_list[0];

/* --------------------------< SendingToUDP >--------------------------------- */
  if(status == 0)
  {
    fd=socket(AF_INET,SOCK_DGRAM,0);
    response->fdUDP = fd;
    if(fd == -1)
    {
      printf("\nCan't create socket\n");
      exit(1);
    }
  }

  memset((void*)&addr,(int)'\0',sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_addr = *a;
  addr.sin_port = htons(bootport);

  if(status == 1)
  {
    response->fdUDP = socketUDP;
    n=sendto(socketUDP,command,sizeof(command),0,(struct sockaddr*)&addr,sizeof(addr));

    if(n==-1)
    {
      printf("\nImpossible to send message to the socket\n");
      exit(1);
    }

  /* ----------------------------< ReceivingFromUDP >------------------------------- */

    addrlen = sizeof(addr);

    n = recvfrom(socketUDP,buffer,128,0,(struct sockaddr*)&addr,&addrlen);

    if(n==-1)
    {
      printf("\nCan't create socket\n");
      exit(3);//error
    }

    write(1,"> [server]: ",12);//stdout
    write(1,buffer,n);
    printf("\n");
    strcpy(response->request,buffer);
  }
  return response;
}

/*requestUDP* createTCP(char* bootIP, int bootport, int status, char* command, int socketUDP) //rever tudo
{
  int fd,addrlen,newfd;
  struct sockaddr_in addr;
  int n,nw;
  char* ptr, buffer[128];

  if((fd = socket(AF_INET,SOCK_STREAM,0)) ==-1)
    exit(1); //error

  memset((void*)&addr,(int)'\0',sizeof(addr));
  addr.sin_family=AF_INET;
  addr.sin_addr.s_addr=htonl(INADDR_ANY);
  addr.sin_port=htons(9000);

  if(bind(fd,(struct sockaddr*)&addr,sizeof(addr))==-1)
    exit(1);//error

  if(listen(fd,5)==-1)
    exit(1); //error

  while(1)
  {
    addrlen=sizeof(addr);

    if((newfd=accept(fd,(struct sockaddr*)&addr,&addrlen))==-1)
      exit(1);//error

    while((n=read(newfd,buffer,128))!=0)
    {
      if(n==-1)
        exit(1);//error

      ptr=&buffer[0];

      while(n>0)
      {
        if((nw=write(newfd,ptr,n))<=0)
        exit(1);//error
        n-=nw;
        ptr+=nw;
      }
    }
    close(newfd);
  }
  close(fd);
  exit(0);
}*/
