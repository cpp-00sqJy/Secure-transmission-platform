#include "BaseShm.h"
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
const char RandX = 'a';
//keyֵ�򿪹����ڴ�
BaseShm::BaseShm(int key)
{
    getShmID(key,0,0);
  
}
//���������ڴ�
BaseShm::BaseShm(int key, int size)
{
    getShmID(key, size, IPC_CREAT | 0644);
    
}
//·���򿪹����ڴ�  ---->ftok()
BaseShm::BaseShm(string name)
{
    key_t t = ftok(name.data(), RandX);
    getShmID(t, 0, 0);
   
}
//ftok ���������ڴ�
BaseShm::BaseShm(string name, int size)
{
    key_t t = ftok(name.data(), RandX);
    getShmID(t, size, IPC_CREAT|0644);
   
}
//���̹��������ڴ�
void* BaseShm::mapShm()
{
    this->m_shmAddr = shmat(this->m_shmID, NULL, 0);
    if (this->m_shmAddr == (void*)-1)
    {
        return NULL;
    }
    return this->m_shmAddr;
}
//�Ͽ�����
int BaseShm::unmapShm()
{
    int ret = shmdt(this->m_shmAddr);
    if (-1 == ret)
    {
        cout << "shmdt err..." << endl;
    }
    return ret;
}
//ɾ�������ڴ�
int BaseShm::delShm()
{
    int ret = shmctl(this->m_shmID,IPC_RMID,NULL);
    if (-1 == ret)
    {
        cout << "shmctl err..." << endl;
    }
    return ret;
}

BaseShm::~BaseShm()
{
}
//���������ڴ� ���� shmID
int BaseShm::getShmID(key_t key, int shmSize, int flag)
{
    cout << "share memory size: " << shmSize << endl;
    this->m_shmID = shmget(key, shmSize, flag);
    if (-1 == this->m_shmID)
    {
        cout << "shmget ʧ��" << endl;
    }
    return  this->m_shmID;
}
