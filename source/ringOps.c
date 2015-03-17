#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "ringOps.h"
#include "network.h"

void GetIP(ringStruct* node)
{
	char localmachine[128];
	struct hostent *h;
  struct in_addr *a;

	if(gethostname(localmachine,128) == -1)
    {
      printf("\nError during hostname query\n\n");
      exit(1);
    }
  if((h=gethostbyname(localmachine))==NULL)
    {
      printf("\nError during hostname query\n\n");
      exit(1);//error
    }

  printf("Hostname: %s\n",localmachine);
  a = (struct in_addr*)h->h_addr_list[0];

  printf("internet address: %s (%08lX)\n", inet_ntoa(*a), (long unsigned int)ntohl(a->s_addr));

  strcpy(node->myIP,inet_ntoa(*a));
}


void Node_Initialization(ringStruct* node)
{
	node->ringID = -1;
	node->myID = -1;
  strcpy(node->myIP,"\0");
  node->myPort = -1;
  node->succiID = -1;
  strcpy(node->succiIP,"\0");
  node->succiPort = -1;
  node->succiFD = -1;
  node->prediID = -1;
  strcpy(node->prediIP,"\0");
  node->prediPort = -1;
  node->prediFD = -1;
}

int JR_Message(char* request,ringStruct* node, int nodeFD)
{
	char cmd[128], msg[128], ip[128];
	int no_arrq, no_novo, no_dest, tcp;
	printf("A analisar: %s",request);


	if(sscanf(request,"%s %d %d %d %s %d",cmd,&no_arrq,&no_novo,&no_dest,ip,&tcp)!= 5)
	{
		//printf("Comando 1: %s %d\n ",cmd,(int)strlen(cmd));
		if(sscanf(request,"%s %d %s %d",cmd,&no_novo,ip,&tcp)!= 4)
		{
  			//printf("Comando 2: %s %d\n ",cmd,(int)strlen(cmd));
  			if(sscanf(request,"%s %d",cmd,&no_novo)!= 2)
  			{
    				printf("Bad Message 5\n");
    				return 1;
  			}
  			else
  			{
    				if(strcmp(cmd,"ID") == 0)
    				{
      					if(node->succiID == -1 && node->prediFD == -1)
      					{
                    memset(msg,0,128);
      							sprintf(msg,"SUCC %d %s %d\n",node->myID, node->myIP, node->myPort);
      							printf("%s",msg);
      							sendTCPv2(msg,strlen(msg), nodeFD);
                    printf("1\n");
                    printf("Succi: %d \t Predi: %d\n",node->succiID,node->prediID);
                    printf("Succi FD: %d \t Predi FD: %d\n",node->succiFD,node->prediFD);
                    printf("Succi TCP: %d \t Predi TCP: %d\n",node->succiPort,node->prediPort);
      							return 0;
      					}
                else
                {
                    memset(msg,0,128);
                    sprintf(msg,"QRY %d %d\n",node->myID,no_novo);
                    printf("%s",msg);
                    sendTCPv2(msg,strlen(msg), node->succiFD);
                    printf("2\n");
                    printf("Succi: %d \t Predi: %d\n",node->succiID,node->prediID);
                    printf("Succi FD: %d \t Predi FD: %d\n",node->succiFD,node->prediFD);
                    printf("Succi TCP: %d \t Predi TCP: %d\n",node->succiPort,node->prediPort);
                    return 0;
                }
            }
    			 else
    			 {
    				  printf("Bad Message 4\n");
    				  return 1;
    			 }
  			}
		}
		else
		{
			if(strcmp(cmd,"NEW") == 0)
			{
				if(node->succiID == -1 && node->prediFD == -1)
        {
          node->prediID = no_novo;
  				strcpy(node->prediIP,ip);
  				node->prediPort = tcp;
  				node->prediFD = nodeFD;
          node->succiID = no_novo;
          strcpy(node->succiIP,ip);
          node->succiPort = tcp;
          node->succiFD = nodeFD;
          printf("3\n");
          printf("Succi: %d \t Predi: %d\n",node->succiID,node->prediID);
          printf("Succi FD: %d \t Predi FD: %d\n",node->succiFD,node->prediFD);
          printf("Succi TCP: %d \t Predi TCP: %d\n",node->succiPort,node->prediPort);
          return 0;
        }else
        {
          node->prediID = no_novo;
          strcpy(node->prediIP,ip);
          node->prediPort = tcp;
          node->prediFD = nodeFD;
          printf("4\n");
          printf("Succi: %d \t Predi: %d\n",node->succiID,node->prediID);
          printf("Succi FD: %d \t Predi FD: %d\n",node->succiFD,node->prediFD);
          printf("Succi TCP: %d \t Predi TCP: %d\n",node->succiPort,node->prediPort);
          return 0;
        }
			}
			else
			{
				printf("Bad Message 7\n");
				return 1;
			}
		}
	}
	else
	{
		//printf("Comando 3: %s %d\n ",cmd,(int)strlen(cmd));
		if(strcmp(cmd,"RSP") == 0)
		{
				sprintf(msg,"SUCC %d %s %d\n",no_dest, ip, tcp);
				printf("%s\n",msg);
				sendTCPv2(msg,strlen(msg), nodeFD);
				return 0;
		}
		else
		{
			printf("Bad Message 6\n");
			return 1;
		}
	}

	return 1;
}

