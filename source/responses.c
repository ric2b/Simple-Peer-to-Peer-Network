#include "responses.h"

void readPredi(char* msg,ringStruct* myNode)
{
    char cmd[128],buffer[128];
    int qNode, i;
    //Ver qual o tipo de comando que foi recebido
    sscanf(msg,"%s",cmd);
    if(strcmp(cmd,"QRY") == 0)
    {
        sscanf(msg,"%s %d %d",cmd,&i,&qNode);
        if(responsability(myNode->prediID,myNode->myID,qNode) == 1)
        {
            sprintf(buffer,"RSP %d %d %d %s %d\n", i, qNode, myNode->myID, myNode->myIP,myNode->myPort);
            sendTCP(msg, myNode->succiFD);
            return;
        }
        else
        {
            sendTCP(msg, myNode->succiFD);
        	return;
        }
    }
    /*else if()
    {
        //meter aqui os outros comportamentos
    }*/
}

void readSucci(char* msg,ringStruct* myNode)
{
    char cmd[128],buffer[128],respIP[128];
    int qNode, i, respID,respTCP;
    //Ver qual o tipo de comando que foi recebido do no succi
    sscanf(msg,"%s",cmd);
    if(strcmp(cmd,"RSP") == 0)
    {
        sscanf(msg,"%s %d %d %d %s %d",cmd,&i,&qNode,&respID,respIP,&respTCP);
        if(i == myNode->myID)
        {
            printf("%i %s %i", respID, respIP, respTCP);
            return;
        }
        else
        {
            sendTCP(msg, myNode->prediFD);
            return;
        }
    }
    /*else if()
    {
        //meter aqui os outros comportamentos
    }*/
}
