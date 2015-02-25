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
	
	struct sockaddr_in addr;
	struct in_addr *a;
	struct hostent *h;
	
	int fd,n,addrlen;
	
		
	
	if((h = gethostbyname("tejo"))==NULL)
		exit(1);

	a = (struct in_addr*)h->h_addr_list[0];
	
	fd=socket(AF_INET,SOCK_STREAM,0);
	if(fd==-1)
	{
		exit(1);
	}

	addrlen=sizeof(addr);
	
	memset((void*)&addr,(int)'\0',addrlen);

	addr.sin_family = AF_INET;
	addr.sin_addr = *a;
	addr.sin_port = htons(8000);
	
	n=connect(fd,(struct sockaddr*)&addr,addrlen);
	
	if(n==-1) exit(1);
	
	
	
	close(fd);
	exit(0);
	
}