void Join_Ring(ringStruct* node, socketStruct start)
{
  char msg[128];
  char buffer[128];
  char localmachine[128];
  char cmd[10],idIP[20];
  int tmpid, tmpport;
  char tmpip[128];
  int startid,startTCP,ringx;
  struct hostent *h;
  struct in_addr *a;
  int temp;
  socketStruct PeerTCP;


  sprintf(msg,"BQRY %d",node->ringID);
  printf("Command sent: %s\n",msg);
  printf("Socket: %d\n",start.socketFD);
  if((temp = sendUDP(msg,strlen(msg),start)) == -1)
    exit(1);
  if((temp = recvUDP(buffer,start)) == -1)
    exit(1);

  printf("Command recieved: %s\n",buffer);

  if(strcmp(buffer,"EMPTY") == 0)
  {
    if(gethostname(localmachine,128) == -1)
    {
      printf("\nError during hostname query\n\n");
      exit(1);
    }
    if((h=gethostbyname(localmachine))==NULL)
      {

        exit(1);//error
      }
    printf("Hostname: %s\n",localmachine);
    a = (struct in_addr*)h->h_addr_list[0];

    sprintf(msg,"REG %d %d %s %d\n",node->ringID, node->myID,inet_ntoa(*a), node->myPort);
    strcpy(node->myIP,inet_ntoa(*a));
    printf("%s\n",msg);

    if((temp = sendUDP(msg,strlen(msg),start)) == -1)
      exit(1);
    if((temp = recvUDP(buffer,start)) == -1)
      exit(1);

    printf("Temp: %s\n",buffer);

    if(strcmp(buffer,"OK") == 0)
      return;
  }
  else
  {
    if(sscanf(buffer,"%s %d %d %s %d",cmd,&ringx,&startid,idIP,&startTCP) != 5)
    {
      printf("Bad Response from start server\n");
      exit(1);
    }
    else
    {
      while(startid == node->myID)
      {
		    printf("Can't use identifier %d, please choose a different one: ",node->myID);
        scanf("%d",&(node->myID));
       }
      printf("IP: %s\nPort: %d\n",idIP,startTCP);

 // vai se ligar ao no de arranque
 // enviando ID i

      PeerTCP = setupSocket(idIP, startTCP, 'T');
      memset((void*)&msg,'\0',sizeof(msg));
      sprintf(msg,"ID %d\n", node->myID);
      printf("Sending 1 %s",msg);
      sendTCP(msg,strlen(msg),PeerTCP);
      memset(buffer,0,128);

      if(recvTCP(buffer,PeerTCP)<0)
  	  {
    		printf("Can't receive message from TCP connection\n");
    		exit(1);
  	  }
  	  closeSocket(PeerTCP);
  	  printf("TCP received: %s\n",buffer);

      // Message after query through the ring
  	  if(sscanf(buffer,"%s %d %s %d",cmd, &tmpid, tmpip, &tmpport) != 4)
  	  {
    		printf("Bad Message 2\n");
    		exit(1);
  	  }

	     while(tmpid == node->myID)
      {
		    printf("Can't use identifier %d, please choose a different one: ",node->myID);
        scanf("%d",&(node->myID));
      }

      if(strcmp(cmd,"SUCC") != 0)
  	  {
    		printf("Bad Message 3\n");
    		exit(1);
	    }

	    memset((void*)&msg,'\0',sizeof(msg));
      // Sends message to l informing him that i'm his predi
      // l is now my succi and information is updated 
      sprintf(msg,"NEW %d %s %d\n", node->myID, node->myIP, node->myPort);
      printf("Sending to %d, message %s",PeerTCP.socketFD, msg);
      PeerTCP = setupSocket(tmpip,tmpport,'T');
      sendTCP(msg,strlen(msg),PeerTCP);
      //printf("Sent\n");
      node->succiFD = PeerTCP.socketFD;
      strcpy(node->succiIP,tmpip);
      node->succiID = tmpid;
      node->succiPort = tmpport;
      //memset(buffer,0,128);
      return;
    }
  }
  return;
}


