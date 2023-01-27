#include "OCCIOP.h"
#include <iostream>
#include <string.h>
#include <time.h>
using namespace std;
using namespace oracle::occi;

OCCIOP::OCCIOP()
{
}

OCCIOP::~OCCIOP()
{
}

bool OCCIOP::connectDB(string username, string password, string hostaddr)
{
	this->env = Environment::createEnvironment("utf8", "utf8");

	this->conn = env->createConnection(username, password, hostaddr);
	if (conn == NULL)
	{
		return false;
	}

	log->Log("数据库连接成功... ", __FILE__, __LINE__);
	//cout << "数据库连接成功" << endl;
	return true;
}

int OCCIOP::getKeyID()
{
	string sql = "select ikeysn from SECMNG.KEYSN for update";

	Statement* stat = conn->createStatement(sql);

	ResultSet* result = stat->executeQuery();

	int keyID = -1;
	if (result->next())
	{
		keyID = result->getInt(1);
	}
	stat->closeResultSet(result);

	conn->terminateStatement(stat);

	return keyID;
}

bool OCCIOP::updataKeyID(int keyID)
{
	// 更新数据库
	string sql = "update SECMNG.KEYSN set ikeysn = " + to_string(keyID);
	Statement* stat = conn->createStatement(sql);
	// 设置自动提交
	stat->setAutoCommit(true);
	// 执行数据查询
	int ret = stat->executeUpdate();
	conn->terminateStatement(stat);
	if (ret <= 0)
	{
		return false;
	}
	return true;
}

bool OCCIOP::writeSecKey(SecKeyNodeInfo* pNode)
{
	char sql[1024] = { 0 };
	sprintf(sql, "Insert Into SECMNG.SECKEYINFO(clientid, serverid, keyid, createtime, state, seckey) \
					values ('%s', '%s', %d, to_date('%s', 'yyyy-mm-dd hh24:mi:ss') , %d, '%s') ",
		pNode->clientID, pNode->serverID, pNode->seckeyID,
		getCurTime().data(), 1, pNode->seckey);
	//cout << "insert sql: " << sql << endl;
	Statement* stat = conn->createStatement();

	stat->setSQL(sql);

	stat->setAutoCommit(true);

	bool b1 = stat->executeUpdate(sql);

	conn->terminateStatement(stat);

	if (b1)
	{
		return true;
	}

	return false;
}

void OCCIOP::closeDB()
{
	env->terminateConnection(conn);
	Environment::terminateEnvironment(env);
}

bool OCCIOP::updateKeyStatus(int keyID)
{

	char sql[1024] = { 0 };
	sprintf(sql, "update SECMNG.SECKEYINFO SET STATE = %d where KEYID = %d", 0, keyID);

	Statement* stat = conn->createStatement();

	stat->setSQL(sql);

	stat->setAutoCommit(true);

	bool b1 = stat->executeUpdate(sql);

	conn->terminateStatement(stat);

	if (b1)
	{
		return true;
	}

	return false;
}

string OCCIOP::getCurTime()
{
	time_t timep;
	time(&timep);
	char tmp[64];
	strftime(tmp, sizeof(tmp), "%Y-%m-%d %H:%M:%S", localtime(&timep));

	return tmp;
}
