#include "Hash.h"

Hash::Hash(HashType type)
{
	this->m_type = type;
	switch (type)
	{
	case T_MD5:
		this->mD5_Init();
		break;
	case T_SHA1:
		this->sha1_Init();
		break;
	case T_SHA224:
		this->sha224_Init();
		break;
	case T_SHA256:
		this->sha256_Init();
		break;
	case T_SHA384:
		this->sha384_Init();
		break;
	case T_SHA512:
		this->sha512_Init();
		break;
	default:
		this->mD5_Init();
		break;
	}
}

Hash::~Hash()
{

}

void Hash::add_Data(string data)
{
	switch (this->m_type)
	{
	case T_MD5:
		this->mD5_addData(data.data());
		break;
	case T_SHA1:
		this->sha1_addData(data.data());
		break;
	case T_SHA224:
		this->sha224_addData(data.data());
		break;
	case T_SHA256:
		this->sha256_addData(data.data());
		break;
	case T_SHA384:
		this->sha384_addData(data.data());
		break;
	case T_SHA512:
		this->sha512_addData(data.data());
		break;
	default:
		this->mD5_addData(data.data());
		break;
	}
}

string Hash::result()
{
	string result = string();
	switch (this->m_type)
	{
	case T_MD5:
		result=this->md5_result();
		break;
	case T_SHA1:
		result = this->sha1_result();
		break;
	case T_SHA224:
		result = this->sha224_result();
		break;
	case T_SHA256:
		result = this->sha256_result();
		break;
	case T_SHA384:
		result = this->sha384_result();
		break;
	case T_SHA512:
		result = this->sha512_result();
		break;
	default:
		result = this->md5_result();
		break;
	}


    return result;
}

string Hash::md5_result()
{
	unsigned char md[MD5_DIGEST_LENGTH];
	char res[MD5_DIGEST_LENGTH * 2 + 1];
	MD5_Final(md, &m_md5);
	for (int i = 0; i < MD5_DIGEST_LENGTH; ++i)
	{
		sprintf(&res[i * 2], "%02x", md[i]);
	}
	return string(res, MD5_DIGEST_LENGTH * 2 + 1);
}

string Hash::sha1_result()
{
	unsigned char md[SHA_DIGEST_LENGTH];
	char res[SHA_DIGEST_LENGTH * 2 + 1];
	SHA1_Final(md, &this->m_sha1);
	for (int i = 0; i < SHA_DIGEST_LENGTH; ++i)
	{
		sprintf(&res[i * 2], "%02x", md[i]);
	}

	return string(res, SHA_DIGEST_LENGTH * 2 + 1);
}

string Hash::sha224_result()
{

	unsigned char md[SHA224_DIGEST_LENGTH];
	char res[SHA224_DIGEST_LENGTH * 2 + 1];
	SHA224_Final(md, &m_sha224);
	for (int i = 0; i < SHA224_DIGEST_LENGTH; ++i)
	{
		sprintf(&res[i * 2], "%02x", md[i]);
	}
	return string(res, SHA224_DIGEST_LENGTH * 2 + 1);
}

string Hash::sha256_result()
{
	unsigned char md[SHA256_DIGEST_LENGTH];
	char res[SHA256_DIGEST_LENGTH * 2 + 1];
	SHA256_Final(md, &m_sha256);
	for (int i = 0; i < SHA256_DIGEST_LENGTH; ++i)
	{
		sprintf(&res[i * 2], "%02x", md[i]);
	}
	return string(res, SHA256_DIGEST_LENGTH * 2 + 1);
}

string Hash::sha384_result()
{
	unsigned char md[SHA384_DIGEST_LENGTH];
	char res[SHA384_DIGEST_LENGTH * 2 + 1];
	SHA384_Final(md, &m_sha384);
	for (int i = 0; i < SHA384_DIGEST_LENGTH; ++i)
	{
		sprintf(&res[i * 2], "%02x", md[i]);
	}
	return string(res, SHA384_DIGEST_LENGTH * 2 + 1);
}

string Hash::sha512_result()
{
	unsigned char md[SHA512_DIGEST_LENGTH];
	char res[SHA512_DIGEST_LENGTH * 2 + 1];
	SHA512_Final(md, &m_sha512);
	for (int i = 0; i < SHA512_DIGEST_LENGTH; ++i)
	{
		sprintf(&res[i * 2], "%02x", md[i]);
	}
	return string(res, SHA512_DIGEST_LENGTH * 2 + 1);
	return string();
}
