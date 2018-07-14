#include "CommonSysProc.h"
#include "ProcMgr.h"
#include "CheckTool.h"
#include "TCPHandle.h"

CommonSysProc::CommonSysProc(ProcDef nProcDef) : BaseProc(nProcDef)
{
	initMapChoose();
}

CommonSysProc::~CommonSysProc()
{

}

bool CommonSysProc::initMapChoose()
{
	m_mChoose.insert(pair<int, ChooseData>(GetMaxRealChoose(), ChooseData("�ɼ�", OPER_PER_INVALID, PROC_DEF_SCORESYSPROC)));
	m_mChoose.insert(pair<int, ChooseData>(++GetMaxRealChoose(), ChooseData("�û���Ϣ", OPER_PER_INVALID, PROC_DEF_USERINFOSYSPROC)));
	m_mChoose.insert(pair<int, ChooseData>(++GetMaxRealChoose(), ChooseData("����", OPER_PER_INVALID, PROC_DEF_ENTERSYSPROC)));

	return true;
}

void CommonSysProc::EndProc()
{
	__super::EndProc();
}

void CommonSysProc::StartRecv(void* vpData, unsigned int DataLen, /*int iMianId,*/ int iAssistId)
{

}

void CommonSysProc::EndRecv()
{
	__super::EndRecv();
}

void CommonSysProc::SwitchToOper(OperPermission CurOper)
{
	__super::SwitchToOper(CurOper);


}