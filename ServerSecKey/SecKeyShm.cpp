#include "SecKeyShm.h"



SecKeyShm::SecKeyShm(int key, int maxNode) :
	BaseShm(key, maxNode * sizeof(SecKeyNodeInfo)), m_maxNode(maxNode)
{

}



SecKeyShm::SecKeyShm(string pathname, int maxNode)
	:BaseShm(pathname, maxNode * sizeof(SecKeyNodeInfo)), m_maxNode(maxNode)
{


}

SecKeyShm::~SecKeyShm()
{

}

int SecKeyShm::shmWrite(SecKeyNodeInfo* pNodeInfo)
{
	int ret = -1;

	SecKeyNodeInfo* pAddr = static_cast<SecKeyNodeInfo*>(mapShm());
	if (pAddr == NULL)
	{
		return ret;
	}
	/* 判断传入的秘钥是否已经存在, 存在则覆盖原始秘钥 */
	SecKeyNodeInfo* pNode = NULL;
	for (int i = 0; i < m_maxNode; i++)
	{
		pNode = pAddr + i;
		cout << i << endl;
		cout << "clientID 比较: " << pNode->clientID << ", " << pNodeInfo->clientID << endl;
		cout << "serverID 比较: " << pNode->serverID << ", " << pNodeInfo->serverID << endl;
		cout << endl;
		if (strcmp(pNode->clientID, pNodeInfo->clientID) == 0 &&
			strcmp(pNode->serverID, pNodeInfo->serverID) == 0)
		{

			memcpy(pNode, pNodeInfo, sizeof(SecKeyNodeInfo));
			unmapShm();
			cout << "写数据成功: 原数据被覆盖!" << endl;
			return 0;
		}

	}

	/* 无原始数据 ,找空结点*/
	int j;
	SecKeyNodeInfo emptyNode;
	for (j = 0; j < m_maxNode; j++)
	{
		pNode = pAddr + j;
		if (memcmp(&emptyNode, pNode, sizeof(SecKeyNodeInfo)) == 0)
		{
			ret = 0;
			memcpy(pNode, pNodeInfo, sizeof(SecKeyNodeInfo));
			cout << "写数据成功: 在新的节点上添加数据!" << endl;
			break;
		}

	}
	/* 共享内存已满 返回 -1 */
	if (j == m_maxNode)
	{
		ret = -1;
	}
	unmapShm();
	return ret;
}

SecKeyNodeInfo SecKeyShm::shmRead(string clientID, string serverID)
{
	SecKeyNodeInfo* pAddr = static_cast<SecKeyNodeInfo*>(mapShm());
	if (pAddr == NULL)
	{
		cout << "共享内存关联失败..." << endl;
		return SecKeyNodeInfo();
	}
	cout << "共享内存关联成功..." << endl;
	int i;
	SecKeyNodeInfo* pNode = NULL;
	SecKeyNodeInfo info;
	for (i = 0; i < m_maxNode; i++)
	{
		pNode = pAddr + i;
		cout << i << endl;
		cout << "clientID 比较: " << pNode->clientID << ", " << clientID.data() << endl;
		cout << "serverID 比较: " << pNode->serverID << ", " << serverID.data() << endl;
		if (strcmp(pNode->clientID, clientID.data()) == 0 &&
			strcmp(pNode->serverID, serverID.data()) == 0)
		{
			info = *pNode;
			cout << "+++++++++++++++++++++++++++++++++++++" << endl;

			cout << info.clientID << " , " << info.serverID << ", "
				<< info.seckeyID << ", " << info.status << ", "
				<< info.seckey << endl;
			cout << "=====================================" << endl;

			break;
		}
	}

	return	info;
}

SecKeyNodeInfo SecKeyShm::shmRead(int keyID)
{

	SecKeyNodeInfo* pAddr = static_cast<SecKeyNodeInfo*>(mapShm());
	if (pAddr == NULL)
	{
		cout << "共享内存关联失败..." << endl;
		return SecKeyNodeInfo();
	}
	cout << "共享内存关联成功..." << endl;
	int i;
	SecKeyNodeInfo* pNode = NULL;
	SecKeyNodeInfo info;
	for (i = 0; i < m_maxNode; i++)
	{
		pNode = pAddr + i;
		/*cout << i << endl;*/
		/*cout << "clientID 比较: " << pNode->clientID << ", " << clientID.data() << endl;
		cout << "serverID 比较: " << pNode->serverID << ", " << serverID.data() << endl;*/
		if (pNode->seckeyID == keyID)
		{
			info = *pNode;
			cout << "+++++++++++++++++++++++++++++++++++++" << endl;
			cout << info.clientID << " , " << info.serverID << ", "
				<< info.seckeyID << ", " << info.status << ", "
				<< info.seckey << endl;
			cout << "=====================================" << endl;
			break;
		}
	}

	return	info;

}

void SecKeyShm::shmInit()
{
	if (m_shmAddr != NULL)
	{
		memset(m_shmAddr, 0, m_maxNode * sizeof(SecKeyNodeInfo));
	}
}
