#include "Hash.h"
#include "RsaCrypto.h"
#include "AesCrypto.h"

int main()
{

	/*Hash hash(T_SHA512);

	hash.add_Data("hello");

	string data = hash.result();

	cout << "data = " << data << endl;*/
	
	/*RsaCrypto rsa;
	rsa.GeneratingKeyPair(2048);


	string str = "让编程改变世界 - good good study, day day up!!!";
	cout << "原始数据: " << str << endl;
	str = rsa.Rsa_Public_encrypt(str);
	cout << "加密后的数据:" << str << endl;
	str = rsa.Rsa_Private_decrypt(str);
	cout << "解密后的数据:" << str << endl;

	string signData = rsa.Rsa_Sign(str);
	bool b = rsa.Rsa_Verify(str, signData);
	cout << "Verify = " << b << endl;*/

	string text = "注意上面A的构造函数，在参数列表后和初始化列表前增加了try关键字，然后构造函数就被分割为了两部分，前面是初始化，后面是初始化时的错误处理。需要指出的是，catch块里面捕获到的异常不能被忽略，即catch块中必须有一个throw语句重新抛出异常，如果没有，则默认会将原来捕获到的异常重新抛出，这和一般的行为是不同的。例如下面代码运行可以发现A会将捕获到的异常原封不动抛出";
	string key = "12345678abcdefgh";
	try {
		AesCrypto aes(key);
		cout << "源数据 :" << text << endl;
		string ret = aes.aesCBCEncData(text);
		ret = aes.aesCBCDecData(ret);
		cout << "解密数据: " << ret << endl;
	}
	catch(exception e)
	{
		cout << "exception: " << e.what() << endl;
	}
	

	return 0;
}