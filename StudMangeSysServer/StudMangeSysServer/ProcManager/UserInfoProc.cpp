#include "UserInfoProc.h"
#include "StringTool.h"
#include "MysqlMgr.h"
#include "MsgPackageMgr.h"
#include "UserInfoMgr.h"

UserInfoProc::UserInfoProc()
{

}

UserInfoProc::~UserInfoProc()
{

}

void UserInfoProc::AddSingleUserInfoRecvHandle(SOCKET SocketId, void* vpData, unsigned int DataLen)
{
	if ( NULL == vpData)
	{
		printf("%s  ��ϢΪ��\n", __FUNCTION__);
		return;
	}
	if (DataLen != sizeof(CS_MSG_ADD_SINGLE_USERINFO_REQ))
	{
		printf("%s  ����DataLen[%u]���ԣ���ȷ����[%u]\n", __FUNCTION__, DataLen, sizeof(CS_MSG_ADD_SINGLE_USERINFO_REQ));
		return;
	}

	CS_MSG_ADD_SINGLE_USERINFO_REQ* RecvMsg = (CS_MSG_ADD_SINGLE_USERINFO_REQ*)vpData;
	short sMyIdent = UserInfoMgr::GetInstance()->GetIdentBySocketId(SocketId);
	if (!(RecvMsg->sIdent < sMyIdent ||  (sMyIdent==1 && RecvMsg->sIdent == 1 && UserInfoMgr::GetInstance()->IsHaveOneAuthorityBySocketId(SocketId, OPER_PER_ADDSINGLEUSERINFO))))
	{
		SC_MSG_ADD_SINGLE_USERINFO_ACK SendMsg;
		SendMsg.bSucceed = false;
		PackData packData = MsgPackageMgr::Pack(&SendMsg, sizeof(SendMsg), ASSIST_ID_ADD_SINGLE_USERINFO_ACK);
		MsgPackageMgr::Send(SocketId, packData);
		printf("%s  ���û�����߱�ʶ[%d]������Ӹ���߱�ʶ[%u]���û���Ϣ\n", __FUNCTION__, sMyIdent, RecvMsg->sIdent);
		return;
	}

	string strAccount = RecvMsg->cAccount;
	string strName = RecvMsg->cName;
	string strPwd = RecvMsg->cPWD;
	string strSex = StringTool::NumberToStr((int)RecvMsg->sSex);
	string strIdent = StringTool::NumberToStr((int)RecvMsg->sIdent);
	string strMajor = RecvMsg->cMajor;
	string strGrade = RecvMsg->cGrade;
	string strInsertInfo = "''" + strAccount + "'',''" + strName + "'',''" + strPwd + "''," + strSex + "," + strIdent + ",''" + strMajor + "'',''" + strGrade + "''";

	//�������ʹ��Ĭ��Ȩ��
	vector<OperPermission> vecOper;
	UserInfoMgr::GetInstance()->GetDefaultAuthorityByIdent((IdentType)RecvMsg->sIdent, vecOper);
	string strAuthority = StringTool::CombVecToStr(vecOper);

	char strMysql[512];
	memset(strMysql, 0, sizeof(strMysql));
	sprintf_s(strMysql, sizeof(strMysql), "call AddSingleUserInfo('%s', '%s', '%s')", strInsertInfo.c_str(), RecvMsg->cAccount, strAuthority.c_str());

	string strRecord = "~";
	strRecord += RecvMsg->cAccount;

	MysqlMgr::GetInstance()->InputMsgQueue(strMysql, MYSQL_OPER_CALL_PROC, ASSIST_ID_ADD_SINGLE_USERINFO_ACK, SocketId, strRecord);
}

