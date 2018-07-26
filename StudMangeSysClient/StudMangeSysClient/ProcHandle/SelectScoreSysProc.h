#ifndef __SELECTSCORESYSPROC_H__
#define __SELECTSCORESYSPROC_H__

#include <vector>
#include "BaseProc.h"

using namespace std;

class SelectScoreSysProc : public BaseProc
{
public:
	SelectScoreSysProc(ProcDef nProcDef);
	~SelectScoreSysProc();

	virtual bool initMapChoose();

	virtual void EndProc();

	virtual void StartRecv(void* vpData, unsigned int DataLen, /*int iMianId,*/ int iAssistId);
	virtual void EndRecv();

	virtual void SwitchToOper(OperPermission CurOper);


	//��ҳѡ����� ����ǰ������
	void GetSubjectsAfterSelectScoreChooseHandle(); //��ѯ�ɼ������������͵��������ƺ�ȫ�ƣ�����ǰ ���п�Ŀ����

	//���幦��ѡ���������
	void SelectSingleScoreByOneSubjectChooseHandle(char* pStrExistSubjects); //���Ƶ�����ѯ�ɼ�
	void SelectSingleScoreBySubjectsChooseHandle(char* pStrExistSubjects); //�������п�Ŀ������ѯ�ɼ�
	void SelectBatchScoreByOneSubjectChooseHandle(char* pStrExistSubjects); //����������ѯ�ɼ�
	void SelectBatchScoreBySubjectsChooseHandle(char* pStrExistSubjects); //�������п�Ŀ������ѯ�ɼ�

	//���ؽ������
	bool GetSubjectsAfterSelectScoreRecvHandle(void* vpData, unsigned int DataLen); //��ѯ�ɼ������������͵��������ƺ�ȫ�ƣ�����ǰ ���п�Ŀ�ظ�
	bool SelectSingleScoreRecvHandle(void* vpData, unsigned int DataLen); //������ѯ�ɼ�����
	bool SelectBatchScoreRecvHandle(void* vpData, unsigned int DataLen); //������ѯ�ɼ�����

private:
	int ShowSubjects(map<int, string> mIStr, int iField=2);
	void ShowSelectSingleScoreResult(vector<string> vSelectFieldShow, vector<string> vSelectResultShow, bool bShowDescribe=false, bool bShowLine=true);
};

#endif