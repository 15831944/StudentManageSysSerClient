#ifndef __STRINGTOOL_H__
#define __STRINGTOOL_H__

#include <iostream>
#include <string>
#include <vector>

#include "PublicDef.h"

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

	//ͨ���ָ������������ַ����ָ����ϳ�mysql update ��set��䲿��
	static string CombToSqlUpdateSetStr(string strField, string strValue, string strSpl="|");
	//������vector��ϳ�mysql update ��set��䲿��
	static string CombToSqlUpdateSetStr(vector<string> vecStrField, vector<string> vecStrValue);

	static bool StrSpliteToUcArray(unsigned char *arr, unsigned int iLen, string str, string strSpl="|");

	/*
	* ���ܣ������ַ�����������ñ���ַ���䣨�ֽ��㣩
	* ����������str�������ַ�����iWidth��ȣ�fillChar����ַ�������ȸպû��߹���������䣻dirFlag��䷽ʽ��0����䣬1�����
	*/
	static string SetStringFieldWidth(string str, unsigned iWidth, char fillChar=' ', unsigned char dirFlag=0); 
};

#endif