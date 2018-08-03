#include "AuthorityProc.h"
#include "StringTool.h"
#include "MysqlMgr.h"
#include "MsgPackageMgr.h"
#include "UserInfoMgr.h"

AuthorityProc::AuthorityProc()
{

}

AuthorityProc::~AuthorityProc()
{

}

void AuthorityProc::GetAuthoritRecvHandle(SOCKET SocketId, void* vpData, unsigned int DataLen)
{
	if ( NULL == vpData)
	{
		printf("%s  ��ϢΪ��\n", __FUNCTION__);
		return;
	}
	if (DataLen != sizeof(CS_MSG_GET_AUTHORITY_REQ))
	{
		printf("%s  ����DataLen[%u]���ԣ���ȷ����[%u]\n", __FUNCTION__, DataLen, sizeof(CS_MSG_GET_AUTHORITY_REQ));
		return;
	}

	CS_MSG_GET_AUTHORITY_REQ* RecvMsg = (CS_MSG_GET_AUTHORITY_REQ*)vpData;

	char strMysql[512];
	memset(strMysql, 0, sizeof(strMysql));
	sprintf_s(strMysql, sizeof(strMysql), "select Authority from userAuthority a, (select userID from userInfo where account='%s') u where a.userID=u.userID", RecvMsg->cAccount);

	string strRecord = "~";
	strRecord += StringTool::NumberToStr(RecvMsg->sType) + "~" + RecvMsg->cAccount;

	MysqlMgr::GetInstance()->InputMsgQueue(strMysql, MYSQL_OPER_SELECT, ASSIST_ID_GET_AUTHORITY_ACK, SocketId, strRecord);
}

void AuthorityProc::EditAuthoritRecvHandle(SOCKET SocketId, void* vpData, unsigned int DataLen)
{
	if ( NULL == vpData)
	{
		printf("%s  ��ϢΪ��\n", __FUNCTION__);
		return;
	}
	if (DataLen != sizeof(CS_MSG_EDIT_AUTHORITY_REQ))
	{
		printf("%s  ����DataLen[%u]���ԣ���ȷ����[%u]\n", __FUNCTION__, DataLen, sizeof(CS_MSG_EDIT_AUTHORITY_REQ));
		return;
	}

	CS_MSG_EDIT_AUTHORITY_REQ* RecvMsg = (CS_MSG_EDIT_AUTHORITY_REQ*)vpData;
	if (RecvMsg->sType < 1 || RecvMsg->sType > 2 || RecvMsg->cAuthorityCount == 0)
	{
		SC_MSG_EDIT_AUTHORITY_ACK SendMsg;
		SendMsg.bSucceed = false;
		PackData packData = MsgPackageMgr::Pack(&SendMsg, sizeof(SendMsg), ASSIST_ID_EDIT_AUTHORITY_ACK);
		MsgPackageMgr::Send(SocketId, packData);

		printf("%s  �������Ĳ�������ȷRecvMsg->sType[%u], RecvMsg->cAuthorityCount[%u]\n", __FUNCTION__, RecvMsg->sType, RecvMsg->cAuthorityCount);
		return;
	}

	vector<OperPermission> vecOper;
	string strAuthorityTmpData = UserInfoMgr::GetInstance()->GetStrTmpDataBySocketId(SocketId);
	UserInfoMgr::GetInstance()->SetStrTmpDataBySocketId(SocketId, ""); //������ʱ���ݾ������ʱ����
	UserInfoMgr::GetInstance()->GetAuthorityByStrAuthority(strAuthorityTmpData, vecOper, RecvMsg->sType);
	//���һ���Ƿ����ɾ
	unsigned char iCount = 0;
	for (unsigned char i=0; i<RecvMsg->cAuthorityCount; i++)
	{
		for (unsigned j=0; j<vecOper.size(); j++)
		{
			if (RecvMsg->cAuthority[i] == (unsigned char)vecOper.at(j))
			{
				iCount++;
				break;
			}
		}
	}
	if (iCount != RecvMsg->cAuthorityCount)
	{
		SC_MSG_EDIT_AUTHORITY_ACK SendMsg;
		SendMsg.bSucceed = false;
		PackData packData = MsgPackageMgr::Pack(&SendMsg, sizeof(SendMsg), ASSIST_ID_EDIT_AUTHORITY_ACK);
		MsgPackageMgr::Send(SocketId, packData);

		printf("%s  �в���%s�Ĳ���Ȩ��\n", __FUNCTION__, (RecvMsg->sType==1?"ɾ��":"����"));
		return;
	}

	vector<string> vecStrMyAuthority = StringTool::Splite(strAuthorityTmpData);
	vector<OperPermission> vecOperTmp;
	for (unsigned i =0; i<vecStrMyAuthority.size(); i++)
	{
		vecOperTmp.push_back((OperPermission)atoi(vecStrMyAuthority.at(i).c_str()));
	}
	if (RecvMsg->sType == 1) //ɾ��
	{
		for (unsigned i=0; i<RecvMsg->cAuthorityCount; i++)
		{
			for (vector<OperPermission>::iterator iter=vecOperTmp.begin(); iter!=vecOperTmp.end(); iter++)
			{
				if ((OperPermission)RecvMsg->cAuthority[i] == *iter)
				{
					vecOperTmp.erase(iter);
					break;
				}
			}
		}
	}
	else
	{
		for (unsigned i=0; i<RecvMsg->cAuthorityCount; i++)
		{
			vecOperTmp.push_back((OperPermission)RecvMsg->cAuthority[i]);
		}
	}
	string strNewAuthority = StringTool::CombVecToStr(vecOperTmp);

	char strMysql[512];
	memset(strMysql, 0, sizeof(strMysql));
	sprintf_s(strMysql, sizeof(strMysql), "update userAuthority a inner join (select userID from userinfo where account='%s') u on u.userID=a.userID set Authority='%s'", RecvMsg->cAccount, strNewAuthority.c_str());

	string strRecord = "~";
	strRecord += StringTool::NumberToStr(RecvMsg->sType) + "~" + RecvMsg->cAccount + "~" + strNewAuthority;

	MysqlMgr::GetInstance()->InputMsgQueue(strMysql, MYSQL_OPER_UPDATE, ASSIST_ID_EDIT_AUTHORITY_ACK, SocketId, strRecord);
}

