#ifndef __DELETEUSERINFOSYSPROC_H__
#define __DELETEUSERINFOSYSPROC_H__

#include "BaseProc.h"

class DeleteUserInfoSysProc : public BaseProc
{
public:
	DeleteUserInfoSysProc(ProcDef nProcDef);
	~DeleteUserInfoSysProc();

	virtual bool initMapChoose();

	virtual void EndProc();

	virtual void StartRecv(void* vpData, unsigned int DataLen, /*int iMianId,*/ int iAssistId);
	virtual void EndRecv();

	virtual void SwitchToOper(OperPermission CurOper);

	//ѡ���������
	void DeleteUserInfoChooseHandle();

	//���ؽ������
	bool DeleteUserInfoRecvHandle(void* vpData, unsigned int DataLen);
};

#endif