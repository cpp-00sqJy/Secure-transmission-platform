#include "nzInterface.h"
#include <fstream>
#include "AesCrypto.h"
#include "SecKeyShm.h"
#include <json/json.h>
#include <sstream>
#include <string.h>
#include <arpa/inet.h>
#include "Message.pb.h"
#include "Codec.h"
#include "RequestFactory.h"
#include"RequestCodec.h"

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

string nzInterface::encData(string& data)
{

	AesCrypto aes(this->m_key);
	//加密后的数据
	string encdata = aes.aesCBCEncData(data);
	//真实数据的长度
	int datasize = data.size();
	//存放需要序列化的数据
	Requestinfo reqinfo;
	reqinfo.cmd = this->m_SecKeyID;
	reqinfo.data = encdata;
	reqinfo.sign = to_string(datasize);

	string size = to_string(datasize);
	//cout << "sign = " << size<<endl;

	CodecFactory* codec = new RequestFactory(&reqinfo);

	Codec* c = codec->createCodec();
	//序列化后的数据
	string output = c->encodeMsg();
	//cout << "序列化后的数据: " << output << endl;
	delete codec;
	delete c;

	return output;
}

string nzInterface::decData(string& data)
{

	CodecFactory* codec = new RequestFactory(data);

	Codec* c = codec->createCodec();

	RequestMsg* requestcodec = (RequestMsg*)c->decodeMsg();

	int SecKeyID = (int)requestcodec->cmdtype();

	string cdata = string(requestcodec->data());

	string sign = string(requestcodec->sign());
	int datasize = stoi(sign);
	//cout << "datasize = " << datasize << endl;
	delete codec;
	delete c;

	SecKeyShm shm(this->key, maxNode);
	SecKeyNodeInfo node = shm.shmRead(SecKeyID);
	string Seckey = string(node.seckey);

	AesCrypto aes(Seckey);

	string decdata = aes.aesCBCDecData(cdata);

	//	cout << "decdata = " << decdata << endl;

	string substr = decdata.substr(0, datasize);
	//cout << "substr = " << substr << endl;

	return substr;
}
