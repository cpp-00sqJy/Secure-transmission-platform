#pragma once
#include<iostream>
using namespace std;

/*��װ�Ĺ����ڴ���*/

class BaseShm
{
public:
	//keyֵ�򿪹����ڴ�
	BaseShm(int key);
	//���������ڴ�
	BaseShm(int key, int size);
	//·���򿪹����ڴ�  ---->ftok()
	BaseShm(string name);
	//ftok ���������ڴ�
	BaseShm(string name, int size);
	//���̹��������ڴ�
	void *mapShm();
	//�Ͽ�����
	int unmapShm();
	//ɾ�������ڴ�
	int delShm();

	~BaseShm();

private:
	//���������ڴ� ���� shmID
	int getShmID(key_t key,int shmSize,int flag );
protected:
	//�����ڴ��׵�ַ
	void* m_shmAddr;
private:
	//�����ڴ淵�ص� ID
	int m_shmID;
};

