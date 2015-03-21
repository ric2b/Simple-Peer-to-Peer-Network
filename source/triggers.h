#ifndef _TRIGGERS_H
#define _TRIGGERS_H

int Join_Ring(ringStruct*, socketStruct );

void searchNode(ringStruct * ringData, int k);

void removeNode(ringStruct * ringData, socketStruct socketCFG);

void showNode(ringStruct * ringData);

int joinRing_KnownSucci(ringStruct * ringData, int succiID, char * succiIP, int succiPort);

#endif
