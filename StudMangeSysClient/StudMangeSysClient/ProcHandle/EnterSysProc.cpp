#include "EnterSysProc.h"
#include "ProcMgr.h"
#include "CheckTool.h"
#include "TCPHandle.h"
#include "UserInfoMgr.h"

EnterSysProc::EnterSysProc(ProcDef nProcDef) : BaseProc(nProcDef)
{
	initMapChoose();
}

EnterSysProc::~EnterSysProc()
{

}

bool EnterSysProc::initMapChoose()
{
	m_mChoose.insert(pair<int, ChooseData>(GetMaxRealChoose(), ChooseData("��¼", OPER_PER_LOGIN, PROC_DEF_COMMONSYSPROC)));
	m_mChoose.insert(pair<int, ChooseData>(++GetMaxRealChoose(), ChooseData("ע��", OPER_PER_REGISTER, PROC_DEF_COMMONSYSPROC)));
	m_mChoose.insert(pair<int, ChooseData>(++GetMaxRealChoose(), ChooseData("�˳�ϵͳ", OPER_PER_INVALID, PROC_DEF_INVALID)));

	return true;
}


void EnterSysProc::EndProc()
{
	__super::EndProc();
}


void EnterSysProc::StartRecv(void* vpData, unsigned int DataLen, /*int iMianId,*/ int iAssistId)
{
#if 0
	if (MAIN_ID_LOGINREGISTER != iMianId)
	{
		printf("iMianId[%d] error��It should be [%d] \n", iMianId, MAIN_ID_LOGINREGISTER);
		return;
	}
#endif

	__super::StartRecv(vpData, DataLen, iAssistId);

	bool bRes = false;
	switch(iAssistId)
	{
	case ASSIST_ID_LOGIN_ACK:
		bRes = LoginRecvHandle(vpData, DataLen);
		break;
	case ASSIST_ID_REGISTER_ACK:
		bRes = RegisterRecvHandle(vpData, DataLen);
		break;
	default:
		if (GetIEndFlag() == 0)
		{
			printf("%s iAssistId[%d] error\n", __FUNCTION__, iAssistId);
			//ProcMgr::GetInstance()->ProcSwitch(GetMyProcDef(), true); //���µ�¼ע��
			SetIEndFlag(-1);
		}
		break;
	}


// 	if (bRes)
// 		ProcMgr::GetInstance()->ProcSwitch(GetNextProc()); //����ɹ����л�����һ������
	if (GetIEndFlag() == -1)
	{
		ProcMgr::GetInstance()->ProcSwitch(GetMyProcDef(), true);
	}
	else if (GetIEndFlag() == 1)
	{
		ProcMgr::GetInstance()->ProcSwitch(GetNextProc()); //����ɹ����л�����һ������
	}
	EndRecv();//��ʹ��GetNextProc()��Ҫ���л����������  ���ǿ����������������⣬����л�����ͬһ������
}

void EnterSysProc::EndRecv()
{
	__super::EndRecv();
}

void EnterSysProc::SwitchToOper(OperPermission CurOper)
{
	__super::SwitchToOper(CurOper);

	switch(CurOper)
	{
	case OPER_PER_LOGIN:
		LoginChooseHandle();
		break;
	case OPER_PER_REGISTER:
		RegisterChooseHandle();
		break;
	default:
		printf("%s û�иò�����\n", __FUNCTION__);
		ProcMgr::GetInstance()->ProcSwitch(GetMyProcDef(), true);  //���µ�¼ע��
		break;
	}
}

void EnterSysProc::LoginChooseHandle()
{
	string strAccount;
	cout<<"����������û�����"<<endl;
	cin>>strAccount;
	if (!(CheckTool::CheckStringLen(strAccount, 30) && CheckTool::CheckStringByValid(strAccount, "A~Z|a~z|0~9")))
	{
		OperInputErrorHandle();
		return;
	}
	

	string strPassword;
	cout<<"������������룺"<<endl;
	cin>>strPassword;
	if (!(CheckTool::CheckStringLen(strPassword, 30) && CheckTool::CheckStringByValid(strPassword, "A~Z|a~z|0~9|_|-")))
	{
		OperInputErrorHandle();
		return;
	}

	if (0 != m_iOperInputErrorLimit)
		m_iOperInputErrorLimit = 0;

	//���ͷ����
	CS_MSG_LOGIN_REQ SendReq;
	strcpy_s(SendReq.cAccount, sizeof(SendReq.cAccount), strAccount.c_str());
	strcpy_s(SendReq.cPWD, sizeof(SendReq.cPWD), strPassword.c_str());
	//SendReq.OperPerId = GetMyProcDef();
	TCPHandle::GetInstance()->Send(&SendReq, sizeof(SendReq), /*MAIN_ID_LOGINREGISTER,*/ ASSIST_ID_LOGIN_REQ);
}

