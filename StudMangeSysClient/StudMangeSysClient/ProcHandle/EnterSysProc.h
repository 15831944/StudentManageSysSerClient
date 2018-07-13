#ifndef __ENTERSYSPROC_H__
#define __ENTERSYSPROC_H__

#include "BaseProc.h"

class EnterSysProc : public BaseProc
{
public:
	EnterSysProc(ProcDef nProcDef);
	~EnterSysProc();

	virtual bool initMapChoose();

	virtual void StartProc();
	virtual void EndProc();

	virtual void StartRecv(void* vpData, unsigned int DataLen, /*int iMianId,*/ int iAssistId);
	virtual void EndRecv();

	virtual void SwitchToOper(OperPermission CurOper);

	//ѡ���������
	void LoginChooseHandle();
	void RegisterChooseHandle();

	//���ؽ������
	bool LoginRecvHandle(void* vpData, unsigned int DataLen);
	bool RegisterRecvHandle(void* vpData, unsigned int DataLen);

};
#endif
