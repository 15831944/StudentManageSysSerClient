#include "AuthoritySysProc.h"
#include "ProcMgr.h"
#include "CheckTool.h"
#include "StringTool.h"
#include "TCPHandle.h"
#include "UserInfoMgr.h"

AuthoritySysProc::AuthoritySysProc(ProcDef nProcDef) : BaseProc(nProcDef)
{
	initMapChoose();
}

AuthoritySysProc::~AuthoritySysProc()
{

}

bool AuthoritySysProc::initMapChoose()
{
	m_mChoose.insert(pair<int, ChooseData>(GetMaxRealChoose(), ChooseData("�����û�Ȩ��", OPER_PER_ADDAUTHORITY, PROC_DEF_AUTHORITYSYSPROC)));
	m_mChoose.insert(pair<int, ChooseData>(++GetMaxRealChoose(), ChooseData("ɾ���û�Ȩ��", OPER_PER_DELETEAUTHORITY, PROC_DEF_AUTHORITYSYSPROC)));
	m_mChoose.insert(pair<int, ChooseData>(++GetMaxRealChoose(), ChooseData("����", OPER_PER_INVALID, PROC_DEF_COMMONSYSPROC)));

	return true;
}

void AuthoritySysProc::EndProc()
{
	__super::EndProc();
}

