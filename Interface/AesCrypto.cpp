#include "AesCrypto.h"

/*
  @ ��ʼ����Կ����
  @ key : ��Կ
*/
AesCrypto::AesCrypto(string key)
{
    if (key.size()==16||key.size()==24||key.size()==32)
    {
        unsigned char* Usekey = (unsigned char*)key.data();
        AES_set_encrypt_key(Usekey, key.size() * 8, &this->enckey);
        AES_set_decrypt_key(Usekey, key.size() * 8, &this->deckey);
        this->userKey = key;
    }
}

AesCrypto::~AesCrypto()
{

}
/*
  @ ���ܺ���
  @ Data : Ҫ���ܵ�����
  @ ����ֵ: ���ܺ������
*/
string AesCrypto::aesCBCEncData(string Data)
{

    return aesCBCCrypto(Data, AES_ENCRYPT);
}
/*
  @ ���ܺ���
  @ decData : Ҫ���ܵ�����
  @ ����ֵ: ���ܺ������
*/
string AesCrypto::aesCBCDecData(string decData)
{

    return aesCBCCrypto(decData, AES_DECRYPT);
}
/*
  @ ��ʼ������
  @ ivec : 16���ֽ�����
*/
void AesCrypto::getIvec(unsigned char* ivec)
{
    for (int i = 0; i < AES_BLOCK_SIZE; ++i)
    {
        ivec[i] = this->userKey.at(AES_BLOCK_SIZE - i - 1);
    }
}
/*
  @ �ӽ��ܺ���
  @ data: ����/���� ������
  @ cryptoType: ����/��������
  @ ����ֵ: ����/���� ����
*/
string AesCrypto::aesCBCCrypto(string data, int cryptoType)
{

    AES_KEY* key = cryptoType == AES_ENCRYPT ? &this->enckey : &this->deckey;

    int length = data.size() + 1;
    
    //���ݳ���ȡģ������0 ,��Ҫ���
    if (length % 16 )
    {
        length = ((length / 16) + 1) * 16;
    }
    char* out = new  char[length];

    unsigned char ivec[AES_BLOCK_SIZE];

    getIvec(ivec);

    AES_cbc_encrypt((const unsigned char*)data.data(), (unsigned char *)out, length, key, ivec, cryptoType);

    string retStr = string(out,length);

    delete[] out;
    return string(retStr);
}
