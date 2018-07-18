#include "UpdateUserInfoSysProc.h"

UpdateUserInfoSysProc::UpdateUserInfoSysProc(ProcDef nProcDef) : BaseProc(nProcDef)
{
	initMapChoose();
}

UpdateUserInfoSysProc::~UpdateUserInfoSysProc()
{

}


bool UpdateUserInfoSysProc::initMapChoose()
{
	//m_mChoose.insert(pair<int, ChooseData>(GetMaxRealChoose(), ChooseData("�����ֶ����������û���Ϣ", OPER_PER_UPDATEBATCHUSERINFOBYONE, PROC_DEF_UPDATEUSERINFOSYSPROC)));
	//m_mChoose.insert(pair<int, ChooseData>(++GetMaxRealChoose(), ChooseData("���̶ֹ��ֶ����������û���Ϣ", OPER_PER_UPDATEBATCHUSERINFOBYMORE, PROC_DEF_UPDATEUSERINFOSYSPROC)));
	m_mChoose.insert(pair<int, ChooseData>(GetMaxRealChoose(), ChooseData("�����ֶε��������û���Ϣ", OPER_PER_UPDATESINGLEUSERINFOEBYONE, PROC_DEF_UPDATEUSERINFOSYSPROC)));
	m_mChoose.insert(pair<int, ChooseData>(++GetMaxRealChoose(), ChooseData("���̶ֹ��ֶε��������û���Ϣ", OPER_PER_UPDATESINGLEUSERINFOBYMORE, PROC_DEF_UPDATEUSERINFOSYSPROC)));
	m_mChoose.insert(pair<int, ChooseData>(++GetMaxRealChoose(), ChooseData("����", OPER_PER_INVALID, PROC_DEF_USERINFOSYSPROC)));

	return true;
}

void UpdateUserInfoSysProc::EndProc()
{
	__super::EndProc();
}

void UpdateUserInfoSysProc::StartRecv(void* vpData, unsigned int DataLen, /*int iMianId,*/ int iAssistId)
{

}

void UpdateUserInfoSysProc::EndRecv()
{
	__super::EndRecv();
}

void UpdateUserInfoSysProc::SwitchToOper(OperPermission CurOper)
{
	__super::SwitchToOper(CurOper);
}

