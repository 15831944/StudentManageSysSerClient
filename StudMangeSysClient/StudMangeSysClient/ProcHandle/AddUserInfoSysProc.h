#ifndef __ADDUSERINFOSYSPROC_H__
#define __ADDUSERINFOSYSPROC_H__

#include "BaseProc.h"

class AddUserInfoSysProc : public BaseProc
{
public:
	AddUserInfoSysProc(ProcDef nProcDef);
	~AddUserInfoSysProc();

	virtual bool initMapChoose();

	virtual void EndProc();

	virtual void StartRecv(void* vpData, unsigned int DataLen, /*int iMianId,*/ int iAssistId);
	virtual void EndRecv();

	virtual void SwitchToOper(OperPermission CurOper);

	//���幦��ѡ���������
	void AddSingleUserInfoChooseHandle(); //���������û���Ϣ����

	//���ؽ������
	bool AddSingleUserInfoRecvHandle(void* vpData, unsigned int DataLen);//���������û���Ϣ�ظ�
};

#endif