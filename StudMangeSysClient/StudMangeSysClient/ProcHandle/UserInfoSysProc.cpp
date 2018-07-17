#include "UserInfoSysProc.h"

UserInfoSysProc::UserInfoSysProc(ProcDef nProcDef) : BaseProc(nProcDef)
{
	initMapChoose();
}

UserInfoSysProc::~UserInfoSysProc()
{

}

bool UserInfoSysProc::initMapChoose()
{
	m_mChoose.insert(pair<int, ChooseData>(GetMaxRealChoose(), ChooseData("�����û���Ϣ", OPER_PER_INVALID, PROC_DEF_ADDUSERINFOSYSPROC)));
	m_mChoose.insert(pair<int, ChooseData>(++GetMaxRealChoose(), ChooseData("�����û���Ϣ", OPER_PER_INVALID, PROC_DEF_UPDATEUSERINFOSYSPROC)));
	m_mChoose.insert(pair<int, ChooseData>(++GetMaxRealChoose(), ChooseData("��ѯ�û���Ϣ", OPER_PER_INVALID, PROC_DEF_SELECTUSERINFOSYSPROC)));
	m_mChoose.insert(pair<int, ChooseData>(++GetMaxRealChoose(), ChooseData("ɾ���û���Ϣ", OPER_PER_INVALID, PROC_DEF_DELETEUSERINFOSYSPROC)));
	m_mChoose.insert(pair<int, ChooseData>(++GetMaxRealChoose(), ChooseData("����", OPER_PER_INVALID, PROC_DEF_COMMONSYSPROC)));

	return true;
}

void UserInfoSysProc::EndProc()
{
	__super::EndProc();
}

void UserInfoSysProc::StartRecv(void* vpData, unsigned int DataLen, /*int iMianId,*/ int iAssistId)
{

}

void UserInfoSysProc::EndRecv()
{
	__super::EndRecv();
}

void UserInfoSysProc::SwitchToOper(OperPermission CurOper)
{
	__super::SwitchToOper(CurOper);
}
