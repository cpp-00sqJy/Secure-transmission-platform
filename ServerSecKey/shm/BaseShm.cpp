#include "BaseShm.h"
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
const char RandX = 'a';
//key值打开共享内存
BaseShm::BaseShm(int key)
{
	getShmID(key, 0, 0);

}
//创建共享内存
BaseShm::BaseShm(int key, int size)
{
	getShmID(key, size, IPC_CREAT | 0644);

}
//路径打开共享内存  ---->ftok()
BaseShm::BaseShm(string name)
{
	key_t t = ftok(name.data(), RandX);
	getShmID(t, 0, 0);

}
//ftok 创建共享内存
BaseShm::BaseShm(string name, int size)
{
	key_t t = ftok(name.data(), RandX);
	getShmID(t, size, IPC_CREAT | 0644);

}
//进程关联共享内存
void* BaseShm::mapShm()
{
	this->m_shmAddr = shmat(this->m_shmID, NULL, 0);
	if (this->m_shmAddr == (void*)-1)
	{
		return NULL;
	}
	return this->m_shmAddr;
}
//断开关联
int BaseShm::unmapShm()
{
	int ret = shmdt(this->m_shmAddr);
	if (-1 == ret)
	{

		//logAddr->setEnableLevel(logAddr->ERROR);
		logAddr->Log("shmdt err...", __FILE__, __LINE__);
		//cout << "shmdt err..." << endl;
	}
	return ret;
}
//删除共享内存
int BaseShm::delShm()
{
	int ret = shmctl(this->m_shmID, IPC_RMID, NULL);
	if (-1 == ret)
	{
		logAddr->Log("shmctl err...", __FILE__, __LINE__);
		//cout << "shmctl err..." << endl;
	}
	return ret;
}

BaseShm::~BaseShm()
{
}
//创建共享内存 返回 shmID
int BaseShm::getShmID(key_t key, int shmSize, int flag)
{
	//cout << "share memory size: " << shmSize << endl;
	this->m_shmID = shmget(key, shmSize, flag);
	if (-1 == this->m_shmID)
	{
		logAddr->Log("shmget err...", __FILE__, __LINE__);
		// cout << "shmget 失败" << endl;
	}
	return  this->m_shmID;
}
