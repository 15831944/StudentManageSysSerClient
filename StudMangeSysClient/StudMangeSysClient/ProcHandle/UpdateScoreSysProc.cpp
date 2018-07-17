#include "UpdateScoreSysProc.h"

UpdateScoreSysProc::UpdateScoreSysProc(ProcDef nProcDef) : BaseProc(nProcDef)
{
	initMapChoose();
}

UpdateScoreSysProc::~UpdateScoreSysProc()
{

}

bool UpdateScoreSysProc::initMapChoose()
{
	//m_mChoose.insert(pair<int, ChooseData>(GetMaxRealChoose(), ChooseData("�����������ĳɼ�", OPER_PER_UPDATEBATCHSCOREBYONESUBJECT, PROC_DEF_UPDATESCORESYSPROC)));
	//m_mChoose.insert(pair<int, ChooseData>(++GetMaxRealChoose(), ChooseData("�������п�Ŀ�������ĳɼ�", OPER_PER_UPDATEBATCHSCOREBYSUBJECTS, PROC_DEF_UPDATESCORESYSPROC)));
	m_mChoose.insert(pair<int, ChooseData>(GetMaxRealChoose(), ChooseData("���Ƶ������ĳɼ�", OPER_PER_UPDATESINGLESCOREBYONESUBJECT, PROC_DEF_UPDATESCORESYSPROC)));
	m_mChoose.insert(pair<int, ChooseData>(++GetMaxRealChoose(), ChooseData("�������п�Ŀ�������ĳɼ�", OPER_PER_UPDATESINGLESCOREBYSUBJECTS, PROC_DEF_UPDATESCORESYSPROC)));
	m_mChoose.insert(pair<int, ChooseData>(++GetMaxRealChoose(), ChooseData("����", OPER_PER_INVALID, PROC_DEF_SCORESYSPROC)));

	return true;
}

void UpdateScoreSysProc::EndProc()
{
	__super::EndProc();
}

void UpdateScoreSysProc::StartRecv(void* vpData, unsigned int DataLen, /*int iMianId,*/ int iAssistId)
{

}

void UpdateScoreSysProc::EndRecv()
{
	__super::EndRecv();
}

void UpdateScoreSysProc::SwitchToOper(OperPermission CurOper)
{
	__super::SwitchToOper(CurOper);
}
