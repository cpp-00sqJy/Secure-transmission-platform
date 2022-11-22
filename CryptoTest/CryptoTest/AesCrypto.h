#pragma once
#include <openssl/aes.h>
#include <string>
using namespace std;
class AesCrypto
{
public:

	//选择秘钥  --》 长度 16 24 32
	AesCrypto(string key);

	~AesCrypto();

	//CBC加密后的数据
	string aesCBCEncData(string Data);
	//CBC解密后的数据
	string aesCBCDecData(string decData);

private:

	//获取初始化向量
	void getIvec(unsigned char * ivec);
	//加解密
	string aesCBCCrypto(string data, int cryptoType);

private:

	string userKey;//秘钥

	AES_KEY enckey; // 加密秘钥

	AES_KEY deckey;// 解密秘钥

};

