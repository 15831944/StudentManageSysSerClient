#ifndef __CHECKTOOL_H__
#define __CHECKTOOL_H__

#include <iostream>
#include <string>
#include <vector>

#include "PublicDef.h"

using namespace std;

class CheckTool
{
public:
	CheckTool();
	~CheckTool();

	static bool CheckStringLen(string str, unsigned len);

	static vector<string> Splite(string str, string strSpl="|");

	static string CombVecToStr(vector<OperPermission> vecOper, string strCom="|");
	static string CombVecToStr(vector<int> vec, string strCom="|");

	//validStr�ǺϷ��ַ������÷ָ���|��������ʱֻ���ж�ֻ�ܺ�����Щ���ǺϷ��ģ���û�к��ж��ٺ��ھ���λ�ó���
	static bool CheckStringByValid(string str, string validStr);

	static string ToLowercase(string strSrc);
	static string ToUppecase(string strSrc);

	static string NumberToStr(int n);
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