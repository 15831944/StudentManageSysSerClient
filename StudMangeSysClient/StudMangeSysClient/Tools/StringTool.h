#ifndef __STRINGTOOL_H__
#define __STRINGTOOL_H__

#include <iostream>
#include <string>
#include <vector>

#include "ProcDef.h"

using namespace std;

class StringTool
{
public:
	StringTool();
	~StringTool();

	static vector<string> Splite(string str, string strSpl="|");

	//�������ɴ��ָ������ַ���
	static string CombVecToStr(vector<OperPermission> vecOper, string strCom="|");
	static string CombVecToStr(vector<int> vec, string strCom="|");
	static string CombVecToStr(vector<string> vec, string strCom="|");
	static string CombArrayToStr(unsigned char *upArr, unsigned iLen, string strCom="|");

	static string ToLowercase(string strSrc);
	static string ToUppecase(string strSrc);

	static string NumberToStr(int n);

	static string CombToSqlUpdateSetStr(string strField, string strValue, string strSpl="|");

	static bool StrSpliteToIntArray(unsigned char *arr, unsigned int iLen, string str, string strSpl="|");
};

#endif