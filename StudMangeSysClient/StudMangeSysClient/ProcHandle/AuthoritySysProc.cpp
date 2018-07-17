#include "AuthoritySysProc.h"

AuthoritySysProc::AuthoritySysProc(ProcDef nProcDef) : BaseProc(nProcDef)
{
	initMapChoose();
}

AuthoritySysProc::~AuthoritySysProc()
{

}

bool AuthoritySysProc::initMapChoose()
{
	m_mChoose.insert(pair<int, ChooseData>(GetMaxRealChoose(), ChooseData("�����û�Ȩ��", OPER_PER_ADDAUTHORITBYONE, PROC_DEF_AUTHORITYSYSPROC)));
	m_mChoose.insert(pair<int, ChooseData>(++GetMaxRealChoose(), ChooseData("ɾ���û�Ȩ��", OPER_PER_DELETEAUTHORITBYONE, PROC_DEF_AUTHORITYSYSPROC)));
	m_mChoose.insert(pair<int, ChooseData>(++GetMaxRealChoose(), ChooseData("����", OPER_PER_INVALID, PROC_DEF_COMMONSYSPROC)));

	return true;
}

void AuthoritySysProc::EndProc()
{
	__super::EndProc();
}

void AuthoritySysProc::StartRecv(void* vpData, unsigned int DataLen, /*int iMianId,*/ int iAssistId)
{

}

void AuthoritySysProc::EndRecv()
{
	__super::EndRecv();
}

void AuthoritySysProc::SwitchToOper(OperPermission CurOper)
{
	__super::SwitchToOper(CurOper);
}
