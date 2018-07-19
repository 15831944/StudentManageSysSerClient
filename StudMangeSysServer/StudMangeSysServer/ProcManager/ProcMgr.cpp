#include "ProcMgr.h"

ProcMgr::ProcMgr()
{
}

ProcMgr::~ProcMgr()
{

}

bool ProcMgr::InitMoniter()
{
	/*
	* ������Ϣ��������
	*/
	m_RecvHandleMoniter.MonitorEvent(ASSIST_ID_LOGIN_REQ, &m_EnterSysProc, &EnterSysProc::LoginRecvHandle);
	m_RecvHandleMoniter.MonitorEvent(ASSIST_ID_REGISTER_REQ, &m_EnterSysProc, &EnterSysProc::RegisterRecvHandle);
	m_RecvHandleMoniter.MonitorEvent(ASSIST_ID_EXIT_SYS_REQ, &m_EnterSysProc, &EnterSysProc::ExitSysRecvHandle);
	m_RecvHandleMoniter.MonitorEvent(ASSIST_ID_GET_CAN_ALTER_AFTER_ALTER_SUBJECTS_REQ, &m_ScoreProc, &ScoreProc::CheckCanAlterSubjectRecvHandle);
	m_RecvHandleMoniter.MonitorEvent(ASSIST_ID_ALTER_SUBJECTS_REQ, &m_ScoreProc, &ScoreProc::AlterSubjectRecvHandle);


	/*
	* �ظ���Ϣ��������
	*/
	m_ReplyHandleMoniter.MonitorEvent(ASSIST_ID_SPECIAL_GET_EXIAT_SUBJECTS, SubjectsMgr::GetInstance(), &SubjectsMgr::GetExistSubjectReplyHandle);
	m_ReplyHandleMoniter.MonitorEvent(ASSIST_ID_LOGIN_ACK, &m_EnterSysProc, &EnterSysProc::LoginReplyHandle);
	m_ReplyHandleMoniter.MonitorEvent(ASSIST_ID_REGISTER_ACK, &m_EnterSysProc, &EnterSysProc::RegisterReplyHandle);
	m_ReplyHandleMoniter.MonitorEvent(ASSIST_ID_ALTER_SUBJECTS_ACK, &m_ScoreProc, &ScoreProc::AlterSubjectReplyHandle);

	return true;
}

CEventProxy<Assist_ID, SOCKET, void*, unsigned int>& ProcMgr::GetRecvHandleMoniter()
{
	return m_RecvHandleMoniter;
}

CEventProxy<Assist_ID, SOCKET, MYSQL_RES*, string>& ProcMgr::GetReplyHandleMoniter()
{
	return m_ReplyHandleMoniter;
}