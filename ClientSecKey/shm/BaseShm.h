#pragma once
#include<iostream>
using namespace std;

/*封装的共享内存类*/

class BaseShm
{
public:
	//key值打开共享内存
	BaseShm(int key);
	//创建共享内存
	BaseShm(int key, int size);
	//路径打开共享内存  ---->ftok()
	BaseShm(string name);
	//ftok 创建共享内存
	BaseShm(string name, int size);
	//进程关联共享内存
	void *mapShm();
	//断开关联
	int unmapShm();
	//删除共享内存
	int delShm();

	~BaseShm();

private:
	//创建共享内存 返回 shmID
	int getShmID(key_t key,int shmSize,int flag );
protected:
	//共享内存首地址
	void* m_shmAddr;
private:
	//共享内存返回的 ID
	int m_shmID;
};