void EnterSysProc::RegisterChooseHandle()
{
	string strName;
	cout<<"���������������"<<endl;
	cin>>strName;
	if (!(CheckTool::CheckStringLen(strName, 30) && CheckTool::CheckStringByValid(strName, "")))
	{
		OperInputErrorHandle();
		return;
	}

	string strAccount;
	cout<<"����������û��������ڵ�¼����"<<endl;
	cin>>strAccount;
	if (!(CheckTool::CheckStringLen(strAccount, 30) && CheckTool::CheckStringByValid(strAccount, "A~Z|a~z|0~9")))
	{
		OperInputErrorHandle();
		return;
	}	

	string strPassword;
	cout<<"������������룺"<<endl;
	cin>>strPassword;
	if (!(CheckTool::CheckStringLen(strPassword, 30) && CheckTool::CheckStringByValid(strPassword, "A~Z|a~z|0~9|_|-")))
	{
		OperInputErrorHandle();	
		return;
	}

	string strSex;
	cout<<"����������Ա�0-��  1-Ů����"<<endl;
	cin>>strSex;
	if (!(CheckTool::CheckStringLen(strSex, 1) && CheckTool::CheckStringByValid(strSex, "0|1")))
	{
		OperInputErrorHandle();
		return;
	}

	string strIdent;
	cout<<"���������ְҵ��1-ѧ�� 2-��ʦ����"<<endl;
	cin>>strIdent;
	if (!(CheckTool::CheckStringLen(strIdent, 1) && CheckTool::CheckStringByValid(strIdent, "1|2")))
	{
		OperInputErrorHandle();
		return;
	}

	if (0 != m_iOperInputErrorLimit)
		m_iOperInputErrorLimit = 0;

	//���ͷ����
	CS_MSG_REGISTER_REQ SendReq;
	strcpy_s(SendReq.cName, sizeof(SendReq.cName), strName.c_str());
	strcpy_s(SendReq.cAccount, sizeof(SendReq.cAccount), strAccount.c_str());
	strcpy_s(SendReq.cPWD, sizeof(SendReq.cPWD), strPassword.c_str());
	strcpy_s(SendReq.cSex, sizeof(SendReq.cSex), strSex.c_str());
	strcpy_s(SendReq.cIdent, sizeof(SendReq.cIdent), strIdent.c_str());
	//SendReq.OperPerId = GetMyProcDef();
	TCPHandle::GetInstance()->Send(&SendReq, sizeof(SendReq), /*MAIN_ID_LOGINREGISTER,*/ ASSIST_ID_REGISTER_REQ);
}

bool EnterSysProc::LoginRecvHandle(void* vpData, unsigned int DataLen)
{
	if (OPER_PER_LOGIN != GetCurOper())
	{
		printf("���ǽ��иò���[%d]����ǰ���еĲ�����[%d] error\n", OPER_PER_LOGIN, GetCurOper());
		SetIEndFlag(-1);
		return false;
	}
	if (DataLen != sizeof(SC_MSG_LOGIN_ACK))
	{
		printf("DataLen[%u] error, It should be [%u]\n", DataLen, sizeof(SC_MSG_LOGIN_ACK));
		SetIEndFlag(-1);
		return false;
	}
	SC_MSG_LOGIN_ACK* RecvMSG = (SC_MSG_LOGIN_ACK*) vpData;
	if (RecvMSG->bSucceed)
	{
		if (!UserInfoMgr::GetInstance()->InitVOperPer() || !UserInfoMgr::GetInstance()->SetVOperPer(RecvMSG->cOperPer))
		{
			printf("����Ȩ������ʧ�ܣ�\n");
			//ProcMgr::GetInstance()->ProcSwitch(GetMyProcDef(), true); //���µ�¼ע��
			SetIEndFlag(-1);
			return false;
		}

		UserInfoMgr::GetInstance()->SetSomeInfo(RecvMSG->cName, RecvMSG->cAccount, RecvMSG->iUserId, RecvMSG->sIdent, RecvMSG->sSex);

		printf(">>>�˺�[%s]��¼ϵͳ�ɹ�����ӭ��<<<��\n", RecvMSG->cAccount);
		SetIEndFlag(1);
	}
	else
	{
		printf("***��¼ϵͳʧ��***\n");
		//ProcMgr::GetInstance()->ProcSwitch(GetMyProcDef(), true); //���µ�¼ע��
		SetIEndFlag(-1);
		return false;
	}

	return true;
}

bool EnterSysProc::RegisterRecvHandle(void* vpData, unsigned int DataLen)
{
	if (OPER_PER_REGISTER != GetCurOper())
	{
		printf("���ǽ��иò���[%d]����ǰ���еĲ�����[%d] error\n", OPER_PER_REGISTER, GetCurOper());
		SetIEndFlag(-1);
		return false;
	}
	if (DataLen != sizeof(SC_MSG_REGISTER_ACK))
	{
		printf("DataLen[%u] error, It should be [%u]\n", DataLen, sizeof(SC_MSG_REGISTER_ACK));
		SetIEndFlag(-1);
		return false;
	}
	SC_MSG_REGISTER_ACK* RecvMSG = (SC_MSG_REGISTER_ACK*) vpData;
	if (RecvMSG->bSucceed)
	{
		if (!UserInfoMgr::GetInstance()->InitVOperPer() || !UserInfoMgr::GetInstance()->SetVOperPer(RecvMSG->cOperPer))
		{
			printf("����Ȩ������ʧ�ܣ�\n");
			//ProcMgr::GetInstance()->ProcSwitch(GetMyProcDef(), true); //���µ�¼ע��
			SetIEndFlag(-1);
			return false;
		}

		UserInfoMgr::GetInstance()->SetSomeInfo(RecvMSG->cName, RecvMSG->cAccount, RecvMSG->iUserId, RecvMSG->sIdent, RecvMSG->sSex);

		printf(">>>�˺�[%s]ע��ɹ�����ӭ������ϵͳ��<<<\n", RecvMSG->cAccount);
		SetIEndFlag(1);
	}
	else
	{
		printf("***ע��ʧ��***\n");
		//ProcMgr::GetInstance()->ProcSwitch(GetMyProcDef(), true); //���µ�¼ע��
		SetIEndFlag(-1);
		return false;
	}

	return true;
}