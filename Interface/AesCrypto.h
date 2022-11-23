#pragma once
#include <openssl/aes.h>
#include <string>
using namespace std;
class AesCrypto
{
public:

	//ѡ����Կ  --�� ���� 16 24 32
	AesCrypto(string key);

	~AesCrypto();

	//CBC���ܺ������
	string aesCBCEncData(string Data);
	//CBC���ܺ������
	string aesCBCDecData(string decData);

private:

	//��ȡ��ʼ������
	void getIvec(unsigned char * ivec);
	//�ӽ���
	string aesCBCCrypto(string data, int cryptoType);

private:

	string userKey;//��Կ

	AES_KEY enckey; // ������Կ

	AES_KEY deckey;// ������Կ

};

