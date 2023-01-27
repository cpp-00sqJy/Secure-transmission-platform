#include <iostream>

using namespace std;


/*用于通信的套接字类*/

//默认超时 10000S
static const int TIMEOUT = 10000;

class TcpSocket
{
public:
    enum ErrorType {ParamError = 3001, TimeoutError, PeerCloseError, MallocError};
    TcpSocket();
    //实例化可通信的套接字
    TcpSocket(int fd);

    ~TcpSocket();

    //连接服务器
    int conToHost(string ip,unsigned short port,int timeout = TIMEOUT );
    //发送报文
    int sendMsg(string sendData, int timeout = TIMEOUT);
    //接受报文
    string recvMsg(int timeout = TIMEOUT);
    //断开连接
    void disconnect();

private:
    //设置I/O非阻塞
    int set_NONBLOCK(int fd);
    //设置I/O阻塞
    int set_BLOCK(int fd);
    //读超时检测 不含读操作
    int readTimeout(unsigned int wait_time);
    //写超时检测 不含写操作
    int writeTimeout(unsigned int wait_time);
    //带连接超时的 connect函数
    int connectTimeout(struct sockaddr_in *addr , unsigned int wait_time);
    //每次从缓冲区中读取n个字符
    int readn(void * buf,int count);
    //每次从缓冲区中写入n个字符
    int writen(const void *buf,int count);

private:
    int m_socket;          //用于通信的套接字
};
