#pragma once
#include <openssl/md5.h>
#include <openssl/sha.h>
#include <iostream>
#include <string.h>
using namespace std;

/*哈希算法类型*/
enum HashType
{
	T_MD5,
	T_SHA1,
	T_SHA224,
	T_SHA256,
	T_SHA384,
	T_SHA512
};


class Hash
{
public:
	//根据哈希算法类型进行初始化
	Hash(HashType type);
	
	~Hash();

	void add_Data(string data);

	string result();
private:
	//初始化MD5
	inline void mD5_Init(){ MD5_Init(&m_md5);}
	inline void mD5_addData(const char* data)
	{
		MD5_Update(&m_md5,data,strlen(data));
	}
	string md5_result();

	inline void sha1_Init() { SHA1_Init(&m_sha1); }
	inline void sha1_addData(const char* data)
	{
		SHA1_Update(&m_sha1, data, strlen(data));
	}
	string sha1_result();

	inline void sha224_Init() { SHA224_Init(&m_sha224); }
	inline void sha224_addData(const char* data)
	{
		SHA224_Update(&m_sha224, data, strlen(data));
	}
	string sha224_result();

	inline void sha256_Init() { SHA256_Init(&m_sha256); }
	inline void sha256_addData(const char* data)
	{
		SHA256_Update(&m_sha256, data, strlen(data));
	}
	string sha256_result();

	inline void sha384_Init() { SHA384_Init(&m_sha384); }
	inline void sha384_addData(const char* data)
	{
		SHA384_Update(&m_sha384, data, strlen(data));
	}
	string sha384_result();

	inline void sha512_Init() { SHA512_Init(&m_sha512); }
	inline void sha512_addData(const char* data)
	{
		SHA512_Update(&m_sha512, data, strlen(data));
	}
	string sha512_result();
private:

	HashType m_type;
	MD5_CTX m_md5;
	SHA_CTX m_sha1;
	SHA256_CTX m_sha224;
	SHA256_CTX m_sha256;
	SHA512_CTX m_sha384;
	SHA512_CTX m_sha512;

};

