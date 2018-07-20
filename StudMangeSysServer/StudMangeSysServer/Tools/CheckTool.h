#ifndef __CHECKTOOL_H__
#define __CHECKTOOL_H__

#include <iostream>
#include <string>
#include <vector>

using namespace std;

class CheckTool
{
public:
	CheckTool();
	~CheckTool();

	static bool CheckStringLen(string str, unsigned len);

	//validStr�ǺϷ��ַ������÷ָ���|��������ʱֻ���ж�ֻ�ܺ�����Щ���ǺϷ��ģ���û�к��ж��ٺ��ھ���λ�ó���
	static bool CheckStringByValid(string str, string validStr);

	
private:
	static bool CheckValidString(vector<string> strVec);
	static bool CheckCharByValid(char ch, vector<string> &strVec);
	static int FindValidIndex(string str);


};

static bool CheckValidForNumber(char ch, string str);
static bool CheckValidForLowercase(char ch, string str);
static bool CheckValidForUppecase(char ch, string str);
static bool CheckValidForOneChar(char ch, string str);

#endif