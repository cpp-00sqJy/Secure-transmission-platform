#pragma once
#include "CodecFactory.h"
#include"RespondCodec.h"

class RespondFactory :
    public CodecFactory
{
public:
    RespondFactory(string enc);
    RespondFactory(Respondinfo* info);
    Codec* createCodec();
    ~RespondFactory();
private:
    bool m_flag;
    string m_enStr;
    Respondinfo* m_info;
};

