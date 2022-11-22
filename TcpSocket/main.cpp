#include <cstdio>
#include "TcpSocket.h"
#include <string.h>
#include <iostream>
#include <unistd.h>


int main()
{
	
	TcpSocket socket;
	// 连接服务器
	cout << "开始连接...";
	socket.conToHost("127.0.0.1", 9999);
	cout << "连接成功...";

	// 通信
	while (1)
	{
		string sendmsg = "hello server ...";
		cout << "发送数据" << sendmsg << endl;
		socket.sendMsg(sendmsg);
		// 接收数据
		int recvLen = -1;
		string recvMsg = socket.recvMsg();
		cout << "recvmsg: " << recvMsg << endl;
		sleep(1);
	}
    return 0;

}