//socketStruct setupListenSocket(char * myIP, int myPort)

int removeNode(ringStruct * ringData, socketStruct socketCFG, socketStruct succiPeer, socketStruct prediPeer)
{
  //getting out with return 0, everything went fine
  //getting out with return 1, something went wrong
  char msg[128], buffer[128];
  if(ringData->succiID == -1 && ringData->prediID == -1) // If the node is unique remove the ring from the server
  {
    sprintf(msg,"UNR %i\n",ringData->ringID);
    if(sendUDP(msg,strlen(msg),socketCFG) == -1)
      return 1;
    if(recvUDP(buffer,socketCFG) == -1)
      return 1;
    else if(strcmp(buffer,"OK")==0)
      return 0;
    else
      return 1;
  }
  else
  {
    if(ringData->starter == 1) // Test to check if the current node is the starter node. If it is, put the next node as the starter one
    {
      sprintf(msg,"REG %i %i %s %i\n", ringData->ringID, ringData->succiID, ringData->succiIP, ringData->succiPort);
      if(sendUDP(msg,strlen(msg),socketCFG) == -1)
        return 1;
      if(recvUDP(buffer,socketCFG) == -1)
        return 1;
      else if(strcmp(buffer,"OK")==0)
      {
        memset(msg,0,strlen(msg));
        sprintf(msg,"BOOT\n");
        //This part is not well done
        sendTCP(msg, strlen(msg), succiPeer);
        closeSocket(succiPeer);
        memset(msg,0,strlen(msg));
        sprintf(msg,"CON %i %s %i\n", ringData->succiID, ringData->succiIP, ringData->succiPort);
        sendTCP(msg, strlen(msg), prediPeer);
        closeSocket(prediPeer);
        return 0;
      }
      else // Default Case
      {
        closeSocket(succiPeer);
        memset(msg,0,strlen(msg));
        sprintf(msg,"CON %i %s %i\n", ringData->succiID, ringData->succiIP, ringData->succiPort);
        sendTCP(msg, strlen(msg), prediPeer);
        closeSocket(prediPeer);
        return 0;
      }
    }
    else
      return 1;
  }
  return 1;
}

int distance(int k, int l)
{ // if it returns -1 something wrong occured
  int res;
  if(l >= k)
    res = l-k;
  else if(l < k)
    res = 64 + l - k;
  else
    res = -1;
  return res;
}

int responsability(int predi, int i, int k)
{ // returns 1 if succi is responsible for k, 0 otherwise.
  if(distance(k,i) < distance(k,predi))
    return 1;
  else
    return 0;
}

int searchNode(ringStruct * ringData, socketStruct succiPeer, int k)
{ // returns 0 if everything went as expected
  char msg[128], buffer[128], cmd[10], qryIP[40];
  int asked, queried, qryID, qryTCP;
  if(responsability(ringData->prediID,ringData->succiID,k) == 1)
  {
    printf("%i %s %i", ringData->succiID, ringData->succiIP, ringData->succiPort);
    return 0;
  }
  else
  {
    sprintf(msg,"QRY %i %i\n", ringData->myID, k);
    sendTCP(msg, strlen(msg), succiPeer);
    recvTCP(buffer, succiPeer);
    sscanf(buffer,"%s %i %i %i %s %i", cmd, &asked, &queried, &qryID, qryIP, &qryTCP);
    if(strcmp(cmd,"RSP") == 0 && asked == ringData->myID && queried == k)
    {
      printf("%i %s %i", qryID, qryIP, qryTCP);
      return 0;
    }
    else
      return 1;
  }
  return 1;
}

int showNode(ringStruct * ringData)
{
  printf("%i %i %i %i", ringData->ringID, ringData->myID, ringData->succiID, ringData->prediID);
  return 0;
}

/*
int joinRing_KnownSucci(ringStruct * ringData, int ringID, int myID, int succiID, char * succiIP, int succiPort)
{
  ringData->succiID = succiID;
  strcpy(ringData->succiIP, succiIP);
  ringData->succiPort = succiPort;
  char joinCommand[128*2]; //para aguentar com os 128 do IP + extras
  sprintf(joinCommand, "NEW %d %s %d", myID, ringData->myIP, ringData->myPort);
  if(connect(ringData->ListenSocket.socketFD, (struct sockaddr*)ringData->ListenSocket.addr, ringData->ListenSocket.addrlen) == -1)
  {
    printf("erro a fazer connect na joinRing_KnownSucci\n");
    exit(-1);
  }
  //socketStruct setupSocket(succiIP, succiPort, 'T');
  void sendTCP(char * msg, int msg_length, socketStruct socketCFG);
  int recvTCP(char * buffer,socketStruct socketCFG);
  return 0;
}
*/
