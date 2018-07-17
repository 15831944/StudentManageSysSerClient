#include "DeleteScoreSysProc.h"

DeleteScoreSysProc::DeleteScoreSysProc(ProcDef nProcDef) : BaseProc(nProcDef)
{
	initMapChoose();
}

DeleteScoreSysProc::~DeleteScoreSysProc()
{

}

bool DeleteScoreSysProc::initMapChoose()
{
	m_mChoose.insert(pair<int, ChooseData>(GetMaxRealChoose(), ChooseData("����ɾ���ɼ�", OPER_PER_DELETEBATCHSCORE, PROC_DEF_DELETESCORESYSPROC)));
	m_mChoose.insert(pair<int, ChooseData>(++GetMaxRealChoose(), ChooseData("����ɾ���ɼ�", OPER_PER_DELETESINGLESCORE, PROC_DEF_DELETESCORESYSPROC)));
	m_mChoose.insert(pair<int, ChooseData>(++GetMaxRealChoose(), ChooseData("����", OPER_PER_INVALID, PROC_DEF_SCORESYSPROC)));

	return true;
}

void DeleteScoreSysProc::EndProc()
{
	__super::EndProc();
}

void DeleteScoreSysProc::StartRecv(void* vpData, unsigned int DataLen, /*int iMianId,*/ int iAssistId)
{

}

void DeleteScoreSysProc::EndRecv()
{
	__super::EndRecv();
}

void DeleteScoreSysProc::SwitchToOper(OperPermission CurOper)
{
	__super::SwitchToOper(CurOper);
}
