#include <cstdio>
#include "SecKeyShm.h"
#include "BaseShm.h"
#include<iostream>
#include <string.h>

using namespace std;

void readShm();
void writeShm();
int main()
{
	SecKeyShm shm();
	cout << "请输入要进行的操作: " << endl;
	cout << "	1. 写内存, 2. 读内存" << endl;
	int sel = 0;
	cin >> sel;
	switch (sel)
	{
	case 1:
		writeShm();
		break;
	case 2:
		readShm();
		break;
	default:
		break;
	}
	cout << "bye bye!!!" << endl;

	return 0;
}

void writeShm()
{
	NodeSHMInfo info;
	strcpy(info.clientID, "kevin");
	strcpy(info.serverID, "Alice");
	strcpy(info.seckey, "我是基督山伯爵...");
	info.seckeyID = 100;
	info.status = 1;
	SecKeyShm shm("/home", 10);
	shm.shmInit();
	shm.shmWrite(&info);

	strcpy(info.clientID, "Bob");
	strcpy(info.serverID, "Jack");
	strcpy(info.seckey, "基督山伯爵就是 - 爱德蒙·唐代斯!!!");
	info.seckeyID = 666;
	info.status = 1;
	shm.shmWrite(&info);
}

void readShm()
{
	char tmp[128];
	strcpy(tmp, "Bob");
	SecKeyShm shm("/home", 10);
	NodeSHMInfo info = shm.shmRead(tmp, "Jack");
	cout << "ClientID: " << info.clientID << endl;
	cout << "ServerID: " << info.serverID << endl;
	cout << "SecKey: " << info.seckey << endl;
	cout << "seckeyID: " << info.seckeyID << endl;
	cout << "status: " << info.status << endl;
	// shm.delShm();
}