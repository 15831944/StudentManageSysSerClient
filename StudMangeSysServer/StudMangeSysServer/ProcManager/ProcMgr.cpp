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


	/*
	* �ظ���Ϣ��������
	*/
	m_ReplyHandleMoniter.MonitorEvent(OPER_PER_LOGIN, &m_EnterSysProc, &EnterSysProc::LoginReplyHandle);
	m_ReplyHandleMoniter.MonitorEvent(OPER_PER_REGISTER, &m_EnterSysProc, &EnterSysProc::RegisterReplyHandle);

	return true;
}

CEventProxy<Assist_ID, SOCKET, void*, unsigned int>& ProcMgr::GetRecvHandleMoniter()
{
	return m_RecvHandleMoniter;
}

CEventProxy<OperPermission, SOCKET, MYSQL_RES*, int>& ProcMgr::GetReplyHandleMoniter()
{
	return m_ReplyHandleMoniter;
}