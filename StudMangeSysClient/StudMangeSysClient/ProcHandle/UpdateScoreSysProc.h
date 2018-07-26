#ifndef __UPDATESCORESYSPROC_H__
#define __UPDATESCORESYSPROC_H__

#include <vector>
#include <string>
#include "BaseProc.h"

using namespace std;

class UpdateScoreSysProc : public BaseProc
{
public:
	UpdateScoreSysProc(ProcDef nProcDef);
	~UpdateScoreSysProc();

	virtual bool initMapChoose();

	virtual void EndProc();

	virtual void StartRecv(void* vpData, unsigned int DataLen, /*int iMianId,*/ int iAssistId);
	virtual void EndRecv();

	virtual void SwitchToOper(OperPermission CurOper);

	//��ҳѡ����� ����ǰ������
	void GetSubjectsAfterUpdateSingleScoreChooseHandle(); //�������ĳɼ����������ƺ�ȫ�ƣ�����ǰ ���п�Ŀ����

	//��ҳ���������Ķ������������󱻲����û��������з��������չʾ��������
	void GetSubjectsScoreAfterUpdateSingleScoreChooseHandle(char* pStrExistSubjects); //�������ĳɼ����������ƺ�ȫ�ƣ���������ǰ ���п�Ŀ�ɼ�����

	//���幦��ѡ���������
	void UpdateSingleScoreByOneSubjectChooseHandle(char* pName, char* pAccount, char* pGrade, unsigned char* pSubjectsId, unsigned char* pScore, unsigned char bSubjectCount); //���Ƶ������ĳɼ�
	void UpdateSingleScoreBySubjectsChooseHandle(char* pName, char* pAccount, char* pGrade, unsigned char* pSubjectsId, unsigned char* pScore, unsigned char bSubjectCount); //�������п�Ŀ�������ĳɼ�

	//���ؽ������
	bool GetSubjectsAfterUpdateSingleScoreRecvHandle(void* vpData, unsigned int DataLen); //�������ĳɼ����������ƺ�ȫ�ƣ�����ǰ ���п�Ŀ�ظ�
	bool GetSubjectsScoreAfterUpdateSingleScoreRecvHandle(void* vpData, unsigned int DataLen);//�������ĳɼ����������ƺ�ȫ�ƣ���������ǰ ���п�Ŀ�ɼ��ظ�
	bool UpdateSingleScoreRecvHandle(void* vpData, unsigned int DataLen); //�������ĳɼ����������ƺ�ȫ�ƣ������ظ�

private:
	bool ShowSubjects(unsigned char* pSubjectsId, unsigned char* pScore, unsigned char bSubjectCount, int iField=2);
	bool FindSubjectsId(unsigned char* pSubjectsId, unsigned char bSubjectCount, unsigned char SubjectsId);
	bool CheckScore(string sScore);

private:
	vector<string> m_vecStrChineseName;
};

#endif