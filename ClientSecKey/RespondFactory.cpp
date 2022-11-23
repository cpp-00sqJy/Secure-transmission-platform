#include "RespondFactory.h"

RespondFactory::RespondFactory(string enc):CodecFactory()
{
	this->m_flag = false;
	this->m_enStr = enc;
}

RespondFactory::RespondFactory(Respondinfo* info):CodecFactory()
{
	this->m_flag = true;
	this->m_info = info;
}

Codec* RespondFactory::createCodec()
{
	Codec* codec = NULL;
	if (m_flag)
	{
		codec = new RespondCodec(m_info);
	}
	else
	{
		codec = new RespondCodec(m_enStr);
	}
	return codec;
}

RespondFactory::~RespondFactory()
{
}
