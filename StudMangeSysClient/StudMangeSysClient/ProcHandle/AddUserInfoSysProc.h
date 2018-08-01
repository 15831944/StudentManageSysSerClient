#ifndef __ADDUSERINFOSYSPROC_H__
#define __ADDUSERINFOSYSPROC_H__

#include <vector>
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
	void AddBatchUserInfoChooseHandle(); //���������û���Ϣ����


	//���ؽ������
	bool AddSingleUserInfoRecvHandle(void* vpData, unsigned int DataLen);//���������û���Ϣ�ظ�
	bool AddBatchUserInfoRecvHandle(void* vpData, unsigned int DataLen);//���������û���Ϣ�ظ�

protected:
	bool SendAddBatchUserInfoData();

private:
	vector<vector<string>> m_vvAddBatchUserInfoFileData;//��������û���Ϣ���� �������̫�࣬ÿ���һ�η���˷��سɹ��ſ��Լ��������һ��
	vector<string> m_vecAddBatchUserInfoFeildData; //��������û���Ϣ�ֶ�
	unsigned m_uAddBatchUserInfoSumCount; //������ӵĶ�̬��¼��
	unsigned m_uAddBatchUserInfoSucceedCount; //������ӳɹ��Ķ�̬��¼��
};


#endif