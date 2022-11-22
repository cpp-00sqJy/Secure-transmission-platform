#include "RsaCrypto.h"
#include <openssl/bio.h>
#include <openssl/err.h>
#include <iostream>
#include <string.h>
//extern "C"
//{
//#include <openssl/applink.c>
//};


RsaCrypto::RsaCrypto()
{
	this->m_publicKey = RSA_new();
	this->m_privateKey = RSA_new();
}

RsaCrypto::RsaCrypto(string filename, bool isPrivate)
{
	this->m_publicKey = RSA_new();
	this->m_privateKey = RSA_new();
	if (isPrivate)
	{
		this->initPrivateKey(filename);

	}
	else
	{
		this->initPublicKey(filename);
	}
}

RsaCrypto::~RsaCrypto()
{
	RSA_free(this->m_privateKey);
	RSA_free(this->m_publicKey);
}

void RsaCrypto::GeneratingKeyPair(int bits, string pub, string pri)
{

	RSA* r = RSA_new();

	BIGNUM* bn = BN_new();
	BN_set_word(bn, 456787);

	RSA_generate_key_ex(r, bits, bn, NULL);

	BIO* pubBio = BIO_new_file(pub.data(), "w");

	PEM_write_bio_RSAPublicKey(pubBio, r);
	BIO_flush(pubBio);
	BIO_free(pubBio);

	BIO* priBio = BIO_new_file(pri.data(), "w");

	PEM_write_bio_RSAPrivateKey(priBio, r, NULL, NULL, 0, NULL, NULL);
	BIO_flush(priBio);
	BIO_free(priBio);

	this->m_publicKey = RSAPublicKey_dup(r);
	this->m_privateKey = RSAPrivateKey_dup(r);

	RSA_free(r);
	BN_free(bn);
}

string RsaCrypto::Rsa_Public_encrypt(string enData)
{
	cout << "加密数据长度: " << enData.size() << endl;

	int pubKeyLen = RSA_size(this->m_publicKey);
	cout << "pubKeyLen: " << pubKeyLen << endl;

	char* enCode = new  char[pubKeyLen + 1];

	int len = RSA_public_encrypt(enData.size(), (const unsigned char*)enData.data(), (unsigned char*)enCode, this->m_publicKey, RSA_PKCS1_PADDING);

	string retStr = string();
	if (len >= 0)
	{
		cout << "len: " << len << ", pubKeyLen: " << pubKeyLen << endl;
		retStr = toBase64(enCode, len);
		//retStr = string(enCode, len);
	}
	else
	{
		ERR_print_errors_fp(stdout);
	}
	delete[] enCode;
	return retStr;
}

string RsaCrypto::Rsa_Private_decrypt(string deData)
{
	char* text = fromBase64(deData);

	int priKeyLen = RSA_size(m_privateKey);

	char* decode = new  char[priKeyLen + 1];

	int len = RSA_private_decrypt(priKeyLen, (const unsigned char*)text, (unsigned char*)decode, m_privateKey, RSA_PKCS1_PADDING);
	string retStr = string();
	if (len >= 0)
	{
		retStr = string(decode, len);
	}
	else
	{
		cout << "私钥解密失败..." << endl;
		ERR_print_errors_fp(stdout);
	}
	delete[] decode;
	delete[] text;
	return retStr;
}

string RsaCrypto::Rsa_Sign(string signData, HashType1 type)
{
	unsigned int len;

	//char* sigret = new  char[priKeyLen + 1];
	char* sigret = new  char[1024];
	memset(sigret, 0, 1024);
	int ret = RSA_sign(type, (const unsigned char*)signData.data(), signData.size(), (unsigned char*)sigret, &len,
		m_privateKey);
	string strRet = string();
	if (len == -1)
	{
		ERR_print_errors_fp(stdout);
		//strRet = string(sigret, ret);
		
	}
	cout << "sign len: " << len << ", ret: " << ret << endl;
	strRet = toBase64(sigret, len);
	delete[] sigret;
	return strRet;
}

bool RsaCrypto::Rsa_Verify(string data, string signData, HashType1 type)
{
	// 验证签名
	int keyLen = RSA_size(m_publicKey);

	char* sign = fromBase64(signData);

	int ret = RSA_verify(type, (const unsigned char*)data.data(), (unsigned int)data.size(),
		(const unsigned char*)sign, keyLen, this->m_publicKey);
	delete[]sign;
	if (ret == -1)
	{
		ERR_print_errors_fp(stdout);
	}
	if (ret != 1)
	{
		return false;
	}

	return true;
}

string RsaCrypto::toBase64(const char* str, int len)
{

	BIO* mem = BIO_new(BIO_s_mem());
	BIO* bs64 = BIO_new(BIO_f_base64());
	// mem添加到bs64中
	bs64 = BIO_push(bs64, mem);
	// 写数据
	BIO_write(bs64, str, len);
	BIO_flush(bs64);
	// 得到内存对象指针
	BUF_MEM* memPtr;
	BIO_get_mem_ptr(bs64, &memPtr);
	string retStr = string(memPtr->data, memPtr->length - 1);
	BIO_free_all(bs64);
	return retStr;

}

char* RsaCrypto::fromBase64(string str)
{
	int length = str.size();
	BIO* bs64 = BIO_new(BIO_f_base64());
	BIO* mem = BIO_new_mem_buf(str.data(), length);
	BIO_push(bs64, mem);
	char* buffer = new char[length];
	memset(buffer, 0, length);
	BIO_read(bs64, buffer, length);
	BIO_free_all(bs64);

	return buffer;
}

bool RsaCrypto::initPublicKey(string pubfile)
{
	BIO* pubBio = BIO_new_file(pubfile.data(), "r");

	if (PEM_read_bio_RSAPublicKey(pubBio, &this->m_publicKey, NULL, NULL) == NULL)
	{
		ERR_print_errors_fp(stdout);
		return false;
	}

	BIO_free(pubBio);

	return true;

}

bool RsaCrypto::initPrivateKey(string prifile)
{
	BIO* priBio = BIO_new_file(prifile.data(), "r");

	if (PEM_read_bio_RSAPublicKey(priBio, &this->m_privateKey, NULL, NULL) == NULL)
	{
		ERR_print_errors_fp(stdout);
		return false;
	}

	BIO_free(priBio);

	return true;

}

