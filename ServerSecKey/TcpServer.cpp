#include "TcpServer.h"
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>

 TcpServer::TcpServer()
 {

 }
 TcpServer::~TcpServer() 
 {

 }
 //服务器设置监听
 int  TcpServer::setListen(unsigned int port)
 {
    int ret = 0;
    //ipv4套接字结构体
    struct sockaddr_in servAddr;
    memset(&servAddr, 0, sizeof(servAddr));
    servAddr.sin_family=AF_INET;       //IPV4协议
    servAddr.sin_port=htons(port);     //主机字节序----->网络字节序 ======>端口
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY); //主机字节序---->网络字节序 ====> IP地址

    //创建监听的文件描述符
    this->m_lfd = socket(AF_INET,SOCK_STREAM,0);
    if (-1 == this->m_lfd)
    {
        ret = errno;
        return ret;
    }
    //设置端口复用
    int opt = 1;
    ret = setsockopt(this->m_lfd,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));
     if (-1 == ret)
    {
        ret = errno;
        return ret;
    }

    //绑定端口和IP
    ret = bind(this->m_lfd,(struct sockaddr*)&servAddr,sizeof(servAddr));
    if (-1 == ret)
    {
        ret = errno;
        return ret;
    }
    //监听
    ret = listen(this->m_lfd,128);
      if (-1 == ret)
    {
        ret = errno;
        return ret;
    }

    return ret;
 }
 //等待并接受客户端连接,默认连接超时10000S
 TcpSocket *  TcpServer::acceptConn(int wait_time)
 {
    int ret = 0;
    if (wait_time > 0)
    {
        fd_set accept_fdset;                        
        FD_ZERO(&accept_fdset);                  //初始化集合
        FD_SET(this->m_lfd,&accept_fdset);       //将需要监听的fd 放入集合
        struct timeval timeout;       
        timeout.tv_sec = wait_time;              //设置监听市场
        timeout.tv_usec = 0;

        do
        {

           //检测读集合
           ret = select(this->m_lfd+1,&accept_fdset,NULL,NULL,&timeout); 

        } while (ret < 0 && errno == EINTR);      //未监听到变化的fd 或 信号中断 继续循环
        if (ret<=0)
        {
            return NULL;
        }
        
    }
    
    //select检测有事件发生  表示三次握手成功  新客户端建立连接成功
    //accept不会阻塞
    struct sockaddr_in cliAddr;
    socklen_t cliAddr_len = sizeof(sockaddr_in);
    int connfd = accept(this->m_lfd,(struct sockaddr *)&cliAddr,&cliAddr_len);
    if (-1 == connfd)
    {
       return NULL;
    }
    return new TcpSocket(connfd);               //返回可通信的文件描述符


 }
 void  TcpServer::closefd()
 {
    close(this->m_lfd);
 }
