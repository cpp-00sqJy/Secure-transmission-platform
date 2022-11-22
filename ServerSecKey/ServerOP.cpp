#include "ServerOP.h"
#include <json/json.h>
#include <fstream>
#include "TcpServer.h"
#include <pthread.h>
#include "Codec.h"
#include "RespondCodec.h"
#include "RespondFactory.h"
#include "TcpSocket.h"
#include "Message.pb.h"
#include "RsaCrypto.h"
#include "Hash.h"
#include "RequestFactory.h"
#include "RequestCodec.h"
#include <unistd.h>
#include "BaseShm.h"
#include "SecKeyShm.h"
#include "string.h"
#include <string.h>
using namespace std;
using namespace Json;




ServerOP::ServerOP(string fileJson)
{
	ifstream ifs(fileJson);

	Value root;
	Reader r;
	r.parse(ifs, root);

	this->serConfig.port = root["Port"].asInt();
	this->serConfig.serverID = root["ServerID"].asString();
	this->serConfig.userDB = root["UserDB"].asString();
	this->serConfig.pswdDB = root["PswdDB"].asString();
	this->serConfig.hostDB = root["HostDB"].asString();
	string ShmKey = root["ShmKey"].asString();
	int maxNode = root["MaxNode"].asInt();
	//实例化操作数据库对象

	this->m_occi.connectDB(serConfig.userDB, serConfig.pswdDB, serConfig.hostDB);


	m_shm = new SecKeyShm(ShmKey, maxNode);
}

ServerOP::~ServerOP()
{
	if (tcpServer)
	{
		delete tcpServer;
	}
}


void ServerOP::startServer()
{
	tcpServer = new TcpServer;

	tcpServer->setListen(this->serConfig.port);

	while (1)
	{
		cout << "等待客户端连接..." << endl;
		TcpSocket* sockTcp = tcpServer->acceptConn();
		if (sockTcp == NULL)
		{
			continue;
		}
		cout << "客户端连接成功..." << endl;

		pthread_t tid;
		pthread_create(&tid, NULL, callback, this);
		m_list.insert(make_pair(tid, sockTcp));

	}

}

void* ServerOP::callback(void* arg)
{

	sleep(1);

	ServerOP* op = (ServerOP*)arg;

	TcpSocket* tcp = op->m_list[pthread_self()];

	string recMsg = tcp->recvMsg();

	CodecFactory* factory = new RequestFactory(recMsg);

	Codec* c = factory->createCodec();

	RequestMsg* req = (RequestMsg*)c->decodeMsg();
	cout << "cmdtype = " << req->cmdtype() << endl;
	string data;
	switch (req->cmdtype())
	{

	case 1:
		data = op->secKeyAgree(req);
		break;
	case 2:
		data = op->secKeyJiaoYan(req);
		break;
	case 3:
		data = op->secKeyZhuxiao(req);
		break;
	case 4:

		break;
	default:
		break;
	}

	delete factory;
	delete c;

	tcp->sendMsg(data);
	tcp->disconnect();
	op->m_list.erase(pthread_self());
	delete tcp;

	return NULL;
}
string ServerOP::secKeyAgree(RequestMsg* req)
{

	ofstream ofs("public.pem");
	ofs << req->data();
	ofs.close();

	Respondinfo reqInfo;
	RsaCrypto rsa("public.pem", false);


	Hash sha1(T_SHA1);
	sha1.add_Data(req->data());
	string sign = sha1.result();
	cout << "sign = " << sign << endl;
	cout << "req->sign() = " << req->sign() << endl;
	bool b = rsa.Rsa_Verify(sign, req->sign());
	if (b != true)
	{
		reqInfo.status = false;
		cout << "数据校验失败...." << endl;
	}
	else
	{
		cout << "校验签名成功..." << endl;
		string randStr = this->getRandKey(Len16);
		cout << "生成的随机秘钥: " << randStr << endl;
		string pubencKey = rsa.Rsa_Public_encrypt(randStr);

		cout << "加密之后的秘钥: " << pubencKey << endl;
		reqInfo.clientID = req->clientid();
		reqInfo.data = pubencKey;
		reqInfo.status = true;
		reqInfo.serverID = this->serConfig.serverID;

		strcpy(node.clientID, req->clientid().data());
		strcpy(node.serverID, req->serverid().data());
		strcpy(node.seckey, pubencKey.data());
		node.seckeyID = m_occi.getKeyID();
		reqInfo.seckeyID = node.seckeyID;
		node.status = 1;

		SecKeyNodeInfo infonode = node;
		strcpy(infonode.seckey, randStr.data());

		bool b1 = m_occi.writeSecKey(&node);
		if (b1)
		{
			m_occi.updataKeyID(node.seckeyID + 1);

			m_shm->shmWrite(&infonode);
		}
		else
		{
			reqInfo.status = false;
		}
	}

	CodecFactory* factory = new RespondFactory(&reqInfo);
	Codec* codec = factory->createCodec();
	string encData = codec->encodeMsg();

	delete factory;
	delete codec;
	return encData;
}

