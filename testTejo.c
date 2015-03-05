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
  int fd, n, addrlen;
  char buffer[128];

  int i;
  char arguments[1024];

  if(argc <= 1)
  { 
    printf("Introduza a mensagem a enviar ao tejo\n");
    exit(-1);
  }

  strcpy(arguments, argv[1]);
  for (i = 2; i < argc; ++i)
  {
    strcat(arguments, " ");
    strcat(arguments, argv[i]);
  }

/* --------------------------< GetHostByName >--------------------------------- */

  if((h=gethostbyname("tejo.tecnico.ulisboa.pt"))==NULL) //substituir por bootIP
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

  memset((void*)&addr,(int)'\0',sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_addr = *a;
  addr.sin_port = htons(58000);

  n=sendto(fd,arguments,strlen(arguments),0,(struct sockaddr*)&addr,sizeof(addr));

  if(n==-1)
  {
  exit(2);
  }

/* ----------------------------< ReceivingFromUDP >------------------------------- */

  addrlen = sizeof(addr);

  n = recvfrom(fd,buffer,128,0,(struct sockaddr*)&addr,&addrlen);

  if(n==-1)
  {
  exit(3);//error
  }


  write(1,"> [server]: ",12);//stdout
  write(1,buffer,n);
  printf("\n");

  close(fd);
  exit(4);


}
