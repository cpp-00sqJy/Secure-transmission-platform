#include "./RequestFactory.h"

RequestFactory::RequestFactory(string enc):CodecFactory()
{
	this->m_flag = false;
    this->m_encStr = enc;
}

RequestFactory::RequestFactory(Requestinfo* info):CodecFactory()
{
	this->m_flag = true;
    this->m_info = info;
}

Codec* RequestFactory::createCodec()
{
	Codec* codec = NULL;
	if (m_flag)
	{
		codec = new RequestCodec(m_info);
	}
	else
	{
		codec = new RequestCodec(m_encStr);
	}
	return codec;
}

RequestFactory::~RequestFactory()
{
}
