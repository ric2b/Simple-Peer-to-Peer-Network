#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>

//#include "configuration.h"

int check_arguments(int argc, char **argv, char* bootIP, int * bootport, int* ringport, int * option)
{
  	//bootIP and bootport are the IP adress e UDP port of the starting server

  	//Ringport is the TCP server port used for establishing a TCP session in the ring
	
	if(argc != 7 || argc <= 1)
	{
		printf("\nThe program doesn't have enough arguments or it has more arguments than it can handle.\n\n");
		printf("Invoke the program as the following: ./ddt -t ringport -i bootIP -p bootport\n\n");
		exit(1);
	}

	while((*option = getopt(argc,argv,"t:i:p:")) != -1) // Checking the various options received on the console application
	{
		 switch(*option)
		 {
			case 't':
				*ringport = atoi(optarg); // Get the argument corresponding to this case and converts it from the string format to integer type
				break;

			case 'i':
				bootIP = optarg;
				break;

			case 'p':
				*bootport = atoi(optarg);
				break;

			default:
				printf("The expected arguments don't have the proper format. Application Aborted.\n");
				exit(2);
		 }
	}
	return 0;
}