void UserInfoProc::AddBatchUserInfoRecvHandle(SOCKET SocketId, void* vpData, unsigned int DataLen)
{
	if ( NULL == vpData)
	{
		printf("%s  ��ϢΪ��\n", __FUNCTION__);
		return;
	}
	if (DataLen != sizeof(CS_MSG_ADD_BATCH_USERINFO_REQ))
	{
		printf("%s  ����DataLen[%u]���ԣ���ȷ����[%u]\n", __FUNCTION__, DataLen, sizeof(CS_MSG_ADD_BATCH_USERINFO_REQ));
		return;
	}

	CS_MSG_ADD_BATCH_USERINFO_REQ* RecvMsg = (CS_MSG_ADD_BATCH_USERINFO_REQ*)vpData;
	if (RecvMsg->bRecordCount < 1 || RecvMsg->bRecordCount > MAXBATCHREQACKCOUNT)
	{
		printf("%s  �ͻ��˴������ĳɼ���¼������ȷRecvMsg->bRecordCount[%u]\n", __FUNCTION__, (unsigned)RecvMsg->bRecordCount);
		return;
	}
	if (RecvMsg->bRecordNO <= 1)
	{
		UserInfoMgr::GetInstance()->SetStrTmpDataBySocketId(SocketId, "0|0"); //��¼�ɹ��ܴ����͵��γɹ�����
	}
	else
	{
		string strTmpData = UserInfoMgr::GetInstance()->GetStrTmpDataBySocketId(SocketId);
		vector<string> vecStr = StringTool::Splite(strTmpData);
		if (vecStr.size() != 2)
		{
			SC_MSG_ADD_BATCH_USERINFO_ACK SendMsg;
			SendMsg.bSucceed = false;
			PackData packData = MsgPackageMgr::Pack(&SendMsg, sizeof(SendMsg), ASSIST_ID_ADD_BATCH_USERINFO_ACK);
			MsgPackageMgr::Send(SocketId, packData);

			printf("%s  �������ʱ���ݲ���ȷ\n", __FUNCTION__);
			return;
		}
		string newStrTMpData = vecStr.at(0) + "|0";
		UserInfoMgr::GetInstance()->SetStrTmpDataBySocketId(SocketId, newStrTMpData); //��¼�ɹ��ܴ����͵��γɹ�����
	}

	short sMyIdent = UserInfoMgr::GetInstance()->GetIdentBySocketId(SocketId);

	for (unsigned char i=0; i < RecvMsg->bRecordCount; i++)
	{
		if (!(RecvMsg->sIdent[i] < sMyIdent ||  (sMyIdent==1 && RecvMsg->sIdent[i] == 1 && UserInfoMgr::GetInstance()->IsHaveOneAuthorityBySocketId(SocketId, OPER_PER_ADDBATCHUSERINFO))))
		{
			string strRecord = "~";
			strRecord += string(RecvMsg->cAccount[i]) + "~" + StringTool::NumberToStr((int)RecvMsg->bRecordCount) + "~" + StringTool::NumberToStr((int)RecvMsg->bRecordNO) + "~" + StringTool::NumberToStr((int)RecvMsg->bEnd) + "~" + StringTool::NumberToStr((int)((i==RecvMsg->bRecordCount-1) ? 1 : 0));
			MysqlMgr::GetInstance()->InputMsgQueue("", MYSQL_OPER_CALL_PROC, ASSIST_ID_ADD_BATCH_USERINFO_ACK, SocketId, strRecord); //�յ����ݿ���䲻ִ�е�

			printf("%s  ���û�����߱�ʶ[%d]������Ӹ���߱�ʶ[%u]���û���Ϣ\n", __FUNCTION__, sMyIdent, RecvMsg->sIdent[i]);
			continue;
		}

		string strAccount = RecvMsg->cAccount[i];
		string strName = RecvMsg->cName[i];
		string strPwd = RecvMsg->cPWD[i];
		string strSex = StringTool::NumberToStr((int)RecvMsg->sSex[i]);
		string strIdent = StringTool::NumberToStr((int)RecvMsg->sIdent[i]);
		string strMajor = RecvMsg->cMajor[i];
		string strGrade = RecvMsg->cGrade[i];
		string strInsertInfo = "''" + strAccount + "'',''" + strName + "'',''" + strPwd + "''," + strSex + "," + strIdent + ",''" + strMajor + "'',''" + strGrade + "''";

		//�������ʹ��Ĭ��Ȩ��
		vector<OperPermission> vecOper;
		UserInfoMgr::GetInstance()->GetDefaultAuthorityByIdent((IdentType)RecvMsg->sIdent[i], vecOper);
		string strAuthority = StringTool::CombVecToStr(vecOper);

		char strMysql[512];
		memset(strMysql, 0, sizeof(strMysql));
		sprintf_s(strMysql, sizeof(strMysql), "call AddSingleUserInfo('%s', '%s', '%s')", strInsertInfo.c_str(), RecvMsg->cAccount[i], strAuthority.c_str());

		string strRecord = "~";
		strRecord += string(RecvMsg->cAccount[i]) + "~" + StringTool::NumberToStr((int)RecvMsg->bRecordCount) + "~" + StringTool::NumberToStr((int)RecvMsg->bRecordNO) + "~" + StringTool::NumberToStr((int)RecvMsg->bEnd) + "~" + StringTool::NumberToStr((int)((i==RecvMsg->bRecordCount-1) ? 1 : 0));

		MysqlMgr::GetInstance()->InputMsgQueue(strMysql, MYSQL_OPER_CALL_PROC, ASSIST_ID_ADD_BATCH_USERINFO_ACK, SocketId, strRecord);
	}
}

