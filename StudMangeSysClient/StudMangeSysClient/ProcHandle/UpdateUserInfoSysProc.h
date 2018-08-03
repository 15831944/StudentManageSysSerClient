#ifndef __UPDATEUSERINFOSYSPROC_H__
#define __UPDATEUSERINFOSYSPROC_H__

#include <map>
#include <string>
#include "BaseProc.h"

using namespace std;

class UpdateUserInfoSysProc : public BaseProc
{
public:
	UpdateUserInfoSysProc(ProcDef nProcDef);
	~UpdateUserInfoSysProc();

	virtual bool initMapChoose();

	virtual void EndProc();

	virtual void StartRecv(void* vpData, unsigned int DataLen, /*int iMianId,*/ int iAssistId);
	virtual void EndRecv();

	virtual void SwitchToOper(OperPermission CurOper);

	//��ҳѡ����� ����ǰ������
	void GetUserInfoAfterUpdateSingleUserInfoChooseHandle(); //���������û���Ϣǰ�����û���Ϣ

	//���幦��ѡ���������
	void UpdateSingleUserInfoByOneFieldChooseHandle(unsigned uUserID, char* pAccount, char* pPwd, char* pName, unsigned char uSex, unsigned char uIdent, char* pMajor, char* pGrade); //�������ֶθ����û���Ϣ����
	void UpdateSingleUserInfoByFixedFieldChooseHandle(unsigned uUserID, char* pAccount, char* pPwd, char* pName, unsigned char uSex, unsigned char uIdent, char* pMajor, char* pGrade); //�����̶��ֶθ����û���Ϣ����

	//���ؽ������
	bool GetUserInfoAfterUpdateSingleUserInfoRecvHandle(void* vpData, unsigned int DataLen); //���������û���Ϣ�����û���Ϣ
	bool UpdateSingleUserInfoRecvHandle(void* vpData, unsigned int DataLen); //���������û���Ϣ����

protected:
	void InitUserInfoFieldName();
	string GetEnglishUserInfoFieldByField(int iFeildId);
	string GetChineseUserInfoFieldByField(int iFeildId);
	bool IsPwdUserInfoField(int iField);

	int GetChooseUserInfoFieldId(string strShowFieldID, int iField=2); //չʾ�ֶβ�ѡ�� 1-account 2-password 3-name 4-sex 5-Ident 6-major 7-grade
	bool InputAllUserInfoFieldValues(string strShowFieldID, map<int, string>& mapNewAllFieldValue, int iField=2); //���ڹ̶��ֶθ��ġ�Ҫ����ݴ����fieldid�����µ�ֵ�����Ϸ��ͷŵ�map����ȥ
		
	bool CheckUserInfoFieldValue(string strFeildValue, int iFieldId); //����fieldid���������ַ����Ƿ�Ϸ�
	string GetOldUserInfoFieldValue(char* pAccount, char* pPwd, char* pName, unsigned char uSex, unsigned char uIdent, char* pMajor, char* pGrade, int iFieldId); //����fieldid��ȡ���û���Ϣֵ
private:
	map<int, string> m_mUserInfoFieldName; 
	int m_maxUserInfoFieldID;

	//�����û���Ϣʱ�����ʱ����
	unsigned char m_sUserInfoField[MAXUSERINFOFIELDCOUNT]; //�û���Ϣ�ֶ�id
	char m_cUserInfoValue[MAXUSERINFOFIELDCOUNT][41];
	unsigned char m_bUserInfoFieldCount; //�û���Ϣ�ֶ���
};

#endif