#include <cstdio>
#include "TcpServer.h"
#include <iostream>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

void *callback(void *arg)
{
    TcpSocket * sock = (TcpSocket * )arg;

    while (1)
    {
        /* code */
        string msg = sock->recvMsg();
        if (msg.empty())
        {
            /* code */
            break;
        }
        cout<<"recvMsg: "<<msg<<endl;
        string sendMsg = " hello client... ";
        sock->sendMsg(sendMsg);

    }
    
}

int main()
{

    TcpServer * server = new TcpServer;
    server->setListen(9999);

    while (1)
    {
        /* code */
        cout<<"开始accept..."<<endl;
        TcpSocket * sockTcp = server->acceptConn();

        cout<<"接收了一个客户端的连接请求...."<<endl;

        if (sockTcp)
        {
            pthread_t tid;
            pthread_create(&tid,NULL,callback,sockTcp);
            pthread_detach(tid);
        }
        
    }
        
        return 0;

}
