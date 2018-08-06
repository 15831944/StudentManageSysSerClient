#ifndef __USERINFOMGR_H__
#define __USERINFOMGR_H__

#include <map>
#include <vector>
#include "xSingleton.h"
#include "PublicDef.h"
#include "NetDef.h"

using namespace std;

class UserInfoMgr : public xSingleton<UserInfoMgr>
{
public: 
	UserInfoMgr();
	~UserInfoMgr();

	//�û��Խӿڣ�����ͬ��
public:
	bool InsertInfo(unsigned __int64 socketId, UserInfo userInfo);
	bool InsertInfo(unsigned __int64 socketId, string strIP, unsigned short sPort, PER_HANDLE_DATA* PerHandleData);

	bool RemoveInfoBySocketId(unsigned __int64 socketId);
	bool RemoveInfoByUserId(unsigned int iUserId);

	bool InitNotNetDataBySocketId(unsigned __int64 socketId); //��ʼ������������

	bool SetUserIdBySocketId(unsigned __int64 socketId, unsigned int iUserId);
	unsigned int GetUserIdBySocketId(unsigned __int64 socketId);

	unsigned __int64 GetSocketIdByAccount(string strAccount);

	bool SetAuthorityBySocketId(unsigned __int64 socketId, string strAuthority);
	string GetAuthorityBySocketId(unsigned __int64 socketId);

	bool SetAuthorityByAccount(string strAccount, string strAuthority);
	string GetAuthorityByAccount(string strAccount);

	bool SetNewAccountByAccount(string strAccount, string strNewAccount);

	bool FindAccount(string strAccount); //���Ҹ��˺�

	bool IsHaveOneAuthorityBySocketId(unsigned __int64 socketId, OperPermission OperPer);

	bool SetAccountBySocketId(unsigned __int64 socketId, string strAccount);
	string GetAccountBySocketId(unsigned __int64 socketId);

	bool SetIdentBySocketId(unsigned __int64 socketId, short sIdent);
	short GetIdentBySocketId(unsigned __int64 socketId);

	bool SetRegNeedCountBySocketId(unsigned __int64 socketId, short sRegNeedCount);
	short GetRegNeedCountBySocketId(unsigned __int64 socketId);

	bool SetStrTmpDataBySocketId(unsigned __int64 socketId, string strTmpData);
	string GetStrTmpDataBySocketId(unsigned __int64 socketId);

	bool SetPerHandleDataBySocketId(unsigned __int64 socketId, PER_HANDLE_DATA* PerHandleData);
	PER_HANDLE_DATA* GetPerHandleDataBySocketId(unsigned __int64 socketId);

	bool SetPerIoDataBySocketId(unsigned __int64 socketId, LPPER_IO_OPERATION_DATA PerIoData);
	LPPER_IO_OPERATION_DATA GetPerIoDataBySocketId(unsigned __int64 socketId);

	//�����Խӿڣ�������ͬ��
public:
	void GetDefaultAuthorityByIdent(IdentType Ident, vector<OperPermission>& vecOper); //������ݱ�ʶ��ȡĬ��Ȩ��
	void GetAuthorityByStrAuthority(string strAuthority, vector<OperPermission>& vecOper, unsigned char bType, string strSpl="|"); //ͨ��Ȩ���ַ���������bType��ȡ��������Ȩ��vector��bType 1���е�  2�����ӵ�

	string GetEnglishUserInfoFieldByField(int iFeildId);
	string GetChineseUserInfoFieldByField(int iFeildId);
	bool GetCanUpdateField(short iMyIdent, string strMyAccount, short iObjIdent, string strObjAccount, bool* pCanUpdate, int iCanUpdateMaxCount); //����Ȩ�ޣ���ȡ�����û����Ը��Ķ����û����ֶ�

	//�Ƕ�̬����
protected:
	void initAllOperPer();
	void InitUserInfoFieldName(); //1-account 2-password 3-name 4-sex 5-Ident 6-major 7-grade

	//����Ȩ�޼���
	map<OperPermission, string> m_mAllOperPer;

	//�û���Ϣ�ֶ�
	map<int, string> m_mUserInfoFieldName;
	int m_maxUserInfoFieldID;

	//��̬���ݣ�����ͬ��
private:
	map<unsigned __int64, UserInfo> m_mUserInfo;
};

#endif