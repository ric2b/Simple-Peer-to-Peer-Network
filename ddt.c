#include <stdio.h>
#include <stdlib.h>


int main(int argc, char argv)
{
  //bootIP and bootport are the IP adress e UDP port of the starting server.

  //Ringport is the TCP server port used for establishing a TCP session in the ring

  char bootIP[1000];
  int bootport,ringport,i;

  //By default, bootIP="tejo.ist.utl.pt" and bootport=58000
  bootIP="tejo.ist.utl.pt/0";
  bootport=58000;
  i=1;


  while(i<argc)
  {
    switch(argv[i])
    {
      case "-t":
        ringport=argv[i+1];
      break;

      case "-i":
        bootIP=argv[i+1];
      break;

      case "-p":
        bootport=argv[i+1];
      break;

      default:
        printf("The expected arguments don't have the proper format. Application Aborted.");
        exit(1);
    }
    i+=2;
  }



}
