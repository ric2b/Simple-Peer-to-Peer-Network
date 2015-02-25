//lab1.c

#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <sys/socket.h>
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

	if((h = gethostbyname("www.google.com"))==NULL)
	{
		exit(1);
	}

	printf("official host name: %s\n", h->h_name);

	a = (struct in_addr*)h->h_addr_list[0];
	printf("internet address: %s (%08lX)\n", inet_ntoa(*a), (long unsigned int)ntohl(a->s_addr));
	exit(0);
}
