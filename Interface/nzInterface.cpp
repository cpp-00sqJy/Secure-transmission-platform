#include "nzInterface.h"
#include <fstream>
#include "AesCrypto.h"
#include "SecKeyShm.h"
#include <json/json.h>
#include <sstream>
#include <string.h>
#include <arpa/inet.h>

using namespace Json;

nzInterface::nzInterface(string json)
{
	ifstream ifs(json);

	Reader rd;
	Value root;
	rd.parse(ifs, root);

	//string cid = root["ClientID"].asString();
	//string sid = root["ServerID"].asString();
	key = root["ShmKey"].asString();
	maxNode = root["MaxNode"].asInt();
	SecKeyShm shm(key, maxNode);
	SecKeyNodeInfo nodeInfo = shm.shmFistNode();
	m_key = string(nodeInfo.seckey);
	m_SecKeyID = nodeInfo.seckeyID;
	status = nodeInfo.status;
}

nzInterface::~nzInterface()
{

}

string nzInterface::encData(const string& data)
{
	AesCrypto aes(this->m_key);
	string encdata = aes.aesCBCEncData(data);

	int len = encdata.size();
	int SeckeyID = htonl(this->m_SecKeyID);
	string buf;
	buf.resize(len + sizeof(len) + sizeof(len));
	memcpy(&buf[0], (const void*)(&SeckeyID), 4);
	memcpy(&buf[0] + 4, (const void*)(&len), 4);
	memcpy(&buf[0] + 8, (const void*)(&encdata[0]), len);

	return buf;
}

string nzInterface::decData(const string& data)
{
	int SecKeyID = 0;
	memcpy((char*)(&SecKeyID), &data[0], 4);
	int keyID = ntohl(SecKeyID);
	cout << "SeckID = " << keyID << endl;
	int len = 0;
	memcpy((char*)(&len), &data[0] + 4, 4);
	cout << "len = " << len << endl;
	std::string msg;
	msg.resize(len);
	memcpy((char*)(&msg[0]), &data[0] + 8, len);
	cout << "msg = " << msg << endl;
	SecKeyShm shm(this->key, maxNode);
	SecKeyNodeInfo node = shm.shmRead(keyID);

	string Seckey = string(node.seckey);

	AesCrypto aes(Seckey);

	string decdata = aes.aesCBCDecData(msg);

	return decdata;
}
