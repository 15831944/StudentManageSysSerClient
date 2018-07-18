#include "DeleteUserInfoSysProc.h"

DeleteUserInfoSysProc::DeleteUserInfoSysProc(ProcDef nProcDef) : BaseProc(nProcDef)
{
	initMapChoose();
}

DeleteUserInfoSysProc::~DeleteUserInfoSysProc()
{

}

bool DeleteUserInfoSysProc::initMapChoose()
{
	m_mChoose.insert(pair<int, ChooseData>(GetMaxRealChoose(), ChooseData("����ɾ���û���Ϣ", OPER_PER_DELETEBATCHUSERINFO, PROC_DEF_DELETEUSERINFOSYSPROC)));
	m_mChoose.insert(pair<int, ChooseData>(++GetMaxRealChoose(), ChooseData("����ɾ���û���Ϣ", OPER_PER_DELETESINGLEUSERINFO, PROC_DEF_DELETEUSERINFOSYSPROC)));
	m_mChoose.insert(pair<int, ChooseData>(++GetMaxRealChoose(), ChooseData("����", OPER_PER_INVALID, PROC_DEF_USERINFOSYSPROC)));

	return true;
}

void DeleteUserInfoSysProc::EndProc()
{
	__super::EndProc();
}

void DeleteUserInfoSysProc::StartRecv(void* vpData, unsigned int DataLen, /*int iMianId,*/ int iAssistId)
{

}

void DeleteUserInfoSysProc::EndRecv()
{
	__super::EndRecv();
}

void DeleteUserInfoSysProc::SwitchToOper(OperPermission CurOper)
{
	__super::SwitchToOper(CurOper);
}


