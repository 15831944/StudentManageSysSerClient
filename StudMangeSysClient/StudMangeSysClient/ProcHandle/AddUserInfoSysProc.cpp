#include "AddUserInfoSysProc.h"
#include "ProcMgr.h"
#include "UserInfoMgr.h"
#include "CheckTool.h"
#include "StringTool.h"
#include "TCPHandle.h"

AddUserInfoSysProc::AddUserInfoSysProc(ProcDef nProcDef) : BaseProc(nProcDef)
{
	initMapChoose();
}

AddUserInfoSysProc::~AddUserInfoSysProc()
{

}

bool AddUserInfoSysProc::initMapChoose()
{
	m_mChoose.insert(pair<int, ChooseData>(GetMaxRealChoose(), ChooseData("���������û���Ϣ", OPER_PER_ADDBATCHUSERINFO, PROC_DEF_ADDUSERINFOSYSPROC)));
	m_mChoose.insert(pair<int, ChooseData>(++GetMaxRealChoose(), ChooseData("���������û���Ϣ", OPER_PER_ADDSINGLEUSERINFO, PROC_DEF_ADDUSERINFOSYSPROC)));
	m_mChoose.insert(pair<int, ChooseData>(++GetMaxRealChoose(), ChooseData("����", OPER_PER_INVALID, PROC_DEF_USERINFOSYSPROC)));

	return true;
}

void AddUserInfoSysProc::EndProc()
{
	__super::EndProc();
}

void AddUserInfoSysProc::StartRecv(void* vpData, unsigned int DataLen, /*int iMianId,*/ int iAssistId)
{
	bool bRes = false;
	switch(iAssistId)
	{
	case ASSIST_ID_ADD_SINGLE_USERINFO_ACK:
		bRes = AddSingleUserInfoRecvHandle(vpData, DataLen);
		break;
	case ASSIST_ID_ADD_BATCH_USERINFO_ACK:
		break;
	default:
		printf("%s iAssistId[%d] error\n", __FUNCTION__, iAssistId);
		//ProcMgr::GetInstance()->ProcSwitch(GetMyProcDef(), true); //���µ�¼ע��
		SetIEndFlag(-1);
		break;
	}


	if (GetIEndFlag() == -1)
	{
		ProcMgr::GetInstance()->ProcSwitch(GetMyProcDef(), true);
	}
	else if (GetIEndFlag() == 1)
	{
		ProcMgr::GetInstance()->ProcSwitch(GetNextProc()); //����ɹ����л�����һ������
	}
	EndRecv(); //��ʹ��GetNextProc()��Ҫ���л����������  ���ǿ����������������⣬����л�����ͬһ������
}

void AddUserInfoSysProc::EndRecv()
{
	__super::EndRecv();
}

void AddUserInfoSysProc::SwitchToOper(OperPermission CurOper)
{
	__super::SwitchToOper(CurOper);


	switch(CurOper)
	{
	case OPER_PER_ADDSINGLEUSERINFO: //���������û���Ϣ����
		AddSingleUserInfoChooseHandle();
		break;
	case OPER_PER_ADDBATCHUSERINFO: //���Ƶ������ӳɼ�  ����ǰ������
		break;
	default:
		printf("%s û�иò�����\n", __FUNCTION__);
		ProcMgr::GetInstance()->ProcSwitch(GetMyProcDef(), true);  
		break;
	}
}

