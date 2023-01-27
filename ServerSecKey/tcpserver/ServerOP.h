#pragma once
#include <map>
#include <string>
#include "TcpSocket.h"
#include "TcpServer.h"
#include "../protobuf/Message.pb.h"
#include "../OCCI/OCCIOP.h"
#include "../shm/SecKeyNodeInfo.h"
using namespace std;

struct serverConfig
{
	unsigned short port;

	string serverID;

	string userDB;

	string pswdDB;

	string hostDB;
};

class ServerOP
{
public:

	enum KeyLen { Len16 = 16, Len24 = 24, Len32 = 32 };

	//初始化配置文件
	ServerOP(string fileJson);

	~ServerOP();
	//线程工作函数
	static void* callback(void* arg);
	//启动服务器
	void startServer();
	//秘钥协商
	string secKeyAgree(RequestMsg* req);
	//秘钥校验
	string secKeyJiaoYan(RequestMsg* req);
	//秘钥注销
	string secKeyZhuxiao(RequestMsg* req);
	//秘钥查看
	string findSecKey();

private:
	//获取随机字符串   --->秘钥
	string getRandKey(KeyLen len);
private:

	Logger* logAddr = Logger::getInstance();

	serverConfig serConfig;

	map<pthread_t, TcpSocket*>m_list;

	TcpServer* tcpServer = NULL;

	OCCIOP m_occi;

	SecKeyShm* m_shm;

	SecKeyNodeInfo node;
};

