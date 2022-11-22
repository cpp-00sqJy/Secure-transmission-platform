#include "ClientOP.h"
#include <fstream>
#include <sstream>
#include <json/json.h>
#include "RequestCodec.h"
#include "RequestFactory.h"
#include "RsaCrypto.h"
#include "TcpSocket.h"
#include "RespondFactory.h"
#include "RespondCodec.h"
#include "Message.pb.h"
#include "Hash.h"
#include "SecKeyNodeInfo.h"
#include "SecKeyShm.h"
using namespace Json;
using namespace std;

ClientOP::ClientOP(string jsonFile)
{
	ifstream ifs(jsonFile);
	Value root;
	Reader r;
	r.parse(ifs, root);

	this->m_Info.serverID = root["ServerID"].asString();
	this->m_Info.clientID = root["ClientID"].asString();
	this->m_Info.ip = root["ServerIP"].asString();
	this->m_Info.port = root["Port"].asInt();

	//cout << "serverID : " << m_Info.serverID << ", clientID: " << m_Info.clientID << ", IP: " << this->m_Info.ip << ", 端口:" << this->m_Info.port << endl;
	string ShmKey = root["ShmKey"].asString();
	int maxNode = root["MaxNode"].asInt();

	m_shm = new SecKeyShm(ShmKey, maxNode);
}

ClientOP::~ClientOP()
{
	delete m_shm;
}

bool ClientOP::seckeyAgree()
{
	/*生成非对称加密秘钥对*/
	RsaCrypto rsa;
	rsa.GeneratingKeyPair(1024);

	/*读取公钥*/
	ifstream ifs("public.pem");
	stringstream str;
	str << ifs.rdbuf();

	/*初始化客户端结构体*/
	Requestinfo reqInfo;
	reqInfo.cmd = 1;                              /*选择功能类型  1 ----->秘钥协商*/
	reqInfo.serverID = this->m_Info.serverID;
	reqInfo.clientID = this->m_Info.clientID;
	reqInfo.data = str.str();					 /*非对称加密的公钥*/

	Hash has(T_SHA1);							/*对公钥进行签名*/
	has.add_Data(str.str());
	reqInfo.sign = rsa.Rsa_Sign(has.result());				/*公钥的签名*/
	cout << "签名完成...." << endl;

	/*工厂类创建*/
	CodecFactory* factory = new RequestFactory(&reqInfo);

	Codec* codec = factory->createCodec();

	/*序列化后的数据*/
	string encStr = codec->encodeMsg();

	/*释放资源*/
	delete factory;
	delete codec;

	/*连接服务器*/
	TcpSocket* tcp = new TcpSocket;
	int ret = tcp->conToHost(this->m_Info.ip, this->m_Info.port);

	if (ret != 0)
	{
		cout << "connect err or TIMEOUT..." << endl;
		return false;
	}
	cout << "connect Sever success..." << endl;
	/*发送序列化后的数据*/
	tcp->sendMsg(encStr);

	/*接受服务器回复的数据*/
	string SerMsg = tcp->recvMsg();

	/*对数据进行反序列化 ------> 解析 */
	factory = new RespondFactory(SerMsg);

	codec = factory->createCodec();

	RespondMsg* res = (RespondMsg*)codec->decodeMsg();

	if (!res->rv())
	{
		cout << "秘钥协商失败..." << endl;
		return false;
	}

	string key = rsa.Rsa_Private_decrypt(res->data());
	cout << "对称加密的秘钥: " << key << endl;

	SecKeyNodeInfo node;
	strcpy(node.clientID, m_Info.clientID.data());
	strcpy(node.serverID, m_Info.serverID.data());
	strcpy(node.seckey, key.data());
	node.seckeyID = res->seckeyid();
	node.status = 1;
	m_shm->shmWrite(&node);

	delete factory;
	delete codec;

	tcp->disconnect();
	delete tcp;


	return true;
}