string ServerOP::secKeyJiaoYan(RequestMsg* req)
{

	RsaCrypto rsa;
	SecKeyNodeInfo nodeinfo = m_shm->shmRead(node.seckeyID);
	cout << "共享内存存储的秘钥: " << nodeinfo.seckey << endl;
	string seckey = string(nodeinfo.seckey);
	string signSeckey = req->sign();
	Hash sha1(T_SHA1);

	sha1.add_Data(seckey);
	string signdata = sha1.result();

	Respondinfo reqInfo;


	//bool b = rsa.Rsa_Verify(signdata, req->sign());
	//cout << "signdata = " << signdata << endl;
	//cout << "signSeckey = " << signSeckey << endl;
	int ret = strcmp(signdata.data(), signSeckey.data());
	if (ret == 0)
	{
		reqInfo.status = true;
		cout << "秘钥校验成功...." << endl;

	}
	else
	{
		reqInfo.status = false;
		cout << "秘钥校验失败...." << endl;
	}


	CodecFactory* factory = new RespondFactory(&reqInfo);
	Codec* codec = factory->createCodec();
	string encData = codec->encodeMsg();

	delete factory;
	delete codec;
	return encData;

}

string ServerOP::secKeyZhuxiao(RequestMsg* req)
{

	int SecKeyId = req->seckeyid();
	node = m_shm->shmRead(SecKeyId);
	node.status = 0;
	strcpy(node.clientID, req->clientid().data());
	strcpy(node.serverID, req->serverid().data());
	node.seckeyID = req->seckeyid();
	int ret = m_shm->shmWrite(&node);


	cout << "SecKeyId: " << SecKeyId << endl;
	bool b = m_occi.updateKeyStatus(SecKeyId);
	Respondinfo res;
	if (b)
	{

		res.seckeyID = SecKeyId;
		res.status = true;
		cout << "秘钥注销成功.." << endl;
	}
	else
	{
		res.seckeyID = SecKeyId;
		res.status = false;
		cout << "秘钥注销失败.." << endl;

	}

	CodecFactory* factory = new RespondFactory(&res);
	Codec* codec = factory->createCodec();
	string encData = codec->encodeMsg();

	delete factory;
	delete codec;
	return encData;
}

string ServerOP::findSecKey()
{







	return string();
}

string ServerOP::getRandKey(KeyLen len)
{
	srand(time(NULL));
	int flag = 0;
	string randStr = string();
	char* cs = "~!@#$%^&*()_+}{|\';[]";
	for (int i = 0; i < len; ++i)
	{
		flag = rand() % 4;	// 4中字符类型
		switch (flag)
		{
		case 0:	// a-z
			randStr.append(1, 'a' + rand() % 26);
			break;
		case 1: // A-Z
			randStr.append(1, 'A' + rand() % 26);
			break;
		case 2: // 0-9
			randStr.append(1, '0' + rand() % 10);
			break;
		case 3: // 特殊字符
			randStr.append(1, cs[rand() % strlen(cs)]);
			break;
		default:
			break;
		}
	}
	return randStr;
}
