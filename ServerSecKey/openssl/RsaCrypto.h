#pragma once
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <string>
#include "../log/Logger.h"
using namespace std;
enum HashType1
{
	type1 = NID_md5,
	type2 = NID_sha1,
	type3 = NID_sha224,
	type4 = NID_sha256,
	type5 = NID_sha384,
	type6 = NID_sha512
};

class RsaCrypto
{

public:

	RsaCrypto();

	RsaCrypto(string fliename, bool isPrivate = true);

	~RsaCrypto();

	//生成密钥对
	void GeneratingKeyPair(int bits, string pub = "public.pem", string pri = "private.pem");
	//公钥加密
	string Rsa_Public_encrypt(string enData);
	//私钥解密
	string Rsa_Private_decrypt(string deData);
	//数据签名
	string Rsa_Sign(string signData, HashType1 type = type3);
	//验证签名
	bool Rsa_Verify(string data, string signData, HashType1 type = type3);

private:
	//编码bash64格式
	string toBase64(const char* str, int len);
	//解码还原二进制
	char* fromBase64(string decData);
	// 得到公钥
	bool initPublicKey(string pubfile);
	// 得到私钥
	bool initPrivateKey(string prifile);
private:
	Logger* logAddr = Logger::getInstance();
	//公钥
	RSA* m_publicKey;
	//私钥
	RSA* m_privateKey;

};

