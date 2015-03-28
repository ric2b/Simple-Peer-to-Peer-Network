#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <math.h>

#include "print_messages.h"

void message_handler(int debug_mode, int message_id,char* message,ringStruct* node, int nb)
{
	if(debug_mode == 1)
	{
		if(message_id == 1)
			printf("Finished processing unknown message\n");
		if(message_id == 2)
		{
			printf("Predi Funct\n");
			printf("Finished processing predi\n");
		}
		if(message_id == 3)
		{
			printf("Succi Funct\n");
			printf("Finished processing succi\n");
		}
		if(message_id == 4)
		{
			printf("Add Node Funct\n");
			printf("Finished processing Add Node\n");	
		}
		if(message_id == 5 || message_id == 6 || message_id == 7 || message_id == 8)
		{
			printf("Succi: %d \t Predi: %d\n",node->succiID,node->prediID);
			printf("Succi FD: %d \t Predi FD: %d\n",node->succiFD,node->prediFD);
			printf("Succi TCP: %d \t Predi TCP: %d\n",node->succiPort,node->prediPort);	
		}
		if(message_id == 9)
		{
			printf("My responsability\n");
			printf("Sending to Master %s",message);
		}
		if(message_id == 10)
			printf("Sending to Succi %s",message);
		if(message_id == 11)
			printf("DISTANCE: %d\n", nb);
		if(message_id == 12)
			printf("A analisar: %s\n",message);	
		if(message_id == 13)
			printf("porta %d ocupada, a tentar a próxima\n", nb);
	}
	return;
}

void print_ring(int valores[]) 
{
    int raio = 16;
    int k = 0;
    int m = 0;
    int ordem = 0;
    int i,j=0,n,p;
    n = 0;
    //int valore[]={0,1,2,4,7,15,23,31,36,45,56,62};
    p = 0;
    int size = 64;
    printf("%d\n",size);
    /*for(i=0;i<64;i++)
    {
        if(valore[j] == i)
        {
            valores[i] = valore[j];
            j++;
        }
        if(i % 4 == 0)
            valores[i] = i;
        else
            valores[i] = -1;

        printf("Print %d : %d\n",i,valores[i]);
    }*/   

    for (i=0; i<=2*raio; i++)
    {
        for (j=0; j<=2*raio; j++)
        {
            double long distancia = sqrt((double long)(i-raio)*(i-raio) + (j-raio)*(j-raio));
            if (distancia>raio-0.5 && distancia<raio+0.5)
            {
                if(j == 0)
                    ordem = 1;
                if(i == 0 || i == 32)
                    ordem = 3; 
                //printf("c%d=%d@%d",(64-i + 1),valores[size-k-1],ordem);
                if(valores[size-k] == (64 - i) && size-k >= 31)
                {
                    if(ordem == 1)
                    {
                        printf("%d",valores[size-k]);
                        k++;
                        ordem = 2;
                        p = 1;
                    }
                }
                else
                    if(ordem == 1)
                    {
                        p = 0;
                        ordem = 2;
                    }
                //printf("c%d=%d@%d-%d",i,valores[m],ordem,m);
                if(i == valores[m] && m <= 32)
                { 
                    if(ordem == 3)
                    {
                        printf("%d",valores[m]);
                        m++;
                        ordem = 4;
                        n = 1;
                    }    
                }
                else
                    if(ordem == 3)
                    {
                        ordem = 4;
                        n = 0;
                    } 
                printf("...");
            }
            else
            { 
                printf("  ");
                if(ordem == 0)
                    ordem = 1;
                else
                    if(ordem == 2)
                        ordem = 3;
                    else
                        if(ordem == 4)
                            ordem = 0;
            }
        }
        printf("\n");
        ordem = 0;
        if(p == 0)
            k++;
        if((n == 0 && i != 0) || (n == 0 && i == 0 && m == 0))
            m++;
 
    }
    return;
}