void AuthorityProc::GetAuthoritReplyHandle(SOCKET SocketId, MYSQL_RES *MysqlRes, string strRecord)
{
	SC_MSG_GET_AUTHORITY_ACK SendMsg;
	SendMsg.bSucceed = true;
	vector<string> vStrRecord= StringTool::Splite(strRecord, "~");
	do 
	{
		if (vStrRecord.size() != 3)
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

		unsigned rowsNum = (unsigned)mysql_num_rows(MysqlRes);
		if (1 == rowsNum)
		{
			MYSQL_ROW sql_row;
			int j = mysql_num_fields(MysqlRes);
			sql_row=mysql_fetch_row(MysqlRes);
			if (1 == j)
			{
				string strAuthority = sql_row[0];

				SendMsg.sType = (unsigned char)atoi(vStrRecord.at(1).c_str());
				strcpy_s(SendMsg.cAccount, sizeof(SendMsg.cAccount), vStrRecord.at(2).c_str());
				vector<OperPermission> vecOper;
				UserInfoMgr::GetInstance()->GetAuthorityByStrAuthority(strAuthority, vecOper, SendMsg.sType); //����sType��strAuthority��ȡ�����Ӧ��vector
				SendMsg.cAuthorityCount = 0;
				for (; SendMsg.cAuthorityCount<vecOper.size(); SendMsg.cAuthorityCount++)
				{
					SendMsg.cAuthority[SendMsg.cAuthorityCount] = (unsigned char)vecOper.at(SendMsg.cAuthorityCount);
				}

				UserInfoMgr::GetInstance()->SetStrTmpDataBySocketId(SocketId, strAuthority); //�Ѷ����Ȩ�޼�¼����ʱ����
			}
			else
			{
				printf("%s  ���������ֶ�������ȷ[%d]\n", __FUNCTION__, j);
				SendMsg.bSucceed = false;
				break;
			}
		}
		else
		{
			printf("%s  �������ݼ�¼������ȷrowsNum[%u]\n", __FUNCTION__, rowsNum);
			SendMsg.bSucceed = false;
			break;
		}
	} while (false);

	PackData packData = MsgPackageMgr::Pack(&SendMsg, sizeof(SendMsg), ASSIST_ID_GET_AUTHORITY_ACK);
	MsgPackageMgr::Send(SocketId, packData);
}

void AuthorityProc::EditAuthoritReplyHandle(SOCKET SocketId, MYSQL_RES *MysqlRes, string strRecord)
{
	SC_MSG_EDIT_AUTHORITY_ACK SendMsg;
	SendMsg.bSucceed = true;
	vector<string> vStrRecord= StringTool::Splite(strRecord, "~");
	do 
	{
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
		
		SendMsg.sType = (unsigned char)atoi(vStrRecord.at(1).c_str());
		strcpy_s(SendMsg.cAccount, sizeof(SendMsg.cAccount), vStrRecord.at(2).c_str());
		//��������Ѿ���¼����Ҫ�����ڴ��е�����
		UserInfoMgr::GetInstance()->SetAuthorityByAccount(SendMsg.cAccount, vStrRecord.at(3));
	} while (false);

	PackData packData = MsgPackageMgr::Pack(&SendMsg, sizeof(SendMsg), ASSIST_ID_EDIT_AUTHORITY_ACK);
	MsgPackageMgr::Send(SocketId, packData);
}