bool ClientOP::seckeyCheck()
{

	//RsaCrypto rsa;
	nodeInfo = m_shm->shmFirstNode();
	cout << "共享内存存储的秘钥: " << nodeInfo.seckey << endl;
	string Seckey = string(nodeInfo.seckey);

	if (nodeInfo.status == 0)
	{
		cout << "秘钥已被注销,请重新生成新的秘钥..." << endl;
		return false;
	}
	Hash sha1(T_SHA1);
	sha1.add_Data(Seckey);

	string sha1_Seckey = sha1.result();

	Requestinfo req;
	req.clientID = m_Info.clientID;
	req.serverID = m_Info.serverID;
	req.cmd = 2;
	//req.data = Seckey;
	req.sign = sha1_Seckey;

	CodecFactory* factory = new RequestFactory(&req);

	Codec* codec = factory->createCodec();

	string encStr = codec->encodeMsg();

	delete factory;
	delete codec;

	/*连接服务器*/
	TcpSocket* tcp = new TcpSocket;
	int ret = tcp->conToHost(this->m_Info.ip, this->m_Info.port);

	if (ret != 0)
	{
		cout << "connect err or TIMEOUT..." << endl;
		return false;
	}
	cout << "connect Sever success..." << endl;

	/*发送序列化后的数据*/
	tcp->sendMsg(encStr);

	/*接受服务器回复的数据*/
	string SerMsg = tcp->recvMsg();

	/*对数据进行反序列化 ------> 解析 */
	factory = new RespondFactory(SerMsg);

	codec = factory->createCodec();

	RespondMsg* res = (RespondMsg*)codec->decodeMsg();

	if (!res->rv())
	{
		cout << "秘钥校验失败..." << endl;
		return false;
	}
	else
	{
		cout << "秘钥校验成功..." << endl;
		delete factory;
		delete codec;

		tcp->disconnect();
		delete tcp;

		return true;
	}

	/*else
	{
		cout << "秘钥已被注销,请重新生成新的秘钥.." << endl;
		return false;

	}*/


}

bool ClientOP::seckeyZhuXiao()
{

	nodeInfo = m_shm->shmFirstNode();

	if (nodeInfo.status == 0)
	{
		cout << "秘钥已被注销..." << endl;
		return false;
	}
	nodeInfo.status = 0;
	strcpy(nodeInfo.clientID, m_Info.clientID.data());
	strcpy(nodeInfo.serverID, m_Info.serverID.data());

	m_shm->shmWrite(&nodeInfo);

	Requestinfo req;
	req.clientID = m_Info.clientID;
	req.serverID = m_Info.serverID;
	req.cmd = 3;
	req.seckeyid = nodeInfo.seckeyID;

	cout << "SeckeyId = " << req.seckeyid << endl;

	CodecFactory* factory = new RequestFactory(&req);
	Codec* codec = factory->createCodec();
	string reqencStr = codec->encodeMsg();

	delete factory;
	delete codec;

	/*连接服务器*/
	TcpSocket* tcp = new TcpSocket;
	int ret = tcp->conToHost(this->m_Info.ip, this->m_Info.port);


	if (ret != 0)
	{
		cout << "connect err or TIMEOUT..." << endl;
		return false;
	}
	cout << "connect Sever success..." << endl;

	/*发送序列化后的数据*/
	tcp->sendMsg(reqencStr);

	/*接受服务器回复的数据*/
	string SerMsg = tcp->recvMsg();

	/*对数据进行反序列化 ------> 解析 */
	factory = new RespondFactory(SerMsg);

	codec = factory->createCodec();

	RespondMsg* res = (RespondMsg*)codec->decodeMsg();

	if (!res->rv())
	{
		cout << "秘钥注销失败..." << endl;
		return false;
	}
	else
	{
		cout << "秘钥注销成功..." << endl;
		delete factory;
		delete codec;

		tcp->disconnect();
		delete tcp;

		return true;
	}


	/*else
	{
		cout << "秘钥已被注销...." << endl;
		return false;
	}*/


}

bool ClientOP::findSeckey()
{




}

