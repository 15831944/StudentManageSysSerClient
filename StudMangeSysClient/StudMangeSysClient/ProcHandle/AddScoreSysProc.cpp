#include "AddScoreSysProc.h"

AddScoreSysProc::AddScoreSysProc(ProcDef nProcDef) : BaseProc(nProcDef)
{
	initMapChoose();
}

AddScoreSysProc::~AddScoreSysProc()
{

}


bool AddScoreSysProc::initMapChoose()
{
	m_mChoose.insert(pair<int, ChooseData>(GetMaxRealChoose(), ChooseData("�����������ӳɼ�", OPER_PER_ADDBATCHSCOREBYONESUBJECT, PROC_DEF_ADDSCORESYSPROC)));
	m_mChoose.insert(pair<int, ChooseData>(++GetMaxRealChoose(), ChooseData("�������п�Ŀ�������ӳɼ�", OPER_PER_ADDBATCHSCOREBYSUBJECTS, PROC_DEF_ADDSCORESYSPROC)));
	m_mChoose.insert(pair<int, ChooseData>(++GetMaxRealChoose(), ChooseData("���Ƶ������ӳɼ�", OPER_PER_ADDSINGLESCOREBYONESUBJECT, PROC_DEF_ADDSCORESYSPROC)));
	m_mChoose.insert(pair<int, ChooseData>(++GetMaxRealChoose(), ChooseData("�������п�Ŀ�������ӳɼ�", OPER_PER_ADDSINGLESCOREBYSUBJECTS, PROC_DEF_ADDSCORESYSPROC)));
	m_mChoose.insert(pair<int, ChooseData>(++GetMaxRealChoose(), ChooseData("����", OPER_PER_INVALID, PROC_DEF_SCORESYSPROC)));

	return true;
}

void AddScoreSysProc::EndProc()
{
	__super::EndProc();
}

void AddScoreSysProc::StartRecv(void* vpData, unsigned int DataLen, /*int iMianId,*/ int iAssistId)
{

}

void AddScoreSysProc::EndRecv()
{
	__super::EndRecv();
}

void AddScoreSysProc::SwitchToOper(OperPermission CurOper)
{
	__super::SwitchToOper(CurOper);
}
