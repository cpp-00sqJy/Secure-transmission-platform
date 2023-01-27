#include "./TcpSocket.h"
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/select.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

    TcpSocket::TcpSocket(){}
    //实例化可通信的套接字
     TcpSocket::TcpSocket(int fd)
    {
        this->m_socket = fd;
    }
    
     TcpSocket::~TcpSocket(){}
    /*
     * conToHost - 连接服务器
     * @ip, port : IP地址 和 端口
     * @wait_time: 等待超时秒数，如果为0表示不检测超时
     * 成功（未超时）返回0，失败返回-1，超时返回-1并且errno = ETIMEDOUT  
     */
    int TcpSocket::conToHost(string ip,unsigned short port,int wait_time)
    {
        int ret = 0;
        if (port <= 0 || port > 65535 || wait_time < 0)
        {
            ret = ParamError;
            return ret ;
        }

        this->m_socket = socket(AF_INET,SOCK_STREAM,0);
        if(this->m_socket < 0)
        {
            ret = errno;
            printf("func socket() err:  %d\n", ret);
		    return ret;
        }

        struct sockaddr_in servAddr;
        memset(&servAddr, 0, sizeof(servAddr));
        servAddr.sin_family = AF_INET;
        servAddr.sin_port = htons(port);
        servAddr.sin_addr.s_addr = inet_addr(ip.data());

        ret = connectTimeout((struct sockaddr_in*)(&servAddr),(unsigned int)wait_time);
        if (ret < 0)      
        {
            //超时
            if (ret == -1 && errno == ETIMEDOUT)
            {
                ret =TimeoutError;
                return ret;
            }
            else
            {
                printf("connectTimeout 调用异常, 错误号: %d\n", errno);
			    return errno;
            }
            
        }

        return ret ;
    }
    /*
     * sendMsg - 发送报文
     * @sendData : 发送数据
     * @wait_time: 等待超时秒数，如果为0表示不检测超时
     * 成功（未超时）返回0，失败返回-1，超时返回-1并且errno = ETIMEDOUT  
     */
    int TcpSocket::sendMsg(string sendData, int wait_time)
    {
        
        int ret = writeTimeout(wait_time); /*ret = 0 --->没超时  ret = -1 ---->超时*/
        
        if (ret == 0)
        {
            int writed = 0;   
            int dataLen = sendData.size()+4;  //发送的数据长度 + 4个字节的报头
            //存储发送的数据 + 数据头
            unsigned char * netData = (unsigned char *)malloc(dataLen);
            
            if (netData == NULL)
            {
                ret = MallocError;
			    printf("func sckClient_send() mlloc Err:%d\n ", ret);
			    return ret;
            }
            //转换为网络字节序
            int netLen = htonl(sendData.size());
            //存储数据长度
            memcpy(netData,&netLen,4);
            //存储数据
            memcpy(netData+4,sendData.data(),sendData.size());

            //返回实际发送字节数
            //失败返回 -1
            writed = writen(netData,dataLen);

            if (writed < dataLen)      //发送失败
            {
                //释放内存
                if (netData != NULL)  
                {
                    free(netData);
                    netData = NULL;
                }
                
            }
            //释放内存
            if (netData != NULL)
                {
                    free(netData);
                    netData = NULL;
                }

        }
        else
        {
             //超时
            if (ret == -1 && errno == ETIMEDOUT)
		    {
			    ret = TimeoutError;
			    printf("func sckClient_send() mlloc Err:%d\n ", ret);
		    }
        }
        
        return ret ;
    }
     /*
     * recvMsg - 接收报文
     * @wait_time: 等待超时秒数，如果为0表示不检测超时
     * 成功返回接收的真实数据 , 失败返回空串
     */
    string TcpSocket::recvMsg(int wait_time)
    {
        int ret = readTimeout(wait_time);

        if (ret != 0)
        {
            if (ret == -1 || errno == ETIMEDOUT)
		    {
			    printf("readTimeout(timeout) err: TimeoutError \n");
			    return string();
		    }
		    else
		    {
			    printf("readTimeout(timeout) err: %d \n", ret);
			    return string();
		    }
        }
        
        int netdataLen = 0;
        //读取4个字节的数据长度
        ret = readn(&netdataLen,4);
        if (ret == -1)
	    {
		    printf("func readn() err:%d \n", ret);
		    return string();
	    }
	    else if (ret < 4)
	    {
		    printf("func readn() err peer closed:%d \n", ret);
		    return string();
	    }

        //转换为主机字节序
        int n = ntohl(netdataLen);
        //根据包头记录中的数据大小申请内存,存放数据
        char * tmpBuf = (char *)malloc(n+1);
        if (NULL == tmpBuf)
        {
            ret = MallocError;
            printf("malloc() err \n");
            return NULL;
        }
        //根据长度读取接收的数据
        ret = readn(tmpBuf,n);
        if (ret == -1)
        {
            printf("func readn() err:%d \n", ret);
            return string();
        }
        else if (ret < n)
        {
            printf("func readn() err peer closed:%d \n", ret);
            return string();
        }

        //多分配一个字节内容，兼容可见字符串 字符串的真实长度仍然为n
        tmpBuf[n]='\0'; 

        string data = string(tmpBuf);
        //释放内存
        free(tmpBuf);
        //返回数据
        return data;
    }
    //断开连接
    void TcpSocket::disconnect()
    {
        if (this->m_socket >= 0 )
        {
            close(this->m_socket);
        }
        
    }
    //设置I/O非阻塞
    int TcpSocket::set_NONBLOCK(int fd)
    {
        int flags = fcntl(fd,F_GETFL);
        if (-1 == flags)
        {
            return flags;
        }
        flags |= O_NONBLOCK;
        int ret = fcntl(fd,F_SETFL,flags);
        return ret;
    }
    //设置I/O阻塞
    int TcpSocket::set_BLOCK(int fd)
    {
int flags = fcntl(fd,F_GETFL);
        if (-1 == flags)
        {
            return flags;
        }
        flags &= ~O_NONBLOCK;
        int ret = fcntl(fd,F_SETFL,flags);
        return ret;
    }
