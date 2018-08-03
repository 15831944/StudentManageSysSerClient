#include "SelectUserInfoSysProc.h"
#include "ProcMgr.h"
#include "CheckTool.h"
#include "StringTool.h"
#include "TCPHandle.h"
#include "UserInfoMgr.h"

SelectUserInfoSysProc::SelectUserInfoSysProc(ProcDef nProcDef) : BaseProc(nProcDef)
{
	initMapChoose();
}

SelectUserInfoSysProc::~SelectUserInfoSysProc()
{

}

bool SelectUserInfoSysProc::initMapChoose()
{
	m_mChoose.insert(pair<int, ChooseData>(GetMaxRealChoose(), ChooseData("������ѯ�û���Ϣ", OPER_PER_SELECTBATCHUSERINFO, PROC_DEF_SELECTUSERINFOSYSPROC)));
	m_mChoose.insert(pair<int, ChooseData>(++GetMaxRealChoose(), ChooseData("������ѯ�û���Ϣ", OPER_PER_SELECTSINGLEUSERINFO, PROC_DEF_SELECTUSERINFOSYSPROC)));
	m_mChoose.insert(pair<int, ChooseData>(++GetMaxRealChoose(), ChooseData("����", OPER_PER_INVALID, PROC_DEF_USERINFOSYSPROC)));

	return true;
}

void SelectUserInfoSysProc::EndProc()
{
	__super::EndProc();
}

