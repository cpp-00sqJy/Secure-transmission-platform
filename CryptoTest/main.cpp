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


	string str = "�ñ�̸ı����� - good good study, day day up!!!";
	cout << "ԭʼ����: " << str << endl;
	str = rsa.Rsa_Public_encrypt(str);
	cout << "���ܺ������:" << str << endl;
	str = rsa.Rsa_Private_decrypt(str);
	cout << "���ܺ������:" << str << endl;

	string signData = rsa.Rsa_Sign(str);
	bool b = rsa.Rsa_Verify(str, signData);
	cout << "Verify = " << b << endl;*/

	string text = "ע������A�Ĺ��캯�����ڲ����б��ͳ�ʼ���б�ǰ������try�ؼ��֣�Ȼ���캯���ͱ��ָ�Ϊ�������֣�ǰ���ǳ�ʼ���������ǳ�ʼ��ʱ�Ĵ�������Ҫָ�����ǣ�catch�����沶�񵽵��쳣���ܱ����ԣ���catch���б�����һ��throw��������׳��쳣�����û�У���Ĭ�ϻὫԭ�����񵽵��쳣�����׳������һ�����Ϊ�ǲ�ͬ�ġ���������������п��Է���A�Ὣ���񵽵��쳣ԭ�ⲻ���׳�";
	string key = "12345678abcdefgh";
	try {
		AesCrypto aes(key);
		cout << "Դ���� :" << text << endl;
		string ret = aes.aesCBCEncData(text);
		ret = aes.aesCBCDecData(ret);
		cout << "��������: " << ret << endl;
	}
	catch(exception e)
	{
		cout << "exception: " << e.what() << endl;
	}
	

	return 0;
}