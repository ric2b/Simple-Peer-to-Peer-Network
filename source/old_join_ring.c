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
  if((temp = sendUDP(msg, start)) == -1)
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
    //printf("Hostname: %s\n",localmachine);
    a = (struct in_addr*)h->h_addr_list[0];
    strcpy(node->myIP,inet_ntoa(*a));
    sprintf(msg,"REG %d %d %s %d\n",node->ringID, node->myID, node->myIP, node->myPort);
    printf("%s\n",msg);

    if((temp = sendUDP(msg,start)) == -1)
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
	  node->starter = startid;
      PeerTCP = setupSocket(idIP, startTCP, 'T');
      memset((void*)&msg,'\0',sizeof(msg));
      sprintf(msg,"ID %d\n", node->myID);
      printf("Sending 1 %s",msg);
      sendTCP(msg,PeerTCP.socketFD);
      memset(buffer,0,128);

	  printf("Receive 1\n");
      recvTCP(buffer,PeerTCP);

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
	  closeSocket(PeerTCP);
	  memset((void*)&msg,'\0',sizeof(msg));
      // Sends message to l informing him that i'm his predi
      // l is now my succi and information is updated
      sprintf(msg,"NEW %d %s %d\n", node->myID, node->myIP, node->myPort);
      printf("Sending to %d, message %s",PeerTCP.socketFD, msg);
      PeerTCP = setupSocket(tmpip,tmpport,'T');
      sendTCP(msg,PeerTCP.socketFD);
      //printf("Sent\n");
      if(node->starter == tmpid)
      {
		node->succiFD = PeerTCP.socketFD;
		strcpy(node->succiIP,tmpip);
		node->succiID = tmpid;
		node->succiPort = tmpport;
		node->prediFD = PeerTCP.socketFD;
		strcpy(node->prediIP,tmpip);
		node->prediID = tmpid;
		node->prediPort = tmpport;
   	  }
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