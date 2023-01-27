#pragma once
#include "./BaseShm.h"
#include <string.h>
#include "./SecKeyNodeInfo.h"

class SecKeyShm :
	public BaseShm
{
public:

	SecKeyShm(int key, int maxNode);
	SecKeyShm(string pathname, int maxNode);
	~SecKeyShm();

	int shmWrite(SecKeyNodeInfo* pNodeInfo);
	SecKeyNodeInfo shmRead(string clientID, string serverID);
	SecKeyNodeInfo shmRead(int keyID);
	SecKeyNodeInfo shmFirstNode();
	void shmInit();

private:

	int m_maxNode;

};

