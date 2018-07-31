#ifndef __USERINFOMGR_H__
#define __USERINFOMGR_H__

#include <map>
#include <vector>
#include "xSingleton.h"
#include "ProtoDef.h"
#include "ProcDef.h"

using namespace std;

class UserInfoMgr : public xSingleton<UserInfoMgr>
{
public:
	UserInfoMgr();
	~UserInfoMgr();

	void SetSomeInfo(string name, string sAccount, unsigned int userId, short sIdent, short sSex);

	void SetUserName(string name);
	string GetUserName();

	void SetUserAccount(string sAccount);
	string GetUserAccount();

	void SetUserUserId(unsigned int userId);
	unsigned int GetUserUserId();

	void SetUserIdent(short sIdent);
	short GetUserIdent();

	void SetUserSex(short sSex);
	short GetUserSex();

	void SetUserMark(short sMark);
	short GetUserMark();

	//�û������Լ�Ȩ�޴���
	bool CheckOperPerValid(OperPermission OperPer);
	bool InitVOperPer();
	bool SetVOperPer(string str);
	//����Ȩ�޲���
	bool CanFindInAllOperPer(OperPermission OperPer);
	string GetDescByOperPer(OperPermission OperPer); //��ȡȨ�޵���������

	string GetChineseNameByType(SubjectsType sType);
	SubjectsType GetTypeByEnglishName(string strName);
	bool CanFindSubjectsType(SubjectsType sType);
protected:
	void initAllSubjects();
	void initAllOperPer();

private:
	string sName;
	string sAccount;
	unsigned int iUserId;
	short sIdent;
	short sSex;

	//�û���̬��¼
	short sMark;

	//�û��Լ�Ȩ��
	vector<OperPermission> m_vOperPer;
	//����Ȩ�޼���
	map<OperPermission, string> m_mAllOperPer;

	//���п�Ŀ
	map<SubjectsType, SubjectsData> m_mAllSubjects;
};

#endif