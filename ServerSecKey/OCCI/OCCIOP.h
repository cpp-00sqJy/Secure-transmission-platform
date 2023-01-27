#pragma once
#include "SecKeyShm.h"
#include <occi.h>
#include <string>
using namespace std;
using namespace oracle::occi;



class OCCIOP
{
public:

	OCCIOP();

	~OCCIOP();

	bool connectDB(string username, string password, string hostaddr);

	int getKeyID();

	bool updataKeyID(int keyID);

	bool writeSecKey(SecKeyNodeInfo* pNode);

	void closeDB();

	bool updateKeyStatus(int keyID);



private:
	//获取当前时间
	string getCurTime();

private:
	Logger* log = Logger::getInstance();
	Environment* env;
	Connection* conn;
};

