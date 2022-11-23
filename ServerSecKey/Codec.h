#pragma once
#include <iostream>
#include"Message.pb.h"
using namespace std;
#include<string>


class Codec
{
public:
	Codec();
	virtual ~Codec();
	virtual string encodeMsg();
	virtual void* decodeMsg();
};