void UserInfoProc::SelectSingleUserInfoRecvHandle(SOCKET SocketId, void* vpData, unsigned int DataLen)
{
	if ( NULL == vpData)
	{
		printf("%s  ��ϢΪ��\n", __FUNCTION__);
		return;
	}
	if (DataLen != sizeof(CS_MSG_SELECT_SINGLE_USERINFO_REQ))
	{
		printf("%s  ����DataLen[%u]���ԣ���ȷ����[%u]\n", __FUNCTION__, DataLen, sizeof(CS_MSG_SELECT_SINGLE_USERINFO_REQ));
		return;
	}

	CS_MSG_SELECT_SINGLE_USERINFO_REQ* RecvMsg = (CS_MSG_SELECT_SINGLE_USERINFO_REQ*)vpData;

	if (RecvMsg->uRecordDataTmp == 1)
	{
		UserInfoMgr::GetInstance()->SetStrTmpDataBySocketId(SocketId, "");
	}
	
	char strMysql[512];
	memset(strMysql, 0, sizeof(strMysql));
	sprintf_s(strMysql, sizeof(strMysql), "select userID, account, password, name, sex, Ident, major, grade from userInfo where account='%s'", RecvMsg->cAccount);

	string strRecord = "~";
	strRecord += string(RecvMsg->cAccount) + "~" + StringTool::NumberToStr((int)RecvMsg->uRecordDataTmp) ;

	MysqlMgr::GetInstance()->InputMsgQueue(strMysql, MYSQL_OPER_SELECT, ASSIST_ID_SELECT_SINGLE_USERINFO_ACK, SocketId, strRecord);
}

