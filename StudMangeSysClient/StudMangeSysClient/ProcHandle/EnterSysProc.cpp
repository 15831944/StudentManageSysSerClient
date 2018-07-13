#include "EnterSysProc.h"
#include "ProcMgr.h"
#include "CheckTool.h"
#include "TCPHandle.h"

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

void EnterSysProc::StartProc()
{
	__super::StartProc();

	int iRealChoose = GetRealChooseByUserChoose(m_iMyChoose);
	if (iRealChoose >= GetMinRealChoose() && iRealChoose <= GetMaxRealChoose())
	{
		if (OPER_PER_INVALID != GetOperPerByRealChoose(iRealChoose))
		{
			SetCurOper(GetOperPerByRealChoose(iRealChoose));
			SwitchToOper(GetOperPerByRealChoose(iRealChoose));
		}
		else if (PROC_DEF_INVALID != GetProcDefByRealChoose(iRealChoose))
		{
			ProcMgr::GetInstance()->ProcSwitch(GetProcDefByRealChoose(iRealChoose));
		}
		else
		{
			//�˳�ϵͳ
			ExitSys();
		}
	}
	else
	{
		ProcMgr::GetInstance()->ProcSwitch(GetMyProcDef(), true);
	}
	
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
		printf("iAssistId[%d] error\n", iAssistId);
		ProcMgr::GetInstance()->ProcSwitch(GetMyProcDef(), true); //���µ�¼ע��
		break;
	}

	if (bRes)
		ProcMgr::GetInstance()->ProcSwitch(GetNextProc()); //����ɹ����л�����һ������
}

void EnterSysProc::EndRecv()
{
	__super::EndRecv();
}

void EnterSysProc::SwitchToOper(OperPermission CurOper)
{
	switch(CurOper)
	{
	case OPER_PER_LOGIN:
		LoginChooseHandle();
		break;
	case OPER_PER_REGISTER:
		RegisterChooseHandle();
		break;
	default:
		cout<<"û�иò�����"<<endl;
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
		SwitchToOper(GetCurOper());	
	

	string strPassword;
	cout<<"������������룺"<<endl;
	cin>>strPassword;
	if (!(CheckTool::CheckStringLen(strPassword, 30) && CheckTool::CheckStringByValid(strPassword, "A~Z|a~z|0~9|_|-")))
		SwitchToOper(GetCurOper());

	//���ͷ����
	SC_MSG_LOGIN_REQ SendReq;
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
		SwitchToOper(GetCurOper());	

	string strAccount;
	cout<<"����������û��������ڵ�¼����"<<endl;
	cin>>strAccount;
	if (!(CheckTool::CheckStringLen(strAccount, 30) && CheckTool::CheckStringByValid(strAccount, "A~Z|a~z|0~9")))
		SwitchToOper(GetCurOper());	

	string strPassword;
	cout<<"������������룺"<<endl;
	cin>>strPassword;
	if (!(CheckTool::CheckStringLen(strPassword, 30) && CheckTool::CheckStringByValid(strPassword, "A~Z|a~z|0~9|_|-")))
		SwitchToOper(GetCurOper());

	string strSex;
	cout<<"����������Ա�0-��  1-Ů����"<<endl;
	cin>>strSex;
	if (!(CheckTool::CheckStringLen(strSex, 1) && CheckTool::CheckStringByValid(strSex, "0|1")))
		SwitchToOper(GetCurOper());

	string strIdent;
	cout<<"���������ְҵ��1-ѧ�� 2-��ʦ����"<<endl;
	cin>>strIdent;
	if (!(CheckTool::CheckStringLen(strIdent, 1) && CheckTool::CheckStringByValid(strIdent, "1|2")))
		SwitchToOper(GetCurOper());

	//���ͷ����
	SC_MSG_REGISTER_REQ SendReq;
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
		return false;
	}
	if (DataLen != sizeof(CS_MSG_LOGIN_ACK))
	{
		printf("DataLen[%u] error, It should be [%u]\n", DataLen, sizeof(CS_MSG_LOGIN_ACK));
		return false;
	}
	CS_MSG_LOGIN_ACK* RecvMSG = (CS_MSG_LOGIN_ACK*) vpData;
	if (RecvMSG->bSucceed)
	{
		if (!ProcMgr::GetInstance()->SetVOperPer(RecvMSG->cOperPer))
		{
			printf("����Ȩ������ʧ�ܣ�\n");
			ProcMgr::GetInstance()->ProcSwitch(GetMyProcDef(), true); //���µ�¼ע��
			return false;
		}
		printf("��¼ϵͳ�ɹ�����ӭ����\n");
	}
	else
	{
		printf("��¼ϵͳʧ��\n");
		ProcMgr::GetInstance()->ProcSwitch(GetMyProcDef(), true); //���µ�¼ע��
		return false;
	}

	return true;
}

bool EnterSysProc::RegisterRecvHandle(void* vpData, unsigned int DataLen)
{
	if (OPER_PER_REGISTER != GetCurOper())
	{
		printf("���ǽ��иò���[%d]����ǰ���еĲ�����[%d] error\n", OPER_PER_REGISTER, GetCurOper());
		return false;
	}
	if (DataLen != sizeof(CS_MSG_REGISTER_ACK))
	{
		printf("DataLen[%u] error, It should be [%u]\n", DataLen, sizeof(CS_MSG_REGISTER_ACK));
		return false;
	}
	CS_MSG_REGISTER_ACK* RecvMSG = (CS_MSG_REGISTER_ACK*) vpData;
	if (RecvMSG->bSucceed)
	{
		if (!ProcMgr::GetInstance()->SetVOperPer(RecvMSG->cOperPer))
		{
			printf("����Ȩ������ʧ�ܣ�\n");
			ProcMgr::GetInstance()->ProcSwitch(GetMyProcDef(), true); //���µ�¼ע��
			return false;
		}
		printf("ע��ɹ�����ӭ������ϵͳ��\n");
	}
	else
	{
		printf("ע��ʧ��\n");
		ProcMgr::GetInstance()->ProcSwitch(GetMyProcDef(), true); //���µ�¼ע��
		return false;
	}

	return true;
}