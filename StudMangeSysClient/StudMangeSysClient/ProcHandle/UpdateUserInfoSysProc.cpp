#include "UpdateUserInfoSysProc.h"
#include "ProcMgr.h"
#include "UserInfoMgr.h"
#include "TCPHandle.h"
#include "CheckTool.h"
#include "StringTool.h"

UpdateUserInfoSysProc::UpdateUserInfoSysProc(ProcDef nProcDef) : BaseProc(nProcDef)
{
	initMapChoose();
	m_maxUserInfoFieldID = 0;
	InitUserInfoFieldName();

	m_bUserInfoFieldCount = 0;
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
	__super::StartRecv(vpData, DataLen, iAssistId);

	bool bRes = false;
	switch(iAssistId)
	{
	case ASSIST_ID_SELECT_SINGLE_USERINFO_ACK:
		bRes = GetUserInfoAfterUpdateSingleUserInfoRecvHandle(vpData, DataLen);
		break;
	case ASSIST_ID_UPDATE_SINGLE_USERINFO_ACK:
		bRes = UpdateSingleUserInfoRecvHandle(vpData, DataLen);
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

void UpdateUserInfoSysProc::EndRecv()
{
	
	if (0 == GetIEndFlag())
		return;

	m_bUserInfoFieldCount = 0;
	memset(m_sUserInfoField, 0, sizeof(m_sUserInfoField));
	memset(m_cUserInfoValue, 0, sizeof(m_cUserInfoValue));

	__super::EndRecv();
}

void UpdateUserInfoSysProc::SwitchToOper(OperPermission CurOper)
{
	__super::SwitchToOper(CurOper);


	switch(CurOper)
	{
	case OPER_PER_UPDATESINGLEUSERINFOEBYONE: //�����ֶε��������û���Ϣ
	case OPER_PER_UPDATESINGLEUSERINFOBYMORE: //���̶ֹ��ֶε��������û���Ϣ
		GetUserInfoAfterUpdateSingleUserInfoChooseHandle();
		break;
	default:
		printf("%s û�иò�����\n", __FUNCTION__);
		ProcMgr::GetInstance()->ProcSwitch(GetMyProcDef(), true);  
		break;
	}
}

void UpdateUserInfoSysProc::GetUserInfoAfterUpdateSingleUserInfoChooseHandle()
{
	if (OPER_PER_UPDATESINGLEUSERINFOEBYONE != GetCurOper() && OPER_PER_UPDATESINGLEUSERINFOBYMORE != GetCurOper())
	{
		printf("%s ���ǽ��иò���[%d | %d]����ǰ���еĲ�����[%d] error\n", __FUNCTION__, OPER_PER_UPDATESINGLEUSERINFOEBYONE, OPER_PER_UPDATESINGLEUSERINFOBYMORE, GetCurOper());
		OperInputErrorHandle(true, 1);
		return;
	}

	CS_MSG_SELECT_SINGLE_USERINFO_REQ SendReq;
	SendReq.uRecordDataTmp = 1;
	short sIdent = UserInfoMgr::GetInstance()->GetUserIdent();

	/* ��ݱ�ʶ��Ȩ�����⣺
	* 1.��Щ��������ЩȨ�ޣ�
	* 2.������ݱ�ʶȷ���ɸ��ĵĶ������ͣ���������ݱ�ʾ�������ͣ���һ���ǿɸ����Լ��ͱ��Լ���ݱ�ʶ�͵��û���
	* 3.�ɸ��ĵ���Щ�ֶΣ���Щ�ֶβ��ɸ��ģ�
	*/
	//�����û���ǰ��ݱ�ʶ�õ��ɻ�ȡ�Ĳ�ͬ��ݱ�ʶ���û���Ϣ����ʦ��ѧ�����ܿ����Լ������룻����Ա���Կ�����ʦ��ѧ�������룬���ǲ��ܿ����Լ��ģ���ʦҲ���ܿ���ѧ�������룻����ֻ���ڵ��Ӷ�������ģ���Ϊ��Ҫȷ��һ�飬���ʺϹ̶��ֶ�������ġ�����ɲ��ɿ����ɷ���������δ���
	if (sIdent== 1)//�����ģ�ֻ�ܲ�ѯ�Լ���Ҳֻ�ܸ����Լ���
	{
		string strAccount = UserInfoMgr::GetInstance()->GetUserAccount();
		strcpy_s(SendReq.cAccount, sizeof(SendReq.cAccount), strAccount.c_str());
	}
	else if (sIdent == 2 || sIdent == 3) //��ʦ���Բ�ѯѧ�����Լ��ģ�Ҳ���Ը���ѧ�����Լ��ġ�����Ա���Բ�ѯ��ʦѧ�����Լ��ģ�Ҳ���Ը��Ľ�ʦѧ�����Լ���
	{
		string strInputTmp = "����ѧ��";
		if (sIdent == 3)
		{
			strInputTmp = "����ѧ�����ʦ";
		}
		printf("1-�����Լ� 2-%s\n", strInputTmp.c_str());
		string strChoose;
		cin>>strChoose;
		if (!(CheckTool::CheckStringLen(strChoose, 1) && CheckTool::CheckStringByValid(strChoose, "1|2")))
		{
			OperInputErrorHandle();
			return;
		}
		if (strChoose == "2")
		{
			cout<<"��������Ķ�����û�����"<<endl;
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

void UpdateUserInfoSysProc::UpdateSingleUserInfoByOneFieldChooseHandle(unsigned uUserID, char* pAccount, char* pPwd, char* pName, unsigned char uSex, unsigned char uIdent, char* pMajor, char* pGrade)
{
	if (uUserID < 1 || NULL == pAccount || uIdent < 1 || uIdent > 3)
	{
		printf("%s �û�IDΪ0���˺�ΪNULL����߱�ʶuIdent[%u]�쳣\n", __FUNCTION__, uIdent);
		SetIEndFlag(-1);
		return;
	}

	CS_MSG_UPDATE_SINGLE_USERINFO_REQ SendReq;
	SendReq.sType = 1;
	strcpy_s(SendReq.cAccount, sizeof(SendReq.cAccount), pAccount);

	short sMyIdent = UserInfoMgr::GetInstance()->GetUserIdent();
	string strMyAccount = UserInfoMgr::GetInstance()->GetUserAccount();
	if (uIdent > sMyIdent)
	{
		printf("%s �޷�������ݱ�ʶ����ߵ��û�\n", __FUNCTION__);
		SetIEndFlag(-1);
		return;
	}

	bool bIsMe = false;
	if (uIdent == sMyIdent && !strMyAccount.empty() && strMyAccount == string(pAccount))
	{
		bIsMe = true;
	}

	int iChooseFieldID = -1;
	if (sMyIdent == 1) //ѧ�������Լ����û���Ϣ
	{
		if ((iChooseFieldID=GetChooseUserInfoFieldId("1|2|3|4")) < 1) //�ɸ��� 1-account 2-password 3-name 4-sex
		{
			OperInputErrorHandle(false);
			return;
		}
	}
	else if(sMyIdent == 2)//��ʦ���Ķ����û���Ϣ
	{
		if (bIsMe) //��ʦ�����Լ����û���Ϣ
		{
			if ((iChooseFieldID=GetChooseUserInfoFieldId("1|2|3|4")) < 1) //�ɸ��� 1-account 2-password 3-name 4-sex
			{
				OperInputErrorHandle(false);
				return;
			}
		}
		else if (uIdent == 1) //��ʦ����ѧ�����û���Ϣ
		{
			if ((iChooseFieldID=GetChooseUserInfoFieldId("1|3|4|6|7")) < 1) //�ɸ��� 1-account 3-name 4-sex 6-major 7-grade
			{
				OperInputErrorHandle(false);
				return;
			}
		}
		else
		{
			printf("%s ��ʦ�޷�����������ʦ���û���Ϣ\n", __FUNCTION__);
			SetIEndFlag(-1);
			return;
		}
	}
	else if(sMyIdent == 3)//����Ա���Ķ����û���Ϣ
	{
		if (bIsMe) //����Ա�����Լ����û���Ϣ
		{
			if ((iChooseFieldID=GetChooseUserInfoFieldId("1|2|3|4|6|7")) < 1) //�ɸ��� 1-account 2-password 3-name 4-sex 6-major 7-grade
			{
				OperInputErrorHandle(false);
				return;
			}
		}
		else if (uIdent == 1 || uIdent == 2) //����Ա����ѧ�����ʦ���û���Ϣ
		{
			if ((iChooseFieldID=GetChooseUserInfoFieldId("1|2|3|4|5|6|7")) < 1) //�ɸ��� 1-account 2-password 3-name 4-sex 5-Ident 6-major 7-grade
			{
				OperInputErrorHandle(false);
				return;
			}
		}
		else //�������������Աֻ��һ����ֻ��Ϊ�˰�ȫ��������ж�
		{
			printf("%s ����Ա�޷�������������Ա���û���Ϣ\n", __FUNCTION__); 
			SetIEndFlag(-1);
			return;
		}
	}
	else
	{
		printf("%s �û�������߱�ʶsMyIdent[%u]�쳣\n", __FUNCTION__, sMyIdent);
		SetIEndFlag(-1);
		return;
	}

	if (iChooseFieldID < 1)
	{
		printf("%s �ֶ�Id����ȷ\n", __FUNCTION__, sMyIdent);
		SetIEndFlag(-1);
		return;
	}

	//չʾ�ɵ��û���Ϣ
	printf("����%s��%sΪ[%s]\n", bIsMe?"��":"�ö���", GetChineseUserInfoFieldByField(iChooseFieldID).c_str(), GetOldUserInfoFieldValue(pAccount, pPwd, pName, uSex, uIdent, pMajor, pGrade, iChooseFieldID).c_str());


	//�����µ��û���Ϣ
	printf("������%s�µ�%s���Ա�0��1Ů ��ݱ�ʶ��1ѧ��2��ʦ����\n", bIsMe?"��":"����", GetChineseUserInfoFieldByField(iChooseFieldID).c_str());
	string strNewFieldValue;
	cin>>strNewFieldValue;
	if (!(CheckUserInfoFieldValue(strNewFieldValue, iChooseFieldID)))
	{
		OperInputErrorHandle(false);
		return;
	}

	if (bIsMe && IsPwdUserInfoField(iChooseFieldID)) //����Ǹ��ı����Լ������룬��Ҫȷ��һ������
	{
		printf("���ٴ��������룺\n");
		string strNewFieldValueAgain;
		cin>>strNewFieldValueAgain;
		if (!(CheckUserInfoFieldValue(strNewFieldValueAgain, iChooseFieldID) && strNewFieldValue==strNewFieldValueAgain))
		{
			cout<<"�ٴ�������������һ�β���ͬ"<<endl;
			OperInputErrorHandle(false);
			return;
		}
	}

	//������ʱ����
	m_bUserInfoFieldCount = 1;
	m_sUserInfoField[0] = (unsigned char)iChooseFieldID;
	strcpy_s(m_cUserInfoValue[0], sizeof(m_cUserInfoValue[0]), strNewFieldValue.c_str());
	/////////

	SendReq.bUserInfoFieldCount = 1;
	SendReq.sUserInfoField[0] = (unsigned char)iChooseFieldID;
	strcpy_s(SendReq.cUserInfoValue[0], sizeof(SendReq.cUserInfoValue[0]), strNewFieldValue.c_str());
	TCPHandle::GetInstance()->Send(&SendReq, sizeof(SendReq), /*MAIN_ID_LOGINREGISTER,*/ ASSIST_ID_UPDATE_SINGLE_USERINFO_REQ);
}

void UpdateUserInfoSysProc::UpdateSingleUserInfoByFixedFieldChooseHandle(unsigned uUserID, char* pAccount, char* pPwd, char* pName, unsigned char uSex, unsigned char uIdent, char* pMajor, char* pGrade)
{
	if (uUserID < 1 || NULL == pAccount || uIdent < 1 || uIdent > 3)
	{
		printf("%s �û�IDΪ0���˺�ΪNULL����߱�ʶuIdent[%u]�쳣\n", __FUNCTION__, uIdent);
		SetIEndFlag(-1);
		return;
	}

	CS_MSG_UPDATE_SINGLE_USERINFO_REQ SendReq;
	SendReq.sType = 2;
	strcpy_s(SendReq.cAccount, sizeof(SendReq.cAccount), pAccount);

	short sMyIdent = UserInfoMgr::GetInstance()->GetUserIdent();
	string strMyAccount = UserInfoMgr::GetInstance()->GetUserAccount();
	if (uIdent > sMyIdent)
	{
		printf("%s �޷�������ݱ�ʶ����ߵ��û�\n", __FUNCTION__);
		SetIEndFlag(-1);
		return;
	}

	bool bIsMe = false;
	if (uIdent == sMyIdent && !strMyAccount.empty() && strMyAccount == string(pAccount))
	{
		bIsMe = true;
	}

	map<int, string> mapNewAllFieldValue;
	string strFeild;
	if (sMyIdent == 1) //ѧ�������Լ����û���Ϣ
	{
		 strFeild = "1|2|3|4";
		 vector<string> strVecFeild = StringTool::Splite(strFeild);
		 printf("����%s���û���Ϣ��\n", bIsMe?"��":"�ö���");
		 for (unsigned i=0; i<strVecFeild.size(); i++)
		 {
			 if (i > 0)
			 {
				 cout<<"\t";
			 }
			 int iFeildId =  (int)atoi(strVecFeild.at(i).c_str());
			 cout<<GetChineseUserInfoFieldByField(iFeildId)<<"��"<<GetOldUserInfoFieldValue(pAccount, pPwd, pName, uSex, uIdent, pMajor, pGrade, iFeildId)<<"��";
			 if (i == strVecFeild.size()-1)
			 {
				 cout<<endl;
			 }
		 }

		if (!InputAllUserInfoFieldValues(strFeild, mapNewAllFieldValue)) //�ɸ��� 1-account 2-password 3-name 4-sex
		{
			OperInputErrorHandle(false);
			return;
		}

		//printf("\n");
	}
	else if(sMyIdent == 2)//��ʦ���Ķ����û���Ϣ
	{
		if (bIsMe) //��ʦ�����Լ����û���Ϣ
		{
			strFeild = "1|2|3|4";
			vector<string> strVecFeild = StringTool::Splite(strFeild);
			printf("����%s���û���Ϣ��\n", bIsMe?"��":"�ö���");
			for (unsigned i=0; i<strVecFeild.size(); i++)
			 {
				 if (i > 0)
				 {
					 cout<<"\t";
				 }
				 int iFeildId =  (int)atoi(strVecFeild.at(i).c_str());
				 cout<<GetChineseUserInfoFieldByField(iFeildId)<<"��"<<GetOldUserInfoFieldValue(pAccount, pPwd, pName, uSex, uIdent, pMajor, pGrade, iFeildId)<<"��";
				 if (i == strVecFeild.size()-1)
				 {
					 cout<<endl;
				 }
			 }

			if (!InputAllUserInfoFieldValues(strFeild, mapNewAllFieldValue)) //�ɸ��� 1-account 2-password 3-name 4-sex
			{
				OperInputErrorHandle(false);
				return;
			}
		}
		else if (uIdent == 1) //��ʦ����ѧ�����û���Ϣ
		{
			strFeild = "1|3|4|6|7";
			vector<string> strVecFeild = StringTool::Splite(strFeild);
			printf("����%s���û���Ϣ��\n", bIsMe?"��":"�ö���");
			for (unsigned i=0; i<strVecFeild.size(); i++)
			{
				if (i > 0)
				{
					cout<<"\t";
				}
				int iFeildId =  (int)atoi(strVecFeild.at(i).c_str());
				cout<<GetChineseUserInfoFieldByField(iFeildId)<<"��"<<GetOldUserInfoFieldValue(pAccount, pPwd, pName, uSex, uIdent, pMajor, pGrade, iFeildId)<<"��";
				if (i == strVecFeild.size()-1)
			 {
				 cout<<endl;
			 }
			}

			if (!InputAllUserInfoFieldValues(strFeild, mapNewAllFieldValue)) //�ɸ��� 1-account 3-name 4-sex 6-major 7-grade
			{
				OperInputErrorHandle(false);
				return;
			}
		}
		else
		{
			printf("%s ��ʦ�޷�����������ʦ���û���Ϣ\n", __FUNCTION__);
			SetIEndFlag(-1);
			return;
		}
	}
	else if(sMyIdent == 3)//����Ա���Ķ����û���Ϣ
	{
		if (bIsMe) //����Ա�����Լ����û���Ϣ
		{
			strFeild = "1|2|3|4|6|7";
			vector<string> strVecFeild = StringTool::Splite(strFeild);
			printf("����%s���û���Ϣ��\n", bIsMe?"��":"�ö���");
			for (unsigned i=0; i<strVecFeild.size(); i++)
			{
				if (i > 0)
				{
					cout<<"\t";
				}
				int iFeildId =  (int)atoi(strVecFeild.at(i).c_str());
				cout<<GetChineseUserInfoFieldByField(iFeildId)<<"��"<<GetOldUserInfoFieldValue(pAccount, pPwd, pName, uSex, uIdent, pMajor, pGrade, iFeildId)<<"��";
				if (i == strVecFeild.size()-1)
				 {
					 cout<<endl;
				 }
			}

			if (!InputAllUserInfoFieldValues(strFeild, mapNewAllFieldValue)) //�ɸ��� 1-account 2-password 3-name 4-sex 6-major 7-grade
			{
				OperInputErrorHandle(false);
				return;
			}
		}
		else if (uIdent == 1 || uIdent == 2) //����Ա����ѧ�����ʦ���û���Ϣ
		{
			strFeild = "1|2|3|4|5|6|7";
			vector<string> strVecFeild = StringTool::Splite(strFeild);
			printf("����%s���û���Ϣ��\n", bIsMe?"��":"�ö���");
			for (unsigned i=0; i<strVecFeild.size(); i++)
			{
				if (i > 0)
				{
					cout<<"\t";
				}
				int iFeildId =  (int)atoi(strVecFeild.at(i).c_str());
				cout<<GetChineseUserInfoFieldByField(iFeildId)<<"��"<<GetOldUserInfoFieldValue(pAccount, pPwd, pName, uSex, uIdent, pMajor, pGrade, iFeildId)<<"��";
				if (i == strVecFeild.size()-1)
				 {
					 cout<<endl;
				 }
			}


			if (!InputAllUserInfoFieldValues(strFeild, mapNewAllFieldValue)) //�ɸ��� 1-account 2-password 3-name 4-sex 5-Ident 6-major 7-grade
			{
				OperInputErrorHandle(false);
				return;
			}
		}
		else //�������������Աֻ��һ����ֻ��Ϊ�˰�ȫ��������ж�
		{
			printf("%s ����Ա�޷�������������Ա���û���Ϣ\n", __FUNCTION__); 
			SetIEndFlag(-1);
			return;
		}
	}
	else
	{
		printf("%s �û�������߱�ʶsMyIdent[%u]�쳣\n", __FUNCTION__, sMyIdent);
		SetIEndFlag(-1);
		return;
	}

	if (mapNewAllFieldValue.empty())
	{
		printf("%s ����Ϊ��\n", __FUNCTION__);
		SetIEndFlag(-1);
		return;
	}

	m_bUserInfoFieldCount = 0;//������ʱ����
	SendReq.bUserInfoFieldCount = 0;
	for (map<int, string>::iterator iter=mapNewAllFieldValue.begin(); iter!=mapNewAllFieldValue.end(); iter++, SendReq.bUserInfoFieldCount++, m_bUserInfoFieldCount++)
	{
		//������ʱ����
		m_sUserInfoField[m_bUserInfoFieldCount] = iter->first;
		strcpy_s(m_cUserInfoValue[m_bUserInfoFieldCount], sizeof(m_cUserInfoValue[m_bUserInfoFieldCount]), iter->second.c_str());
		////

		SendReq.sUserInfoField[SendReq.bUserInfoFieldCount] = iter->first;
		strcpy_s(SendReq.cUserInfoValue[SendReq.bUserInfoFieldCount], sizeof(SendReq.cUserInfoValue[SendReq.bUserInfoFieldCount]), iter->second.c_str());
	}

	TCPHandle::GetInstance()->Send(&SendReq, sizeof(SendReq), /*MAIN_ID_LOGINREGISTER,*/ ASSIST_ID_UPDATE_SINGLE_USERINFO_REQ);
}

bool UpdateUserInfoSysProc::GetUserInfoAfterUpdateSingleUserInfoRecvHandle(void* vpData, unsigned int DataLen)
{
	if (OPER_PER_UPDATESINGLEUSERINFOEBYONE != GetCurOper() && OPER_PER_UPDATESINGLEUSERINFOBYMORE != GetCurOper())
	{
		printf("%s ���ǽ��иò���[%d | %d]����ǰ���еĲ�����[%d] error\n", __FUNCTION__, OPER_PER_UPDATESINGLEUSERINFOEBYONE, OPER_PER_UPDATESINGLEUSERINFOBYMORE, GetCurOper());
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

	if (OPER_PER_UPDATESINGLEUSERINFOEBYONE == GetCurOper())
	{
		UpdateSingleUserInfoByOneFieldChooseHandle(RecvMSG->uUserID, RecvMSG->cAccount, RecvMSG->cPWD, RecvMSG->cName, RecvMSG->sSex, RecvMSG->sIdent, RecvMSG->cMajor, RecvMSG->cGrade);
	}
	else
	{
		UpdateSingleUserInfoByFixedFieldChooseHandle(RecvMSG->uUserID, RecvMSG->cAccount, RecvMSG->cPWD, RecvMSG->cName, RecvMSG->sSex, RecvMSG->sIdent, RecvMSG->cMajor, RecvMSG->cGrade);
	}

	return true;
}

bool UpdateUserInfoSysProc::UpdateSingleUserInfoRecvHandle(void* vpData, unsigned int DataLen)
{
	if (OPER_PER_UPDATESINGLEUSERINFOEBYONE != GetCurOper() && OPER_PER_UPDATESINGLEUSERINFOBYMORE != GetCurOper())
	{
		printf("%s ���ǽ��иò���[%d | %d]����ǰ���еĲ�����[%d] error\n", __FUNCTION__, OPER_PER_UPDATESINGLEUSERINFOEBYONE, OPER_PER_UPDATESINGLEUSERINFOBYMORE, GetCurOper());
		SetIEndFlag(-1);
		return false;
	}
	if (DataLen != sizeof(SC_MSG_UPDATE_SINGLE_USERINFO_ACK))
	{
		printf("DataLen[%u] error, It should be [%u]\n", DataLen, sizeof(SC_MSG_UPDATE_SINGLE_USERINFO_ACK));
		SetIEndFlag(-1);
		return false;
	}

	SC_MSG_UPDATE_SINGLE_USERINFO_ACK* RecvMSG = (SC_MSG_UPDATE_SINGLE_USERINFO_ACK*) vpData;
	if (RecvMSG->bSucceed)
	{
		string strTmp;
		if (UserInfoMgr::GetInstance()->GetUserAccount()==string(RecvMSG->cAccount))
		{
			strTmp = "�Լ�";

			for (unsigned char i=0; i<m_bUserInfoFieldCount; i++)
			{
				if (m_sUserInfoField[i] == 1)
				{
					UserInfoMgr::GetInstance()->SetUserAccount(m_cUserInfoValue[i]);
				}
				else if (m_sUserInfoField[i] == 3)
				{
					UserInfoMgr::GetInstance()->SetUserName(m_cUserInfoValue[i]);
				}
				else if (m_sUserInfoField[i] == 4)
				{
					UserInfoMgr::GetInstance()->SetUserSex((short)atoi(m_cUserInfoValue[i]));
				}
			}	
		}
		else
		{
			strTmp = string("�˺�Ϊ[") + RecvMSG->cAccount + "]";
		}

		printf("����%s�ĵ���%s�û���Ϣ�ɹ�\n", strTmp.c_str(), RecvMSG->sType==1?"���ֶ�":(RecvMSG->sType==2?"�̶��ֶ�":"NULL"));
		SetIEndFlag(1);
	}
	else
	{
		SetIEndFlag(-1);
		printf("�����û���Ϣʧ��\n");
		return false;
	}
	
	return true;
}

void UpdateUserInfoSysProc::InitUserInfoFieldName()
{
	m_mUserInfoFieldName.insert(pair<int, string>(++m_maxUserInfoFieldID, "account|�˺�"));
	m_mUserInfoFieldName.insert(pair<int, string>(++m_maxUserInfoFieldID, "password|����"));
	m_mUserInfoFieldName.insert(pair<int, string>(++m_maxUserInfoFieldID, "name|����"));
	m_mUserInfoFieldName.insert(pair<int, string>(++m_maxUserInfoFieldID, "sex|�Ա�"));
	m_mUserInfoFieldName.insert(pair<int, string>(++m_maxUserInfoFieldID, "Ident|��ݱ�ʶ"));
	m_mUserInfoFieldName.insert(pair<int, string>(++m_maxUserInfoFieldID, "major|רҵ"));
	m_mUserInfoFieldName.insert(pair<int, string>(++m_maxUserInfoFieldID, "grade|�༶"));
}

string UpdateUserInfoSysProc::GetEnglishUserInfoFieldByField(int iFeildId)
{
	if (iFeildId < 1 || iFeildId > m_maxUserInfoFieldID)
	{
		return "";
	}

	string strFieldName = m_mUserInfoFieldName[iFeildId];
	vector<string> strVec = StringTool::Splite(strFieldName);
	if (strVec.size() != 2)
	{
		return "";
	}

	return strVec.at(0);
}

string UpdateUserInfoSysProc::GetChineseUserInfoFieldByField(int iFeildId)
{
	if (iFeildId < 1 || iFeildId > m_maxUserInfoFieldID)
	{
		return "";
	}

	string strFieldName = m_mUserInfoFieldName[iFeildId];
	vector<string> strVec = StringTool::Splite(strFieldName);
	if (strVec.size() != 2)
	{
		return "";
	}

	return strVec.at(1);
}

bool UpdateUserInfoSysProc::IsPwdUserInfoField(int iField)
{
	return iField==2 ? true : false;
}

int UpdateUserInfoSysProc::GetChooseUserInfoFieldId(string strShowFieldID, int iField)
{
	if (strShowFieldID.empty())
	{
		return -1;
	}

	vector<string> strVecFieldId = StringTool::Splite(strShowFieldID);
	map<unsigned, string> strMapFieldNameChoose; //ѡ��������ֶ�id������ϵ
	for (unsigned i=0; i<strVecFieldId.size(); i++)
	{
		strMapFieldNameChoose.insert(pair<unsigned, string>(i+1, strVecFieldId.at(i)));
	}

	unsigned iCount = 0;
	char chTmp[51];
	for (map<unsigned, string>::iterator iter=strMapFieldNameChoose.begin(); iter!=strMapFieldNameChoose.end(); iter++, iCount++)
	{
		if (iCount != 0 && iCount % iField == 0)
		{
			cout<<endl;
		}

		memset(chTmp, 0, sizeof(chTmp));
		sprintf_s(chTmp, sizeof(chTmp), "%u-%s", iter->first, GetChineseUserInfoFieldByField((int)atoi(iter->second.c_str())).c_str());
		cout<<StringTool::SetStringFieldWidth(chTmp, 16);	
		
	}
	if (iCount > 0)
	{
		cout<<endl;
	}
	
	cout<<"��ѡ������ĵ��û��ֶΣ�"<<endl;
	string strFieldChoose;
	cin>>strFieldChoose;
	if (!(CheckTool::CheckStringLen(strFieldChoose, 1) && CheckTool::CheckStringByValid(strFieldChoose, "0~9") && strMapFieldNameChoose.find((unsigned)atoi(strFieldChoose.c_str()))!=strMapFieldNameChoose.end()))
	{
		return -1;
	}

	return (int)atoi(strMapFieldNameChoose[(unsigned)atoi(strFieldChoose.c_str())].c_str());
}

bool UpdateUserInfoSysProc::InputAllUserInfoFieldValues(string strShowFieldID, map<int, string>& mapNewAllFieldValue, int iField)
{
	mapNewAllFieldValue.clear();
	if (strShowFieldID.empty())
	{
		return false;
	}

	vector<string> strVecFieldId = StringTool::Splite(strShowFieldID);
	map<unsigned, string> strMapFieldNameChoose; //ѡ��������ֶ�id������ϵ
	for (unsigned i=0; i<strVecFieldId.size(); i++)
	{
		strMapFieldNameChoose.insert(pair<unsigned, string>(i+1, strVecFieldId.at(i)));
	}

	unsigned iCount = 0;
	char chTmp[51];
	for (map<unsigned, string>::iterator iter=strMapFieldNameChoose.begin(); iter!=strMapFieldNameChoose.end(); iter++, iCount++)
	{
		if (iCount != 0 && iCount % iField == 0)
		{
			cout<<endl;
		}

		memset(chTmp, 0, sizeof(chTmp));
		sprintf_s(chTmp, sizeof(chTmp), "%u-%s", iter->first, GetChineseUserInfoFieldByField((int)atoi(iter->second.c_str())).c_str());
		cout<<StringTool::SetStringFieldWidth(chTmp, 16);	
	}
	if (iCount > 0)
	{
		cout<<endl;
	}

	printf("���մ����Һʹ��ϵ��µ�˳�������Ӧ�µ��û���Ϣ����ʽΪ XX|XX|XX�� �Ա�0��1Ů ��ݱ�ʶ��1ѧ��2��ʦ����\n");
	string strNewAllFieldValue;
	cin>>strNewAllFieldValue;
	vector<string> vecNewAllFieldValue = StringTool::Splite(strNewAllFieldValue);
	if (vecNewAllFieldValue.size() != strMapFieldNameChoose.size())
	{
		return false;
	}

	for (unsigned j=0; j<vecNewAllFieldValue.size(); j++)
	{
		int iFieldId = (int)atoi(strMapFieldNameChoose[j+1].c_str());
		if (!(CheckUserInfoFieldValue(vecNewAllFieldValue.at(j), iFieldId)))
		{
			mapNewAllFieldValue.clear();
			return false;
		}
		mapNewAllFieldValue.insert(pair<int, string>(iFieldId, vecNewAllFieldValue.at(j)));
	}

	return true;
}

bool UpdateUserInfoSysProc::CheckUserInfoFieldValue(string strFeildValue, int iFieldId)
{
	bool bRes = true;
	if (iFieldId == 1) //1-account
	{
		if (!(CheckTool::CheckStringLen(strFeildValue, 30) && CheckTool::CheckStringByValid(strFeildValue, "A~Z|a~z|0~9")))
		{
			bRes = false;
		}
	}
	else if (iFieldId == 2) //2-password
	{
		if (!(CheckTool::CheckStringLen(strFeildValue, 30) && CheckTool::CheckStringByValid(strFeildValue, "A~Z|a~z|0~9|_|-")))
		{
			bRes = false;
		}
	}
	else if (iFieldId == 3) //3-name
	{
		if (!(CheckTool::CheckStringLen(strFeildValue, 30) && CheckTool::CheckStringByValid(strFeildValue, "")))
		{
			bRes = false;
		}
	}
	else if (iFieldId == 4) //4-sex
	{
		if (!(CheckTool::CheckStringLen(strFeildValue, 1) && CheckTool::CheckStringByValid(strFeildValue, "0|1")))
		{
			bRes = false;
		}
	}
	else if (iFieldId == 5) //5-Ident
	{
		if (!(CheckTool::CheckStringLen(strFeildValue, 1) && CheckTool::CheckStringByValid(strFeildValue, "1|2|3")))
		{
			bRes = false;
		}
	}
	else if (iFieldId == 6) //6-major
	{
		if (!(CheckTool::CheckStringLen(strFeildValue, 40) && CheckTool::CheckStringByValid(strFeildValue, "")))
		{
			bRes = false;
		}	
	}
	else if (iFieldId == 7) //7-grade
	{
		if (!(CheckTool::CheckStringLen(strFeildValue, 30) && CheckTool::CheckStringByValid(strFeildValue, "")))
		{
			bRes = false;
		}	
	}
	else
	{
		bRes = false;
	}

	return bRes;
}

string UpdateUserInfoSysProc::GetOldUserInfoFieldValue(char* pAccount, char* pPwd, char* pName, unsigned char uSex, unsigned char uIdent, char* pMajor, char* pGrade, int iFieldId)
{
	if (iFieldId == 1)
	{
		return NULL==pAccount ? "NULL" : pAccount;
	}
	else if (iFieldId == 2)
	{
		return NULL==pPwd ? "NULL" : pPwd;
	}
	else if (iFieldId == 3)
	{
		return NULL==pName ? "NULL" : pName;
	}
	else if (iFieldId == 4)
	{
		return 0==uSex ? "0[��]" : (1==uSex ? "1[Ů]" : "NULL");
	}
	else if (iFieldId == 5)
	{
		return 1==uIdent ? "1[ѧ��]" : (2==uIdent ? "2[��ʦ]" : (3==uIdent ? "3[����Ա]" : "NULL"));
	}
	else if (iFieldId == 6)
	{
		return NULL==pMajor ? "NULL" : pMajor;
	}
	else if (iFieldId == 7)
	{
		return NULL==pGrade ? "NULL" : pGrade;
	}
	else
	{
		return "NULL";
	}
}