void UserInfoProc::UpdateSingleUserInfoRecvHandle(SOCKET SocketId, void* vpData, unsigned int DataLen)
{
	if ( NULL == vpData)
	{
		printf("%s  ��ϢΪ��\n", __FUNCTION__);
		return;
	}
	if (DataLen != sizeof(CS_MSG_UPDATE_SINGLE_USERINFO_REQ))
	{
		printf("%s  ����DataLen[%u]���ԣ���ȷ����[%u]\n", __FUNCTION__, DataLen, sizeof(CS_MSG_UPDATE_SINGLE_USERINFO_REQ));
		return;
	}

	CS_MSG_UPDATE_SINGLE_USERINFO_REQ* RecvMsg = (CS_MSG_UPDATE_SINGLE_USERINFO_REQ*)vpData;
	if (RecvMsg->bUserInfoFieldCount == 0)
	{
		SC_MSG_UPDATE_SINGLE_USERINFO_ACK SendMsg;
		SendMsg.bSucceed = false;
		PackData packData = MsgPackageMgr::Pack(&SendMsg, sizeof(SendMsg), ASSIST_ID_UPDATE_SINGLE_USERINFO_ACK);
		MsgPackageMgr::Send(SocketId, packData);

		printf("%s  RecvMsg->bUserInfoFieldCount�û���Ϣ�ֶ�Ϊ0\n", __FUNCTION__);
		return;
	}

	//�����Լ����û���Ϣ����ͬʱ���ĵ����ݿ���ڴ棻���������û���ֻ����ĵ����ݿ⣬�û���Ҫ���µ�¼����Ч

	//������ݱ�ʶ�ж��Ƿ���Ը��Ķ����û���Ϣ  -- ѧ����ʦ����Ա���е��������û���Ϣ��Ȩ�ޡ�����ֻ��Ҫֻ�ܸ����Լ��ͱ��Լ�����ݱ�ʶ���û����û����Ը����Լ������룬���ǲ��ܸ��ı��˵ġ�������ǹ���Ա���Ը��Ľ�ʦ��ѧ�ĵ�����
	vector<string> vecDataTmp = StringTool::Splite(UserInfoMgr::GetInstance()->GetStrTmpDataBySocketId(SocketId)); //ȡ������ǰ��ѯ����������ʱ����
	UserInfoMgr::GetInstance()->SetStrTmpDataBySocketId(SocketId, "");
	if (vecDataTmp.size() != 2)
	{
		SC_MSG_UPDATE_SINGLE_USERINFO_ACK SendMsg;
		SendMsg.bSucceed = false;
		PackData packData = MsgPackageMgr::Pack(&SendMsg, sizeof(SendMsg), ASSIST_ID_UPDATE_SINGLE_USERINFO_ACK);
		MsgPackageMgr::Send(SocketId, packData);

		printf("%s  ����ǰ��ѯʱ�������ʱ����������ʱ���汻��ѯ��ݱ�ʶ���˺ţ�\n", __FUNCTION__);
		return;
	}
	short sMyIdent = UserInfoMgr::GetInstance()->GetIdentBySocketId(SocketId);
	string strMyAccount = UserInfoMgr::GetInstance()->GetAccountBySocketId(SocketId);
	short sObjIdent = (short)atoi(vecDataTmp.at(0).c_str());
	string strObjAccount = vecDataTmp.at(1);
	bool bCanUpdate[8]; //���û��ɸ��Ķ����û����ֶ�
	memset(bCanUpdate, false, sizeof(bCanUpdate));
	if (!UserInfoMgr::GetInstance()->GetCanUpdateField(sMyIdent, strMyAccount, sObjIdent, strObjAccount, bCanUpdate, sizeof(bCanUpdate)/sizeof(bool))) //�ɸ��ĵ��û���Ϣ�ֶ�
	{
		SC_MSG_UPDATE_SINGLE_USERINFO_ACK SendMsg;
		SendMsg.bSucceed = false;
		PackData packData = MsgPackageMgr::Pack(&SendMsg, sizeof(SendMsg), ASSIST_ID_UPDATE_SINGLE_USERINFO_ACK);
		MsgPackageMgr::Send(SocketId, packData);

		printf("%s  �û���[%s]��ݱ�ʶ[%d]���²�����ݱ�ʶ[%d]�ĵ����û���Ϣ\n", __FUNCTION__, UserInfoMgr::GetInstance()->GetAccountBySocketId(SocketId).c_str(), sMyIdent, sObjIdent);
		return;
	}

	string strUpdateSet;
	int iUpdateMySelfAccount = 0; //�Ƿ�������Լ����˺ţ�����¡����ִ�гɹ���ͬ�������û��ڴ���û�����
	string strUpdateMySelfAccount; //��¼�����Լ����˺�
	for (unsigned char i=0; i<RecvMsg->bUserInfoFieldCount; i++)
	{
		if (!strUpdateSet.empty())
		{
			strUpdateSet += ",";
		}
		if ((RecvMsg->sUserInfoField[i] == 4 || (RecvMsg->sUserInfoField[i] == 5 && sMyIdent == 3 && string(RecvMsg->cUserInfoValue[i]) != "3")) && bCanUpdate[RecvMsg->sUserInfoField[i]]) //�Ա����ݱ�ʶ --ֻ�й���Ա�Ż��漰����ݱ�ʶ������ѧ����ʦ��
		{
			strUpdateSet += UserInfoMgr::GetInstance()->GetEnglishUserInfoFieldByField(RecvMsg->sUserInfoField[i]);
			strUpdateSet += string("=") + RecvMsg->cUserInfoValue[i];
		}
		else if (((RecvMsg->sUserInfoField[i] > 0 && RecvMsg->sUserInfoField[i] < 4) || (RecvMsg->sUserInfoField[i] > 5  && RecvMsg->sUserInfoField[i] < 8)) && bCanUpdate[RecvMsg->sUserInfoField[i]])
		{
			strUpdateSet += UserInfoMgr::GetInstance()->GetEnglishUserInfoFieldByField(RecvMsg->sUserInfoField[i]);
			strUpdateSet += string("='") + RecvMsg->cUserInfoValue[i] + "'";

			if (RecvMsg->sUserInfoField[i]==1 && sMyIdent==sObjIdent && strMyAccount==strObjAccount)
			{
				iUpdateMySelfAccount = 1;
				strUpdateMySelfAccount = RecvMsg->cUserInfoValue[i];
			}
		}
		else
		{
			SC_MSG_UPDATE_SINGLE_USERINFO_ACK SendMsg;
			SendMsg.bSucceed = false;
			PackData packData = MsgPackageMgr::Pack(&SendMsg, sizeof(SendMsg), ASSIST_ID_UPDATE_SINGLE_USERINFO_ACK);
			MsgPackageMgr::Send(SocketId, packData);

			printf("%s  ���ܸ��ĸ��û���Ϣ�ֶ�id[%u]��������ֵΪ[%s]\n", __FUNCTION__, RecvMsg->sUserInfoField[i], RecvMsg->cUserInfoValue[i]);
			return;
		}
	}

	if (strUpdateSet.empty())
	{
		SC_MSG_UPDATE_SINGLE_USERINFO_ACK SendMsg;
		SendMsg.bSucceed = false;
		PackData packData = MsgPackageMgr::Pack(&SendMsg, sizeof(SendMsg), ASSIST_ID_UPDATE_SINGLE_USERINFO_ACK);
		MsgPackageMgr::Send(SocketId, packData);

		printf("%s  û�пɸ��µĶ���\n", __FUNCTION__);
		return;
	}

	char strMysql[512];
	memset(strMysql, 0, sizeof(strMysql));
	sprintf_s(strMysql, sizeof(strMysql), "update userInfo set %s where account='%s'", strUpdateSet.c_str(), RecvMsg->cAccount);

	string strRecord = "~";
	strRecord += StringTool::NumberToStr((int)RecvMsg->sType) + "~" + RecvMsg->cAccount + "~" + StringTool::NumberToStr(iUpdateMySelfAccount)+"|"+strUpdateMySelfAccount;

	MysqlMgr::GetInstance()->InputMsgQueue(strMysql, MYSQL_OPER_UPDATE, ASSIST_ID_UPDATE_SINGLE_USERINFO_ACK, SocketId, strRecord);
}