void AuthoritySysProc::StartRecv(void* vpData, unsigned int DataLen, /*int iMianId,*/ int iAssistId)
{
	bool bRes = false;
	switch(iAssistId)
	{
	case ASSIST_ID_GET_AUTHORITY_ACK:
		bRes = GetAuthoritbyAfterEditAuthoritbyRecvHandle(vpData, DataLen);
		break;
	case ASSIST_ID_EDIT_AUTHORITY_ACK:
		bRes = EditAuthoritRecvHandle(vpData, DataLen);
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

void AuthoritySysProc::EndRecv()
{
	__super::EndRecv();
}

void AuthoritySysProc::SwitchToOper(OperPermission CurOper)
{
	switch(CurOper)
	{
	case OPER_PER_ADDAUTHORITY: //����ĳ�û�һ�ֻ��߶���Ȩ��
	case OPER_PER_DELETEAUTHORITY: //ɾ��ĳ�û�һ�ֻ��߶���Ȩ��
		GetAuthoritbyAfterEditAuthoritbyChooseHandle();
		break;
	default:
		printf("%s û�иò�����\n", __FUNCTION__);
		ProcMgr::GetInstance()->ProcSwitch(GetMyProcDef(), true);  
		break;
	}
}

void AuthoritySysProc::GetAuthoritbyAfterEditAuthoritbyChooseHandle()
{
	if (OPER_PER_ADDAUTHORITY != GetCurOper() && OPER_PER_DELETEAUTHORITY != GetCurOper())
	{
		printf("���ǽ��иò���[%d | %d]����ǰ���еĲ�����[%d] error\n", OPER_PER_ADDAUTHORITY, OPER_PER_DELETEAUTHORITY, GetCurOper());
		OperInputErrorHandle(true, 1);
		return;
	}

	CS_MSG_GET_AUTHORITY_REQ SendReq;
	if (OPER_PER_ADDAUTHORITY == GetCurOper())
	{
		SendReq.sType = 2;
	}
	else
	{
		SendReq.sType = 1;
	}

	cout<<"�������ȡ����ɾȨ�޶�����û�����"<<endl;
	string strAccount;
	cin>>strAccount;
	if (!(CheckTool::CheckStringLen(strAccount, 30) && CheckTool::CheckStringByValid(strAccount, "A~Z|a~z|0~9")))
	{
		OperInputErrorHandle();
		return;
	}
	strcpy_s(SendReq.cAccount, sizeof(SendReq.cAccount), strAccount.c_str());

	TCPHandle::GetInstance()->Send(&SendReq, sizeof(SendReq), /*MAIN_ID_LOGINREGISTER,*/ ASSIST_ID_GET_AUTHORITY_REQ);
}

void AuthoritySysProc::EditAuthoritChooseHandle(unsigned char sType, char* cAccount, unsigned char* cAuthority, unsigned char cAuthorityCount)
{
	if (cAccount == NULL || cAuthority == NULL || 0 == cAuthorityCount || cAuthorityCount > MAXAUTHORITBYCOUNT)
	{
		printf("%s ������˺��ֶλ�Ȩ���ֶ�ΪNULL������Ȩ����Ŀ�쳣[%u]\n", __FUNCTION__, (unsigned)cAuthorityCount);
		SetIEndFlag(-1);
		return;
	}
	if (sType < 1 || sType > 2)
	{
		printf("%s Ȩ�޲������Ͳ���sType[%u]\n", __FUNCTION__, (unsigned)sType);
		SetIEndFlag(-1);
		return;
	}

	//�ȼ�鴫�͹�����Ȩ���Ƿ�Ϸ�
	for (unsigned char i=0; i<cAuthorityCount; i++)
	{
		if (!UserInfoMgr::GetInstance()->CanFindInAllOperPer((OperPermission)cAuthority[i]))
		{
			printf("%s ����Ȩ�޲��Ϸ�[%u]\n", __FUNCTION__, (unsigned)cAuthority[i]);
			SetIEndFlag(-1);
			return;
		}
	}
	//����ʾ����ɾȨ�ޣ���ѡ��
	ShowAuthoritby(cAuthority, cAuthorityCount);
	
	if (sType == 1)
	{
		printf("��������Ҫɾ�������Ȩ�ޣ���ʽΪ XX|XX|XX����\n");
	}
	else
	{
		printf("��������Ҫ���Ӷ����Ȩ�ޣ���ʽΪ XX|XX|XX����\n");
	}
	string strOperPer;
	cin>>strOperPer;
	if (!(CheckTool::CheckStringLen(strOperPer, 90) && CheckStringVaildRemoveSpl(strOperPer, "|", "0~9"))) 
	{
		OperInputErrorHandle(false);
		return;
	}
	vector<string> vecStrOperPer = StringTool::Splite(strOperPer);

	CS_MSG_EDIT_AUTHORITY_REQ SendReq;
	SendReq.sType = sType;
	SendReq.cAuthorityCount = 0;
	for (; SendReq.cAuthorityCount<vecStrOperPer.size() && SendReq.cAuthorityCount<MAXAUTHORITBYCOUNT; SendReq.cAuthorityCount++)
	{
		SendReq.cAuthority[SendReq.cAuthorityCount] = (unsigned char)atoi(vecStrOperPer.at(SendReq.cAuthorityCount).c_str());
	}
	strcpy_s(SendReq.cAccount, sizeof(SendReq.cAccount), cAccount);

	TCPHandle::GetInstance()->Send(&SendReq, sizeof(SendReq), /*MAIN_ID_LOGINREGISTER,*/ ASSIST_ID_EDIT_AUTHORITY_REQ);
}

bool AuthoritySysProc::GetAuthoritbyAfterEditAuthoritbyRecvHandle(void* vpData, unsigned int DataLen)
{
	if (OPER_PER_ADDAUTHORITY != GetCurOper() && OPER_PER_DELETEAUTHORITY != GetCurOper())
	{
		printf("���ǽ��иò���[%d | %d]����ǰ���еĲ�����[%d] error\n", OPER_PER_ADDAUTHORITY, OPER_PER_DELETEAUTHORITY, GetCurOper());
		SetIEndFlag(-1);
		return false;
	}
	if (DataLen != sizeof(CS_MSG_GET_AUTHORITY_ACK))
	{
		printf("DataLen[%u] error, It should be [%u]\n", DataLen, sizeof(CS_MSG_GET_AUTHORITY_ACK));
		SetIEndFlag(-1);
		return false;
	}

	CS_MSG_GET_AUTHORITY_ACK* RecvMSG = (CS_MSG_GET_AUTHORITY_ACK*) vpData;
	if (RecvMSG->bSucceed)
	{
		EditAuthoritChooseHandle(RecvMSG->sType, RecvMSG->cAccount, RecvMSG->cAuthority, RecvMSG->cAuthorityCount);
	}
	else
	{
		printf("��ȡ�û�����ɾȨ�޷��ز��ɹ�\n");
		SetIEndFlag(-1);
		return false;
	}

	return true;
}

bool AuthoritySysProc::EditAuthoritRecvHandle(void* vpData, unsigned int DataLen)
{
	if (OPER_PER_ADDAUTHORITY != GetCurOper() && OPER_PER_DELETEAUTHORITY != GetCurOper())
	{
		printf("���ǽ��иò���[%d | %d]����ǰ���еĲ�����[%d] error\n", OPER_PER_ADDAUTHORITY, OPER_PER_DELETEAUTHORITY, GetCurOper());
		SetIEndFlag(-1);
		return false;
	}
	if (DataLen != sizeof(CS_MSG_EDIT_AUTHORITY_ACK))
	{
		printf("DataLen[%u] error, It should be [%u]\n", DataLen, sizeof(CS_MSG_EDIT_AUTHORITY_ACK));
		SetIEndFlag(-1);
		return false;
	}

	CS_MSG_EDIT_AUTHORITY_ACK* RecvMSG = (CS_MSG_EDIT_AUTHORITY_ACK*) vpData;
	if (RecvMSG->sType < 1 || RecvMSG->sType > 2)
	{
		printf("%s ����Ȩ�޲������Ͳ���RecvMSG->sType[%u]\n", __FUNCTION__, (unsigned)RecvMSG->sType);
		SetIEndFlag(-1);
		return false;
	}

	if (RecvMSG->bSucceed)
	{
		printf("���˺�[%s]%sȨ�޳ɹ�\n", RecvMSG->cAccount, (RecvMSG->sType==1?"ɾ��":"����"));
		SetIEndFlag(1);
	}
	else
	{
		printf("��ɾȨ�޲������ز��ɹ�\n");
		SetIEndFlag(-1);
		return false;
	}

	return true;
}


void AuthoritySysProc::ShowAuthoritby(unsigned char* cAuthority, unsigned char cAuthorityCount, int iField)
{
	if (NULL == cAuthority || 0 == cAuthorityCount)
	{
		return;
	}

	if (iField == 0)
		iField = 0xEFFFFFFF;
	else if (iField < 0)
		iField *= -1;

	unsigned char iCount=0;
	string str = "";
	char ch[60];
	for (; iCount<cAuthorityCount; iCount++)
	{
		if (iCount != 0 && iCount % iField == 0)
		{
			cout<<str<<endl;
			str = "";
		}
// 		if (iCount != 0 && iCount % iField != 0)
// 		{
// 			cout<<"\t\t";
// 		}
		
		memset(ch, 0 , sizeof(ch));
		sprintf_s(ch, sizeof(ch), "%u--%s", cAuthority[iCount], UserInfoMgr::GetInstance()->GetDescByOperPer((OperPermission)cAuthority[iCount]).c_str());
		str += StringTool::SetStringFieldWidth(ch, 35) ;
		//printf("%u--%s", cAuthority[iCount], UserInfoMgr::GetInstance()->GetDescByOperPer((OperPermission)cAuthority[iCount]).c_str());
	}
	if (iCount > 0)
	{
		cout<<str<<endl;
	}
}

bool AuthoritySysProc::FindOneInArray(unsigned char* pArray, unsigned char iCount, unsigned char iMaxCount, unsigned char Obj)
{
	bool bRes = false;
	if (!pArray || iCount==0 || iCount>iMaxCount)
	{
		return bRes;
	}

	unsigned char* pArrayTmp = new unsigned char[iCount];
	memcpy(pArrayTmp, pArray, iCount);

	for (unsigned char i=0; i<iCount; i++)
	{
		if (Obj == pArrayTmp[i])
		{
			bRes = true;
			break;
		}
	}

	delete []pArrayTmp;
	return bRes;
}

bool AuthoritySysProc::CheckStringVaildRemoveSpl(string str, string strSpl, string strVaild)
{
	vector<string> vecStr = StringTool::Splite(str, strSpl);
	string strTmp;
	for (unsigned i=0; i<vecStr.size(); i++)
	{
		strTmp += vecStr.at(i);
	}

	return CheckTool::CheckStringByValid(strTmp, strVaild);
}