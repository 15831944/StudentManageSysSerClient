#ifndef __SELECTUSERINFOSYSPROC_H__
#define __SELECTUSERINFOSYSPROC_H__

#include <vector>
#include "BaseProc.h"

class SelectUserInfoSysProc : public BaseProc
{
public:
	SelectUserInfoSysProc(ProcDef nProcDef);
	~SelectUserInfoSysProc();

	virtual bool initMapChoose();

	virtual void EndProc();

	virtual void StartRecv(void* vpData, unsigned int DataLen, /*int iMianId,*/ int iAssistId);
	virtual void EndRecv();

	virtual void SwitchToOper(OperPermission CurOper);

	//���幦��ѡ���������
	void SelectSingleUserInfoChooseHandle(); //������ѯ�û���Ϣ����
	void SelectBatchUserInfoChooseHandle(); //������ѯ�û���Ϣ����

	//���ؽ������
	bool SelectSingleUserInfoRecvHandle(void* vpData, unsigned int DataLen); //������ѯ�û���Ϣ����
	bool SelectBatchUserInfoRecvHandle(void* vpData, unsigned int DataLen); //������ѯ�û���Ϣ����

protected:
	void ShowSelectSingleScoreResult(vector<string> vSelectFieldShow, vector<string> vSelectResultShow, bool bShow=true);
};

#endif