#pragma once
#include "Codec.h"
struct Requestinfo
{
    int cmd;
    string clientID;
    string serverID;
    string sign;
    string data;
};

class RequestCodec :
    public Codec
{
public:
    RequestCodec();
    RequestCodec(string encstr);
    RequestCodec(Requestinfo* info);
    void initMessage(string encstr);
    void initMessage(Requestinfo* info);
    string encodeMsg();
    void* decodeMsg();
    ~RequestCodec();

private:
    string m_encStr;
    RequestMsg m_msg;
};

