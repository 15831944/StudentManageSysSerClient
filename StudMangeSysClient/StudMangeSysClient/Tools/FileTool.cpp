#include "FileTool.h"

FileTool::FileTool()
{

}

FileTool::~FileTool()
{

}

bool FileTool::ReadFileToStrVec(vector<string>& vecStr, string strPath)
{
	fstream ReadFile;
	vecStr.clear();
	ReadFile.open(strPath.c_str(), ios::in);
	if (ReadFile.fail())
	{
		cout<<"���ļ�ʧ��"<<endl;
		return false;
	}

	string str;
	while(getline(ReadFile, str))
	{
		vecStr.push_back(str);
	}
	ReadFile.close();

	return true;
}