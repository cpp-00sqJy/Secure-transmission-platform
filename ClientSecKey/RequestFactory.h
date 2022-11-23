#pragma once
#include "CodecFactory.h"
#include"RequestCodec.h"
class RequestFactory :
    public CodecFactory
{
public:
    RequestFactory(string enc);
    RequestFactory(Requestinfo* info);
    Codec* createCodec();
    ~RequestFactory();

private:
    bool m_flag;
    string m_encStr;
    Requestinfo* m_info;
};

