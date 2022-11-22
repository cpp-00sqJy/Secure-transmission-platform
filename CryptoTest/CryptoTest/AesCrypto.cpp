#include "AesCrypto.h"

/*
  @ 初始化秘钥长度
  @ key : 秘钥
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
  @ 加密函数
  @ Data : 要加密的数据
  @ 返回值: 加密后的数据
*/
string AesCrypto::aesCBCEncData(string Data)
{

    return aesCBCCrypto(Data, AES_ENCRYPT);
}
/*
  @ 解密函数
  @ decData : 要解密的数据
  @ 返回值: 解密后的数据
*/
string AesCrypto::aesCBCDecData(string decData)
{

    return aesCBCCrypto(decData, AES_DECRYPT);
}
/*
  @ 初始化向量
  @ ivec : 16个字节数据
*/
void AesCrypto::getIvec(unsigned char* ivec)
{
    for (int i = 0; i < AES_BLOCK_SIZE; ++i)
    {
        ivec[i] = this->userKey.at(AES_BLOCK_SIZE - i - 1);
    }
}
/*
  @ 加解密函数
  @ data: 加密/解密 的数据
  @ cryptoType: 加密/解密类型
  @ 返回值: 加密/解密 数据
*/
string AesCrypto::aesCBCCrypto(string data, int cryptoType)
{

    AES_KEY* key = cryptoType == AES_ENCRYPT ? &this->enckey : &this->deckey;

    int length = data.size() + 1;
    
    //数据长度取模不等于0 ,需要填充
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