void AddUserInfoSysProc::AddSingleUserInfoChooseHandle()
{
	string strName;
	cout<<"��������Ӷ����������"<<endl;
	cin>>strName;
	if (!(CheckTool::CheckStringLen(strName, 30) && CheckTool::CheckStringByValid(strName, "")))
	{
		OperInputErrorHandle();
		return;
	}

	string strAccount;
	cout<<"��������Ӷ�����û��������ڵ�¼����"<<endl;
	cin>>strAccount;
	if (!(CheckTool::CheckStringLen(strAccount, 30) && CheckTool::CheckStringByValid(strAccount, "A~Z|a~z|0~9")))
	{
		OperInputErrorHandle();
		return;
	}	

	string strPassword;
	cout<<"��������Ӷ�������룺"<<endl;
	cin>>strPassword;
	if (!(CheckTool::CheckStringLen(strPassword, 30) && CheckTool::CheckStringByValid(strPassword, "A~Z|a~z|0~9|_|-")))
	{
		OperInputErrorHandle();	
		return;
	}

	string strSex;
	cout<<"��������Ӷ�����Ա�0-��  1-Ů����"<<endl;
	cin>>strSex;
	if (!(CheckTool::CheckStringLen(strSex, 1) && CheckTool::CheckStringByValid(strSex, "0|1")))
	{
		OperInputErrorHandle();
		return;
	}

	string strIdent;
	string strValid;
	if (UserInfoMgr::GetInstance()->GetUserIdent() == 2 || UserInfoMgr::GetInstance()->GetUserIdent() == 1) //ѧ�������Ȩ�����������Ҳ����ʹ�øù��ܡ�����ֻ�ܺ���ʦ��Ȩ��һ��
	{
		//cout<<"��������Ӷ����ְҵ��1-ѧ������"<<endl;
		strIdent = "1";
		strValid = "1";
	}
	else if (UserInfoMgr::GetInstance()->GetUserIdent() == 3) //����Աѡ��
	{
		cout<<"��������Ӷ����ְҵ��1-ѧ�� 2-��ʦ����"<<endl;
		cin>>strIdent;
		strValid = "1|2";
	}
	else
	{
		printf("���Ϸ�����߱�ʶ\n");
		OperInputErrorHandle(true, 1);
		return;
	}
	if (!(CheckTool::CheckStringLen(strIdent, 1) && CheckTool::CheckStringByValid(strIdent, strValid)))
	{
		OperInputErrorHandle();
		return;
	}

	string strMajor;
	cout<<"��������Ӷ����רҵ��"<<endl;
	cin>>strMajor;
	if (!(CheckTool::CheckStringLen(strMajor, 40) && CheckTool::CheckStringByValid(strMajor, "")))
	{
		OperInputErrorHandle();
		return;
	}	

	string strGrade;
	cout<<"��������Ӷ���İ༶��"<<endl;
	cin>>strGrade;
	if (!(CheckTool::CheckStringLen(strGrade, 30) && CheckTool::CheckStringByValid(strGrade, "")))
	{
		OperInputErrorHandle();
		return;
	}	

	//���ͷ����
	CS_MSG_ADD_SINGLE_USERINFO_REQ SendReq;
	SendReq.sSex = (unsigned char)atoi(strSex.c_str());
	SendReq.sIdent = (unsigned char)atoi(strIdent.c_str());
	strcpy_s(SendReq.cName, sizeof(SendReq.cName), strName.c_str());
	strcpy_s(SendReq.cAccount, sizeof(SendReq.cAccount), strAccount.c_str());
	strcpy_s(SendReq.cPWD, sizeof(SendReq.cPWD), strPassword.c_str());
	strcpy_s(SendReq.cMajor, sizeof(SendReq.cMajor), strMajor.c_str());
	strcpy_s(SendReq.cGrade, sizeof(SendReq.cGrade), strGrade.c_str());
	
	TCPHandle::GetInstance()->Send(&SendReq, sizeof(SendReq), /*MAIN_ID_LOGINREGISTER,*/ ASSIST_ID_ADD_SINGLE_USERINFO_REQ);
}

bool AddUserInfoSysProc::AddSingleUserInfoRecvHandle(void* vpData, unsigned int DataLen)
{
	if (OPER_PER_ADDSINGLEUSERINFO != GetCurOper())
	{
		printf("���ǽ��иò���[%d]����ǰ���еĲ�����[%d] error\n", OPER_PER_ADDSINGLEUSERINFO, GetCurOper());
		SetIEndFlag(-1);
		return false;
	}
	if (DataLen != sizeof(CS_MSG_ADD_SINGLE_USERINFO_ACK))
	{
		printf("DataLen[%u] error, It should be [%u]\n", DataLen, sizeof(CS_MSG_ADD_SINGLE_USERINFO_ACK));
		SetIEndFlag(-1);
		return false;
	}
	CS_MSG_ADD_SINGLE_USERINFO_ACK* RecvMSG = (CS_MSG_ADD_SINGLE_USERINFO_ACK*) vpData;
	if (RecvMSG->bSucceed)
	{
		printf("��������˺�[%s]�û���Ϣ�ɹ�\n", RecvMSG->cAccount);
		SetIEndFlag(1);
	}
	else
	{
		printf("���������û���Ϣʧ��\n");
		SetIEndFlag(-1);
		return false;
	}

	return true;
}