void SelectUserInfoSysProc::StartRecv(void* vpData, unsigned int DataLen, /*int iMianId,*/ int iAssistId)
{
	bool bRes = false;
	switch(iAssistId)
	{
	case ASSIST_ID_SELECT_SINGLE_USERINFO_ACK:
		bRes = SelectSingleUserInfoRecvHandle(vpData, DataLen);
		break;
	case ASSIST_ID_SELECT_BATCH_USERINFO_ACK:
		bRes = SelectBatchUserInfoRecvHandle(vpData, DataLen);
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

void SelectUserInfoSysProc::EndRecv()
{
	__super::EndRecv();
}

void SelectUserInfoSysProc::SwitchToOper(OperPermission CurOper)
{
	__super::SwitchToOper(CurOper);


	switch(CurOper)
	{
	case OPER_PER_SELECTBATCHUSERINFO: //������ѯ�û���Ϣ
		SelectBatchUserInfoChooseHandle();
		break;
	case OPER_PER_SELECTSINGLEUSERINFO: //������ѯ�û���Ϣ
		SelectSingleUserInfoChooseHandle();
		break;
	default:
		printf("%s û�иò�����\n", __FUNCTION__);
		ProcMgr::GetInstance()->ProcSwitch(GetMyProcDef(), true);  
		break;
	}
}

void SelectUserInfoSysProc::SelectSingleUserInfoChooseHandle()
{
	if (OPER_PER_SELECTSINGLEUSERINFO != GetCurOper())
	{
		printf("%s ���ǽ��иò���[%d]����ǰ���еĲ�����[%d] error\n", __FUNCTION__, OPER_PER_SELECTSINGLEUSERINFO, GetCurOper());
		OperInputErrorHandle(true, 1);
		return;
	}

	CS_MSG_SELECT_SINGLE_USERINFO_REQ SendReq;
	SendReq.uRecordDataTmp = 0;
	short sIdent = UserInfoMgr::GetInstance()->GetUserIdent();

	
	/*
	* ��ݱ�ʶ��Ȩ�����⣺
	* 1.��Щ��������ЩȨ�ޣ�
	* 2.������ݱ�ʶȷ���ɲ鿴�Ķ������ͣ���������ݱ�ʾ�������ͣ���һ���ǿɲ鿴�Լ��ͱ��Լ���ݱ�ʶ�͵��û���
	* 3.�ɲ鿴����Щ�ֶΣ���Щ�ֶβ��ɲ鿴��
	*/
	//�����û���ǰ��ݱ�ʶ�õ��ɻ�ȡ�Ĳ�ͬ��ݱ�ʶ���û���Ϣ����ʦ��ѧ�����ܿ����Լ������룻����Ա���Կ�����ʦ��ѧ�������룬���ǲ��ܿ����Լ��ģ���ʦҲ���ܿ���ѧ�������롣����ɲ��ɿ����ɷ���������δ���
	if (sIdent== 1)//�����ģ�ֻ�ܲ�ѯ�Լ���Ҳֻ�ܸ����Լ���
	{
		string strAccount = UserInfoMgr::GetInstance()->GetUserAccount();
		strcpy_s(SendReq.cAccount, sizeof(SendReq.cAccount), strAccount.c_str());
	}
	else if (sIdent == 2 || sIdent == 3) //��ʦ���Բ�ѯѧ�����Լ��ģ�Ҳ���Ը���ѧ�����Լ��ġ�����Ա���Բ�ѯ��ʦѧ�����Լ��ģ�Ҳ���Ը��Ľ�ʦѧ�����Լ���
	{
		string strInputTmp = "��ѯѧ��";
		if (sIdent == 3)
		{
			strInputTmp = "��ѯѧ�����ʦ";
		}
		printf("1-��ѯ�Լ� 2-%s\n", strInputTmp.c_str());
		string strChoose;
		cin>>strChoose;
		if (!(CheckTool::CheckStringLen(strChoose, 1) && CheckTool::CheckStringByValid(strChoose, "1|2")))
		{
			OperInputErrorHandle();
			return;
		}
		if (strChoose == "2")
		{
			cout<<"�������ѯ������û�����"<<endl;
			string strAccount;
			cin>>strAccount;
			if (!(CheckTool::CheckStringLen(strAccount, 30) && CheckTool::CheckStringByValid(strAccount, "A~Z|a~z|0~9")))
			{
				OperInputErrorHandle();
				return;
			}
			strcpy_s(SendReq.cAccount, sizeof(SendReq.cAccount), strAccount.c_str());
		}
		else
		{
			string strAccount = UserInfoMgr::GetInstance()->GetUserAccount();
			strcpy_s(SendReq.cAccount, sizeof(SendReq.cAccount), strAccount.c_str());
		}
	}
	else
	{
		printf("%s �û�������ݱ�ʶ[%d]����\n", __FUNCTION__, UserInfoMgr::GetInstance()->GetUserIdent());
		OperInputErrorHandle(true, 1);
		return;
	}


	TCPHandle::GetInstance()->Send(&SendReq, sizeof(SendReq), /*MAIN_ID_LOGINREGISTER,*/ ASSIST_ID_SELECT_SINGLE_USERINFO_REQ);
}

void SelectUserInfoSysProc::SelectBatchUserInfoChooseHandle()
{

}

bool SelectUserInfoSysProc::SelectSingleUserInfoRecvHandle(void* vpData, unsigned int DataLen)
{
	if (OPER_PER_SELECTSINGLEUSERINFO != GetCurOper())
	{
		printf("%s ���ǽ��иò���[%d]����ǰ���еĲ�����[%d] error\n", __FUNCTION__, OPER_PER_SELECTSINGLEUSERINFO, GetCurOper());
		SetIEndFlag(-1);
		return false;
	}
	if (DataLen != sizeof(SC_MSG_SELECT_SINGLE_USERINFO_ACK))
	{
		printf("DataLen[%u] error, It should be [%u]\n", DataLen, sizeof(SC_MSG_SELECT_SINGLE_USERINFO_ACK));
		SetIEndFlag(-1);
		return false;
	}

	SC_MSG_SELECT_SINGLE_USERINFO_ACK* RecvMSG = (SC_MSG_SELECT_SINGLE_USERINFO_ACK*) vpData;
	if (RecvMSG->bResCode == 1)
	{
		printf("%s ������ѯʧ��\n", __FUNCTION__);
		SetIEndFlag(-1);
		return false;
	}
	else if (RecvMSG->bResCode == 2)
	{
		printf("%s û���û���Ϣ\n", __FUNCTION__);
		SetIEndFlag(-1);
		return false;
	}
	else if (RecvMSG->bResCode == 3)
	{
		printf("%s ��ݱ�ʶ�����ϲ�ѯ���˸��û�\n", __FUNCTION__);
		SetIEndFlag(-1);
		return false;
	}
	else if (RecvMSG->bResCode == 4)
	{
		printf("%s �����쳣\n", __FUNCTION__);
		SetIEndFlag(-1);
		return false;
	}


	vector<string> vSelectFieldShow;
	vector<string> vSelectResultShow;
	vSelectFieldShow.push_back("ѧ��");
	vSelectResultShow.push_back(StringTool::NumberToStr((int)RecvMSG->uUserID));
	vSelectFieldShow.push_back("����");
	vSelectResultShow.push_back(RecvMSG->cName);
	vSelectFieldShow.push_back("�˺�");
	vSelectResultShow.push_back(RecvMSG->cAccount);
	vSelectFieldShow.push_back("�༶");
	vSelectResultShow.push_back(RecvMSG->cGrade);
	vSelectFieldShow.push_back("����");
	vSelectResultShow.push_back(RecvMSG->cPWD);//�ɷ���������δ���
	vSelectFieldShow.push_back("�Ա�");
	vSelectResultShow.push_back(RecvMSG->sSex==0?"��":(RecvMSG->sSex==1?"Ů":"NULL"));
	vSelectFieldShow.push_back("��ݱ�ʶ");
	vSelectResultShow.push_back(RecvMSG->sIdent==1?"ѧ��":(RecvMSG->sSex==2?"��ʦ":(RecvMSG->sSex==3?"����Ա":"NULL")));
	vSelectFieldShow.push_back("רҵ");
	vSelectResultShow.push_back(RecvMSG->cMajor);

	ShowSelectSingleScoreResult(vSelectFieldShow, vSelectResultShow, true);
	SetIEndFlag(1);

	return true;
}

bool SelectUserInfoSysProc::SelectBatchUserInfoRecvHandle(void* vpData, unsigned int DataLen)
{

	return true;
}


void SelectUserInfoSysProc::ShowSelectSingleScoreResult(vector<string> vSelectFieldShow, vector<string> vSelectResultShow, bool bShow)
{
	if (vSelectFieldShow.empty() || vSelectFieldShow.empty())
	{
		printf("û����ʾ��Ϣ\n");
		return;
	}
	if (vSelectFieldShow.size() != vSelectFieldShow.size())
	{
		printf("�ֶ�������������ͬ\n");
		return;
	}

	string strFieldShow;
	string strResult;

	for (unsigned i=0; i<vSelectFieldShow.size(); i++)
	{
		if (i == vSelectFieldShow.size()-1)
		{
			strFieldShow += vSelectFieldShow.at(i);
			strResult += vSelectResultShow.at(i);
		}
		else
		{
			strFieldShow += StringTool::SetStringFieldWidth(vSelectFieldShow.at(i), 12) ;
			strResult += StringTool::SetStringFieldWidth(vSelectResultShow.at(i), 12);
		}

	}

	if (bShow)
	{
		printf("-------------------------------\n");
		printf("��ѯ�����ʾ��\n");
		cout<<strFieldShow<<endl;
	}
	cout<<strResult<<endl;
}