void UserInfoProc::AddSingleUserInfoReplyHandle(SOCKET SocketId, MYSQL_RES *MysqlRes, string strRecord)
{
	SC_MSG_ADD_SINGLE_USERINFO_ACK SendMsg;
	SendMsg.bSucceed = true;
	vector<string> vStrRecord= StringTool::Splite(strRecord, "~");
	do 
	{
		if (vStrRecord.size() != 2)
		{
			printf("%s  ����������[%u] ��¼�������������д�strRecord[%s]\n", __FUNCTION__, vStrRecord.size(), strRecord.c_str());
			SendMsg.bSucceed = false;
			break;
		}
		if ((int)atoi(vStrRecord.at(0).c_str()) != 0)
		{
			printf("%s  ���ݿ��������\n", __FUNCTION__);
			SendMsg.bSucceed = false;
			break;
		}

		strcpy_s(SendMsg.cAccount, sizeof(SendMsg.cAccount), vStrRecord.at(1).c_str());
	} while(false);

	PackData packData = MsgPackageMgr::Pack(&SendMsg, sizeof(SendMsg), ASSIST_ID_ADD_SINGLE_USERINFO_ACK);
	MsgPackageMgr::Send(SocketId, packData);
}

void UserInfoProc::AddBatchUserInfoReplyHandle(SOCKET SocketId, MYSQL_RES *MysqlRes, string strRecord)
{
	unsigned char bSendFlag = 0;
	vector<string> vStrRecord= StringTool::Splite(strRecord, "~");

	string strTmpData = UserInfoMgr::GetInstance()->GetStrTmpDataBySocketId(SocketId);
	vector<string> vecStrTmpData = StringTool::Splite(strTmpData);
	do 
	{
		if (vStrRecord.size() != 6)
		{
			printf("%s  ����������[%u] ��¼�������������д�strRecord[%s]\n", __FUNCTION__, vStrRecord.size(), strRecord.c_str());
			bSendFlag = 2;
			break;
		}
		if ((int)atoi(vStrRecord.at(0).c_str()) != 0) //����sqlִ��ʧ�ܣ��������Լ���������������Ϣ��¼
		{
			printf("%s  ���ݿ��������\n", __FUNCTION__);
			bSendFlag = 1;
			break;
		}
		if (vecStrTmpData.size() != 2)
		{
			bSendFlag = 2;
			//UserInfoMgr::GetInstance()->SetStrTmpDataBySocketId(SocketId,""); //�����ʱ����
			printf("%s  �������ʱ���ݲ���ȷ\n", __FUNCTION__);
			break;
		}
		
		vecStrTmpData.at(0) = StringTool::NumberToStr(((int)atoi(vecStrTmpData.at(0).c_str()))+1);
		vecStrTmpData.at(1) = StringTool::NumberToStr(((int)atoi(vecStrTmpData.at(1).c_str()))+1);
		//��¼�ɹ���
		string newStrTmpData = vecStrTmpData.at(0) + "|" + vecStrTmpData.at(1);
		UserInfoMgr::GetInstance()->SetStrTmpDataBySocketId(SocketId, newStrTmpData); //��¼�ɹ��ܴ����͵��γɹ�����


		if (vStrRecord.at(5) != "1") 
		{
			printf("%s  �������������û���Ϣδ��ϣ���������\n", __FUNCTION__);
			bSendFlag = 0;
		}
	} while(false);

	if ((vStrRecord.size() == 6 && vStrRecord.at(5) == "1") || bSendFlag==2)
	{
		SC_MSG_ADD_BATCH_USERINFO_ACK SendMsg;
		SendMsg.bSucceed = true;
		if (bSendFlag == 2 || (vecStrTmpData.size()==2 && vecStrTmpData.at(1)=="0")) //�����ش�����ߵ�ǰ�����һ����û�гɹ����ͷ���ʧ��
		{
			SendMsg.bSucceed = false;
			UserInfoMgr::GetInstance()->SetStrTmpDataBySocketId(SocketId,""); //�����ʱ����
		}

		if (vStrRecord.size() == 6 && vStrRecord.at(5) == "1" && vecStrTmpData.size()==2)//����������
		{
			printf("%s  �������������û���Ϣ��ϣ������Ҫ���%d�û���Ϣ��¼���ɹ������%s��\n", __FUNCTION__, atoi(vStrRecord.at(2).c_str()), vecStrTmpData.at(1).c_str());
		}
		if (vStrRecord.size() == 6 && vStrRecord.at(4) == "1" && vecStrTmpData.size()==2) //���������û���Ϣ���
		{
			printf("%s  �û�[%s]����������%d���û���Ϣ��¼��ʵ�ʳɹ�������%s����¼\n", __FUNCTION__, UserInfoMgr::GetInstance()->GetAccountBySocketId(SocketId).c_str(), (int)(atoi(vStrRecord.at(3).c_str())-1)*MAXBATCHREQACKCOUNT+atoi(vStrRecord.at(2).c_str()), vecStrTmpData.at(0).c_str());
			UserInfoMgr::GetInstance()->SetStrTmpDataBySocketId(SocketId,""); //�����ʱ����
		}

		if (vecStrTmpData.size()==2)
		{
			SendMsg.bSucceedRecordCount = (unsigned char)atoi(vecStrTmpData.at(1).c_str());
		}

		PackData packData = MsgPackageMgr::Pack(&SendMsg, sizeof(SendMsg), ASSIST_ID_ADD_BATCH_USERINFO_ACK);
		MsgPackageMgr::Send(SocketId, packData);
	}
}

