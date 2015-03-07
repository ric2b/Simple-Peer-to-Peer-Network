#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>


int main(int argc, char ** argv)
{
  struct hostent *h;
  struct in_addr *a;
  struct sockaddr_in addr;
  int fd, n;
  char buffer[128];
  char host[128];  
  char message[128];
  int port;

  if(argc != 3)
  {
    printf("Introduza o host e a porta\n");
    exit(-1);
  }

  strcpy(host, argv[1]);
  sscanf(argv[2], " %d ", &port);

/* --------------------------< GetHostByName >--------------------------------- */

  if((h=gethostbyname(host))==NULL) //substituir por bootIP
  {
    exit(0);//error
  }

  a=(struct in_addr*)h->h_addr_list[0];

/* --------------------------< SendingToUDP >--------------------------------- */

  fd=socket(AF_INET,SOCK_DGRAM,0);

  if(fd == -1)
  {
    exit(1);
  }

  socklen_t addrlen = sizeof(addr);

  while(1)
  {
    memset((void*)&addr,(int)'\0',addrlen);
    addr.sin_family = AF_INET;
    addr.sin_addr = *a;
    addr.sin_port = htons(port);

    printf("> ");
    fgets(message, 128, stdin);
    n=sendto(fd,message,strlen(message),0,(struct sockaddr*)&addr,addrlen);

    if(n==-1)
    {
      printf("erro a enviar\n");
      exit(2);
    }

    memset(buffer, 0, 128);
    n = recvfrom(fd,buffer,128,0,(struct sockaddr*)&addr,&addrlen);

    if(n==-1)
    {
      printf("erro a receber\n");
      exit(3);//error
    }

    printf("[server]: %s\n", buffer);


  }
  close(fd);
  exit(4);
}