/*
 * readTimeout - 读超时检测函数，不含读操作
 * @wait_time: 等待超时秒数，如果为0表示不检测超时
 * 成功（未超时）返回0，失败返回-1，超时返回-1并且errno = ETIMEDOUT  
 */
    int TcpSocket::readTimeout(unsigned int wait_time)
    {
        int ret = 0;
        if (wait_time > 0)
        {
           fd_set read_fdset;

           FD_ZERO(&read_fdset);
           FD_SET(this->m_socket,&read_fdset);

           struct timeval timeout;

           timeout.tv_sec = wait_time;
           timeout.tv_usec = 0;

        //1 若timeout时间到（超时），没有检测到读事件 ret返回=0
		//2 若ret返回<0 &&  errno == EINTR 说明select的过程中被别的信号中断（可中断睡眠原理）
		//2-1 若返回-1，select出错
		//3 若ret返回值>0 表示有read事件发生，返回事件发生的个数
           do
           {

             ret = select(this->m_socket+1,&read_fdset,NULL,NULL,&timeout);

           } while (ret < 0 && errno == EINTR );

           if (0 == ret)    //超时
           {
              ret = -1;
              errno = ETIMEDOUT;
           }
           else if (1 == ret)    //没超时
           {
             ret = 0;
           }
          
        }
        
       return ret ;
    }
