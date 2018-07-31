#ifndef __AUTHORITYSYSPROC_H__
#define __AUTHORITYSYSPROC_H__

#include "BaseProc.h"

class AuthoritySysProc : public BaseProc
{
public:
	AuthoritySysProc(ProcDef nProcDef);
	~AuthoritySysProc();

	virtual bool initMapChoose();

	virtual void EndProc();

	virtual void StartRecv(void* vpData, unsigned int DataLen, /*int iMianId,*/ int iAssistId);
	virtual void EndRecv();

	virtual void SwitchToOper(OperPermission CurOper);

	//��ҳѡ����� ����ǰ������
	void GetAuthoritbyAfterEditAuthoritbyChooseHandle(); //��ȡ���л��߿�����Ȩ�� ����

	//���幦��ѡ���������
	void EditAuthoritChooseHandle(unsigned char sType, char* cAccount, unsigned char* cAuthority, unsigned char cAuthorityCount); //��ɾȨ������

	//���ؽ������
	bool GetAuthoritbyAfterEditAuthoritbyRecvHandle(void* vpData, unsigned int DataLen); //��ȡ���л��߿�����Ȩ�� �ظ�
	bool EditAuthoritRecvHandle(void* vpData, unsigned int DataLen); //��ɾȨ�޻ظ�

private:
	void ShowAuthoritby(unsigned char* cAuthority, unsigned char cAuthorityCount, int iField=2);
	bool FindOneInArray(unsigned char* pArray, unsigned char iCount, unsigned char iMaxCount, unsigned char Obj);
	bool CheckStringVaildRemoveSpl(string str, string strSpl, string strVaild);
};

#endif