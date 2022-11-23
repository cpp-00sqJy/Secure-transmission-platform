#include <cstdio>
#include "TcpServer.h"
#include <iostream>
#include <string.h>
#include <pthread.h>
#include "nzInterface.h"


using namespace std;

void* callback(void* arg)
{
	nzInterface inter("config.json");
	TcpSocket* sockTcp = (TcpSocket*)arg;
	int i = 10;
	// 通信
	while (i--)
	{
		string msg = sockTcp->recvMsg();
		// 解密
		msg = inter.decData(msg);
		cout << "recvMsg: " << msg << endl;
		// 发送数据
		string sendMsg = "hello, client ...";
		sendMsg = inter.encData(sendMsg);
		sockTcp->sendMsg(sendMsg);
	}
	sockTcp->disConnect();
}
int main()
{
	// 创建对象
	TcpServer* server = new TcpServer;
	// 设置监听
	server->setListen(8888);
	// 等待并接受连接请求
	while (1)
	{
		cout << "开始accept..." << endl;
		TcpSocket* sockTcp = server->acceptConn();
		cout << "接受了一个客户端的连接请求..." << endl;
		if (sockTcp)
		{
			pthread_t tid;
			pthread_create(&tid, NULL, callback, sockTcp);
			pthread_detach(tid);
		}
	}
	return 0;
}