/*
 * writeTimeout - 写超时检测函数，不含写操作
 * @wait_time: 等待超时秒数，如果为0表示不检测超时
 * 成功（未超时）返回0，失败返回-1，超时返回-1并且errno = ETIMEDOUT  
 */
    int TcpSocket::writeTimeout(unsigned int wait_time)
    {
        int ret = 0;
        if (wait_time > 0)
        {
           
           fd_set write_fdset;

           FD_ZERO(&write_fdset);
           FD_SET(this->m_socket,&write_fdset);

           struct timeval timeout;

           timeout.tv_sec = wait_time;
           timeout.tv_usec = 0;

           do
           {
            //检测写集合
            ret = select(this->m_socket+1,NULL,&write_fdset,NULL,&timeout);

           } while (ret < 0 && errno == EINTR);
           
           if (0 == ret)           //超时
           {
              ret = -1;
              errno = ETIMEDOUT;
           }
           else if(1 == ret)       //未超时
            {
               ret = 0; 
            }

        }
        
         return ret;

    }
   /*
    * connectTimeout - 检测连接超时,包含connect 连接
    * @struct sockaddr_in *addr : 套接字结构体
    * @wait_time: 等待超时秒数，如果为0表示不检测超时
    * 成功（未超时）返回0，失败返回-1，超时返回-1并且errno = ETIMEDOUT  
    */
    int TcpSocket::connectTimeout(struct sockaddr_in *addr , unsigned int wait_time)
    {
        int ret;
        socklen_t addrlen = sizeof(struct sockaddr_in);

        if (wait_time > 0)
        {
            //设置套接字非阻塞模式
            set_NONBLOCK(this->m_socket);         
        }
        //非阻塞连接
        ret = connect(this->m_socket,(struct sockaddr*)addr,addrlen);
        //ret < 0 && errno = EINPROGRESS   表示连接进行中....
        if(ret < 0 && errno == EINPROGRESS)
        {
            fd_set connect_fdset;

            FD_ZERO(&connect_fdset);
            FD_SET(this->m_socket,&connect_fdset);

            struct timeval timeout;
            
            timeout.tv_sec = wait_time;
            timeout.tv_usec = 0;

            do
            {
                //发送连接, 检测写集合
                ret = select(this->m_socket+1,NULL,&connect_fdset,NULL,&timeout);
                
            } while (ret < 0 && errno == EINTR);

            if (0 == ret)         //超时
            {
              ret = -1;
              errno = ETIMEDOUT;
            }
            else if (ret < 0)      //select 检测失败
            {
               return -1;
            }
            else if (ret == 1)     //检测到文件描述符变化
            {

                //此时并未能正确检测到是否连接成功
                //返回的结果 : 1、已经连接成功  2、是套接字产生错误
                //使用 getsockopt函数 将结果返回至 err
                int err;
                socklen_t sockLen = sizeof(err);
                int sockoptret = getsockopt(this->m_socket,SOL_SOCKET,SO_ERROR,&err,&sockLen);
                if (-1 == sockoptret)
                {
                    return -1;
                }
                if (0 == err)             //连接成功
                {
                    ret = 0;
                }
                else
                {
                    errno =err;           //发生错误
                    ret = -1;
                }
                
            }
        
        }
        if(wait_time > 0)
        {
            //将标志位恢复至阻塞
            set_BLOCK(this->m_socket);
        }
        return ret ;
    }
    
    /*
     * readn - 读取固定字节数
     * @fd: 文件描述符
     * @buf: 接收缓冲区
     * @count: 要读取的字节数
     * 成功返回count，失败返回-1，读到EOF返回<count
     */
    int TcpSocket::readn(void * buf,int count)
    {
        size_t nleft = count;
        ssize_t nread;
        char *bufp = (char *)buf;

        while (nleft > 0)
        {
           if ((nread = read(this->m_socket,bufp,nleft))<0)
           {
             if(errno = EINTR)
             {
                continue;
             }
             return -1;
           }
           else if (nread == 0)
           {
                return count - nleft;
           }
           
           bufp += nread;
           nleft -= nread;
           
        }

        return count;
    }
    /*
     * writen - 发送固定字节数
     * @buf: 发送缓冲区
     * @count: 要读取的字节数
     * 成功返回count，失败返回-1
     */  
    int TcpSocket::writen(const void *buf,int count)
    {
            size_t nleft = count;
	    ssize_t nwritten;
	    char *bufp = (char*)buf;

        while (nleft > 0)
        {
            if ((nwritten = write(m_socket, bufp, nleft)) < 0)
            {
                if (errno == EINTR)	// 被信号打断
                {
                    continue;
                }
                return -1;
            }
            else if (nwritten == 0)
            {
                continue;
            }

            bufp += nwritten;
            nleft -= nwritten;
        }

	        
            return count;

    }
    
    