void UserInfoProc::SelectSingleUserInfoReplyHandle(SOCKET SocketId, MYSQL_RES *MysqlRes, string strRecord)
{
	SC_MSG_SELECT_SINGLE_USERINFO_ACK SendMsg;
	SendMsg.bResCode = 0;
	do 
	{
		vector<string> vStrRecord= StringTool::Splite(strRecord, "~");
		if (vStrRecord.size() != 3)
		{
			printf("%s  ����������[%u] ��¼�������������д�strRecord[%s]\n", __FUNCTION__, vStrRecord.size(), strRecord.c_str());
			SendMsg.bResCode = 4;
			break;
		}
		if ((int)atoi(vStrRecord.at(0).c_str()) != 0)
		{
			printf("%s  ���ݿ��������\n", __FUNCTION__);
			SendMsg.bResCode = 1;
			break;
		}

		unsigned rowsNum = (unsigned)mysql_num_rows(MysqlRes);
		if (1 == rowsNum) 
		{
			MYSQL_ROW sql_row;
			MYSQL_FIELD *fd = NULL;
			char chTmp[52];
			int j = mysql_num_fields(MysqlRes);
			sql_row=mysql_fetch_row(MysqlRes);
			if (j == 8)
			{
				vector<string> vecStrField;
				for(int i=0; fd=mysql_fetch_field(MysqlRes);i++)
				{
					memset(chTmp, 0, sizeof(chTmp));
					strcpy_s(chTmp, sizeof(chTmp), fd->name);
					vecStrField.push_back(chTmp);
				}
				if (vecStrField.size() != j)
				{
					printf("%s  ���ؽ���ֶδ�����ֶ���������\n", __FUNCTION__);
					SendMsg.bResCode = 4;
					break;
				}

				for (unsigned i=0; i<vecStrField.size(); i++)
				{
					if (StringTool::ToLowercase(vecStrField.at(i)) == StringTool::ToLowercase(string("userID")))
					{
						if (sql_row[i])
						{
							SendMsg.uUserID = (unsigned)atoi(sql_row[i]);
						}
					}
					else if (StringTool::ToLowercase(vecStrField.at(i)) == StringTool::ToLowercase(string("account")))
					{
						if (sql_row[i])
						{
							strcpy_s(SendMsg.cAccount, sizeof(SendMsg.cAccount), sql_row[i]);
						}
						else
						{
							strcpy_s(SendMsg.cAccount, sizeof(SendMsg.cAccount), "NULL");
						}
					}
					else if (StringTool::ToLowercase(vecStrField.at(i)) == StringTool::ToLowercase(string("password")))
					{
						if (sql_row[i])
						{
							strcpy_s(SendMsg.cPWD, sizeof(SendMsg.cPWD), sql_row[i]);
						}
						else
						{
							strcpy_s(SendMsg.cPWD, sizeof(SendMsg.cPWD), "NULL");
						}
					}
					else if (StringTool::ToLowercase(vecStrField.at(i)) == StringTool::ToLowercase(string("name")))
					{
						if (sql_row[i])
						{
							strcpy_s(SendMsg.cName, sizeof(SendMsg.cName), sql_row[i]);
						}
						else
						{
							strcpy_s(SendMsg.cName, sizeof(SendMsg.cName), "NULL");
						}
					}
					else if (StringTool::ToLowercase(vecStrField.at(i)) == StringTool::ToLowercase(string("sex")))
					{
						if (sql_row[i])
						{
							SendMsg.sSex = (unsigned char)atoi(sql_row[i]);
						}
					}
					else if (StringTool::ToLowercase(vecStrField.at(i)) == StringTool::ToLowercase(string("Ident")))
					{
						if (sql_row[i])
						{
							SendMsg.sIdent = (unsigned char)atoi(sql_row[i]);
						}
					}
					else if (StringTool::ToLowercase(vecStrField.at(i)) == StringTool::ToLowercase(string("major")))
					{
						if (sql_row[i])
						{
							strcpy_s(SendMsg.cMajor, sizeof(SendMsg.cMajor), sql_row[i]);
						}
						else
						{
							strcpy_s(SendMsg.cMajor, sizeof(SendMsg.cMajor), "NULL");
						}
					}
					else if (StringTool::ToLowercase(vecStrField.at(i)) == StringTool::ToLowercase(string("grade")))
					{
						if (sql_row[i])
						{
							strcpy_s(SendMsg.cGrade, sizeof(SendMsg.cGrade), sql_row[i]);
						}
						else
						{
							strcpy_s(SendMsg.cGrade, sizeof(SendMsg.cGrade), "NULL");
						}
					}
					else
					{
						printf("%s  û�е��ֶ���[%s]\n", __FUNCTION__, vecStrField.at(i).c_str());
						memset(&SendMsg, 0, sizeof(SendMsg));
						SendMsg.bResCode = 4;
						break;
					}
				}
				
				//������߱�ʶ�ж��Ƿ�ɲ� -- ÿ����ݶ��е�����ѯȨ�ޣ�����ֻ�ܲ�ѯ�Լ��ͱ��Լ���ݱ�ʶ�͵��û���������ǹ����ߣ����Բ�ѯѧ���ͽ�ʦ�����룬��ʦ���ܲ�ѯѧ�������룬�Լ�Ҳ���ܲ�ѯ�Լ�������
				short sMyIdent = UserInfoMgr::GetInstance()->GetIdentBySocketId(SocketId);
				if (sMyIdent > SendMsg.sIdent)
				{
					if (sMyIdent != 3)
					{
						memset(SendMsg.cPWD, 0, sizeof(SendMsg.cPWD));
						strcpy_s(SendMsg.cPWD, sizeof(SendMsg.cPWD), "******");
					}

					if (vStrRecord.at(2) == "1")
					{
						UserInfoMgr::GetInstance()->SetStrTmpDataBySocketId(SocketId, StringTool::NumberToStr((int)SendMsg.sIdent) + "|" + SendMsg.cAccount); //���汻��ѯ�û���ݱ�ʶ���˺ŵ�������ʱ����
					}
				}
				else if (sMyIdent == SendMsg.sIdent && UserInfoMgr::GetInstance()->GetAccountBySocketId(SocketId) == SendMsg.cAccount) //��ѯ�û��Լ�����Ϣ
				{
					memset(SendMsg.cPWD, 0, sizeof(SendMsg.cPWD));
					strcpy_s(SendMsg.cPWD, sizeof(SendMsg.cPWD), "******");

					if (vStrRecord.at(2) == "1")
					{
						UserInfoMgr::GetInstance()->SetStrTmpDataBySocketId(SocketId, StringTool::NumberToStr((int)SendMsg.sIdent) + "|" + SendMsg.cAccount); //���汻��ѯ�û���ݱ�ʶ���˺ŵ�������ʱ����
					}
				}
				else
				{
					printf("%s  �û���[%s]��ݱ�ʶ[%d]��ѯ������ݱ�ʶ[%d]�ĵ����û���Ϣ\n", __FUNCTION__, UserInfoMgr::GetInstance()->GetAccountBySocketId(SocketId).c_str(), sMyIdent, SendMsg.sIdent);
					SendMsg.bResCode = 3;
					break;
				}
			}
			else
			{
				printf("%s  ���ݿⷵ�ؽ��������������\n", __FUNCTION__);
				SendMsg.bResCode = 4;
				break;
			}
		}
		else
		{
			printf("%s  ���ݿⷵ�����ݼ�¼����ȷ���ּ�¼��Ϊ[%u]��0��ʾ���ݿ�û�ж�Ӧ���ݼ�¼������1��ʾ���ݿ��¼��Ϣ�쳣��\n", __FUNCTION__,rowsNum);
			SendMsg.bResCode = 2;
			break;
		}
	} while (false);

	PackData packData = MsgPackageMgr::Pack(&SendMsg, sizeof(SendMsg), ASSIST_ID_SELECT_SINGLE_USERINFO_ACK);
	MsgPackageMgr::Send(SocketId, packData);
}

