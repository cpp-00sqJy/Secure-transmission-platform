# 安全传输平台部署须知

## 基础测试

- 服务器测试环境

  - Centos7  (不支持Ubuntu，其他环境均未测试)
  - Oracle服务器版本 11gR2
  - OpensSSL版本OpenSSL 1.1.1c  (64位)
    - 下载地址:  <https://www.openssl.org/source/>
  - Protobuf版本 3.8.0
    - https://github.com/google/protobuf/releases 
  - jsoncpp版本 0.10.7
  - scons 版本 3.0.5 用于编译和链接jsoncpp

- 客户端测试环境

  - Centos7  (不支持Ubuntu，其他环境均未测试)
  - OpensSSL版本OpenSSL 1.1.1c  (64位)
  - Protobuf版本 3.8.0
  - jsoncpp版本 0.10.7
  - scons 版本 3.0.5 用于编译和链接jsoncpp

- 测试前请先配置好本地环境

  - 配置好以上环境

  - 在 /etc/ld.so.conf 文件配置环境变量

    - 例 /usr/local/openssl/lib

  - OCCI环境配置

    ```shell
    # OCCI_HOME 该环境变量的路径需要做对应修改
      export OCCI_HOME=/opt/instantclient_11_2    #安裝包名 自行修改
      export OCCI_INCLUDE_DIR=$OCCI_HOME/sdk/include
      export OCCI_LIBRARY_PATH=$OCCI_HOME
      export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$OCCI_LIBRARY_PATH
    # 程序编译是搜索的库目录
      export LIBRARY_PATH=$LIBRARY_PATH:$OCCI_LIBRARY_PATH
    # 程序编译时搜索的头文件目录
      export CPLUS_INCLUDE_PATH=$CPLUS_INCLUDE_PATH:$OCCI_INCLUDE_DIR
    ```

## 项目运行

- **客户端运行注意事项**
  - ClientSecKey需部署与业务客户端同一台主机

- **ClientSecKey**

  - 生成client

    ```shell
    make 或 make client
    ```

  - 运行客户端

    ```shell
    ./client
    ```

  - 自行修改client.json配置文件

    - 自行更換 ServerID | ClientID | ServerIP | Port |ShmKey

    ```json
    {
        "ServerID":"3333",
        "ClientID":"4444",
        "ServerIP":"192.168.68.235",  
        "Port":9898,
        "ShmKey":"/usr/local",         
        "MaxNode":1
    }
    ```

- **服务器部署注意事项**

  - 需与业务服务器部署于同一台主机

- **ServerSecKey**

  - - 生成server

      ```shell
      make 或 make server
      ```

    - 运行客户端

      ```shell
      ./server
      ```

    - 自行修改client.json配置文件

      - 自行更換 ServerID | ClientID | ServerIP | Port |ShmKey

      ```json
      {
          "Port": 9898,
          "ServerID":"3333",
          "UserDB":"SECMNG",
          "PswdDB":"SECMNG",
          "HostDB":"192.168.68.245:1521/orcl",
          "ShmKey":"/usr/lib",
          "MaxNode":100
      }
      ```

- **加解密接口**

  - 部署好提供的动态库和头文件
    - libinterface.so
    - nzInterface.h
  - 分别在业务服务器和业务客户端配置提供的json文件
    - clientConfig.json
      - 该文件ShmKey需与客户端的client.json的ShmKey一致
    - serverConfig.json
      - 该文件ShmKey需与服务器的server.json的ShmKey一致



