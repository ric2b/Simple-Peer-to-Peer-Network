//lab1.c

#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
extern int errno;

int main(void)
{	
	struct hostent *h;
	struct in_addr *a;
	struct sockaddr_in addr;
	int fd,n,addrlen;
	char buffer[128];

	if((h = gethostbyname("tejo"))==NULL)
		exit(1);

	a = (struct in_addr*)h->h_addr_list[0];
	
	fd=socket(AF_INET,SOCK_DGRAM,0);
	if(fd==-1)
	{
		exit(1);
	}

	addrlen=sizeof(addr);
	
	memset((void*)&addr,(int)'\0',addrlen);

	addr.sin_family = AF_INET;
	addr.sin_addr = *a;
	addr.sin_port = htons(8000);
	
	n=sendto(fd,"Hello!\n",7,0,(struct sockaddr*)&addr,addrlen);
	
	if(n==-1) exit(1);
	
	
	n=recvfrom(fd,buffer,128,0,(struct sockaddr *)&addr,&addrlen);

	if(n==-1) exit(1);

	write(1,"echo: ",6);
	write(1,buffer,n);

	close(fd);
	exit(0);
	
}
