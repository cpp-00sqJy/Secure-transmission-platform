#pragma once
#include <iostream>
#include "../shm/SecKeyShm.h"
using namespace std;


struct ClientInfo
{
	string serverID;
	string clientID;
	string ip;
	unsigned short port;
};

class ClientOP
{
public:
	//初始化配置文件
	ClientOP(string jsonFile);

	~ClientOP();
	//秘钥协商
	bool seckeyAgree();
	//秘钥校验
	bool seckeyCheck();
	//秘钥注销
	bool seckeyZhuXiao();
	//秘钥查看
	bool findSeckey();

private:
	ClientInfo m_Info;

	SecKeyShm* m_shm;

	SecKeyNodeInfo nodeInfo;

	static int m_SeckyID;

};


