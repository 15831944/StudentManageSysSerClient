#ifndef __ADDSCORESYSPROC_H__
#define __ADDSCORESYSPROC_H__

#include <vector>
#include "BaseProc.h"

class AddScoreSysProc : public BaseProc
{
public:
	AddScoreSysProc(ProcDef nProcDef);
	~AddScoreSysProc();

	virtual bool initMapChoose();

	virtual void EndProc();

	virtual void StartRecv(void* vpData, unsigned int DataLen, /*int iMianId,*/ int iAssistId);
	virtual void EndRecv();

	virtual void SwitchToOper(OperPermission CurOper);


	//��ҳѡ����� ����ǰ������
	void GetSubjectsAfterAddSingleScoreChooseHandle(); //�������ӳɼ����������ƺ�ȫ�ƣ�����ǰ ���п�Ŀ����

	//���幦��ѡ���������
	void AddSingleScoreByOneSubjectChooseHandle(char* pStrExistSubjects); //���Ƶ������ӳɼ�
	void AddSingleScoreBySubjectsChooseHandle(char* pStrExistSubjects); //�������п�Ŀ�������ӳɼ�
	void AddBatchScoreChooseHandle(); //������ӳɼ����������ƺ��������п�Ŀ����˵��ƣ�

	//���ؽ������
	bool GetSubjectsAfterAddSingleScoreRecvHandle(void* vpData, unsigned int DataLen); //�������ӳɼ����������ƺ�ȫ�ƣ�����ǰ ���п�Ŀ�ظ�
	bool AddSingleScoreRecvHandle(void* vpData, unsigned int DataLen); //�������ӳɼ����������ƺ�ȫ�ƣ������ظ�
	bool AddBatchScoreRecvHandle(void* vpData, unsigned int DataLen);//������ӳɼ����������ƺ��������п�Ŀ����˵��ƣ� �����ظ�
	
protected:
	int ShowSubjects(map<int, string> mIStr, int iField=2);
	bool CheckScore(string sScore);

	bool SendAddBatchScoreData(short sType);

private:
	vector<vector<string>> m_vvAddBatchScoreFileData;//������ӳɼ����� �������̫�࣬ÿ���һ�η���˷��سɹ��ſ��Լ��������һ��
	vector<string> m_vecAddBatchScoreFeildData; //������ӳɼ��ֶ�
	unsigned m_uAddBatchScoreSumCount; //������ӵĶ�̬��¼��
};

#endif