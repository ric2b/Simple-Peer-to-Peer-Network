#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void msgline(int code)
{
  switch(code)
  {
    case '0':
      printf("The function doesn't have enough arguments or it has more arguments than it can handle.\n");
      break;

    case '1':
      printf("The expected arguments don't have the proper format. Application Aborted.\n");
      break;

    case '2':
      printf("\n");
      printf("Welcome to your favorite p2p client! You have chosen the following specifications: \n");
      printf("\n");
      break;

    case '3':
      printf("\n");
      printf("Type 'help' to show the available commands.\n");
      printf("\n");
      break;

    case '4':
      printf("\n");
      printf("You have closed the application.\n");
      printf("\n");
      break;

    case '5':
      printf("\n");
      printf("You removed your node from the current ring.\n");
      printf("\n");
      break;

    case '6':
      printf("\n");
      printf("Searching the identifier and localization of the node responsible for the chosen identifier.\n");
      printf("\n");
      break;

    case '7':
      printf("\n");
      printf("Joining stuff!\n");
      printf("\n");
      break;

    case '8':
      printf("\n");
      printf("The command you have inserted is non existent.\n");
      printf("\n");
      printf("Type 'help' to show the available commands.\n");
      printf("\n");
      break;
  }
}
