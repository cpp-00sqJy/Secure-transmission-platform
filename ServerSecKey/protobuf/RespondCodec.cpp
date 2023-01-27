#include "./RespondCodec.h"

RespondCodec::RespondCodec()
{

}
RespondCodec::RespondCodec(string encstr)
{
	this->initMessage(encstr);
}
RespondCodec::RespondCodec(Respondinfo* info)
{
	this->initMessage(info);
}
void RespondCodec::initMessage(string encstr)
{
	this->m_encStr = encstr;
}
void RespondCodec::initMessage(Respondinfo* info)
{
	this->m_msg.set_rv(info->status);
	this->m_msg.set_clientid(info->clientID);
	this->m_msg.set_serverid(info->serverID);
	this->m_msg.set_seckeyid(info->seckeyID);
	this->m_msg.set_data(info->data);
}
string RespondCodec::encodeMsg()
{
	string output;
	this->m_msg.SerializeToString(&output);
	return output;
}
void* RespondCodec::decodeMsg()
{
	this->m_msg.ParseFromString(this->m_encStr);
	return &m_msg;
}
RespondCodec::~RespondCodec()
{

}
