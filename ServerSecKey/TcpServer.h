#pragma once
#include "TcpSocket.h"

class TcpServer
{
public:
    TcpServer();
    ~TcpServer();
    //服务器设置监听
    int setListen(unsigned int port);
    //等待并接受客户端连接,默认连接超时10000S
    TcpSocket * acceptConn(int timeout=10000);
    void closefd();


private:

    int m_lfd;
};