void UserInfoProc::UpdateSingleUserInfoReplyHandle(SOCKET SocketId, MYSQL_RES *MysqlRes, string strRecord)
{
	SC_MSG_UPDATE_SINGLE_USERINFO_ACK SendMsg;
	SendMsg.bSucceed = true;
	do 
	{
		vector<string> vStrRecord= StringTool::Splite(strRecord, "~");
		if (vStrRecord.size() != 4)
		{
			printf("%s  ����������[%u] ��¼�������������д�strRecord[%s]\n", __FUNCTION__, vStrRecord.size(), strRecord.c_str());
			SendMsg.bSucceed = false;
			break;
		}
		if ((int)atoi(vStrRecord.at(0).c_str()) != 0)
		{
			printf("%s  ���ݿ��������\n", __FUNCTION__);
			SendMsg.bSucceed = false;
			break;
		}

		//�����Լ����û���������������ݿ�ģ���Ҫ�����ڴ��
		vector<string> vecStrUpdateMySelfAccount = StringTool::Splite(vStrRecord.at(3));
		if (vecStrUpdateMySelfAccount.size()==2 && vecStrUpdateMySelfAccount.at(0) == "1")
		{
			UserInfoMgr::GetInstance()->SetNewAccountByAccount(vStrRecord.at(2).c_str(), vecStrUpdateMySelfAccount.at(1));
		}

		SendMsg.sType = (short)atoi(vStrRecord.at(1).c_str());
		strcpy_s(SendMsg.cAccount, sizeof(SendMsg.cAccount), vStrRecord.at(2).c_str());
	} while (false);

	PackData packData = MsgPackageMgr::Pack(&SendMsg, sizeof(SendMsg), ASSIST_ID_UPDATE_SINGLE_USERINFO_ACK);
	MsgPackageMgr::Send(SocketId, packData);
}
