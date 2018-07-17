#include "ScoreSysProc.h"

ScoreSysProc::ScoreSysProc(ProcDef nProcDef) : BaseProc(nProcDef)
{
	initMapChoose();
}

ScoreSysProc::~ScoreSysProc()
{

}

bool ScoreSysProc::initMapChoose()
{
	m_mChoose.insert(pair<int, ChooseData>(GetMaxRealChoose(), ChooseData("���ӳɼ�", OPER_PER_INVALID, PROC_DEF_ADDSCORESYSPROC)));
	m_mChoose.insert(pair<int, ChooseData>(++GetMaxRealChoose(), ChooseData("���ĳɼ�", OPER_PER_INVALID, PROC_DEF_UPDATESCORESYSPROC)));
	m_mChoose.insert(pair<int, ChooseData>(++GetMaxRealChoose(), ChooseData("��ѯ�ɼ�", OPER_PER_INVALID, PROC_DEF_SELECTSCORESYSPROC)));
	m_mChoose.insert(pair<int, ChooseData>(++GetMaxRealChoose(), ChooseData("ɾ���ɼ�", OPER_PER_INVALID, PROC_DEF_DELETESCORESYSPROC)));
	m_mChoose.insert(pair<int, ChooseData>(++GetMaxRealChoose(), ChooseData("������Ŀ", OPER_PER_INVALID, PROC_DEF_ALTERSUBJECTSSYSPROC)));
	m_mChoose.insert(pair<int, ChooseData>(++GetMaxRealChoose(), ChooseData("����", OPER_PER_INVALID, PROC_DEF_COMMONSYSPROC)));

	return true;
}

void ScoreSysProc::EndProc()
{
	__super::EndProc();
}

void ScoreSysProc::StartRecv(void* vpData, unsigned int DataLen, /*int iMianId,*/ int iAssistId)
{

}
void ScoreSysProc::EndRecv()
{
	__super::EndRecv();
}

void ScoreSysProc::SwitchToOper(OperPermission CurOper)
{
	__super::SwitchToOper(CurOper);
}