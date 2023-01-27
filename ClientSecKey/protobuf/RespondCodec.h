#pragma once
#include "./Codec.h"

struct Respondinfo
{
    int status;
    int seckeyID;
    string serverID;
    string clientID;
    string data;
};

class RespondCodec :
    public Codec
{
public:
    RespondCodec();
    RespondCodec(string encstr);
    RespondCodec(Respondinfo* info);
    void initMessage(string encstr);
    void initMessage(Respondinfo* info);
    string encodeMsg();
    void* decodeMsg();
    ~RespondCodec();
private:
    string m_encStr;
    RespondMsg m_msg;
};

