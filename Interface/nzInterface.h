#pragma once
#include <string>



using namespace std;

class nzInterface
{
public:

	nzInterface(string json);

	~nzInterface();

	string encData(string& data);

	string decData(string& data);



private:
	string key;
	int maxNode;
	string m_key;
	int status;
	int m_SecKeyID;


};

