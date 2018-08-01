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
			CS_MSG_ADD_BATCH_USERINFO_ACK SendMsg;
			SendMsg.bSucceed = false;
			PackData packData = MsgPackageMgr::Pack(&SendMsg, sizeof(SendMsg), ASSIST_ID_ADD_BATCH_USERINFO_ACK);
			MsgPackageMgr::Send(SocketId, packData);

			printf("%s  �������ʱ���ݲ���ȷ\n", __FUNCTION__);
			return;
		}
		string newStrTMpData = vecStr.at(0) + "|0";
		UserInfoMgr::GetInstance()->SetStrTmpDataBySocketId(SocketId, newStrTMpData); //��¼�ɹ��ܴ����͵��γɹ�����
	}

	for (unsigned char i=0; i < RecvMsg->bRecordCount; i++)
	{
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

void UserInfoProc::AddSingleUserInfoReplyHandle(SOCKET SocketId, MYSQL_RES *MysqlRes, string strRecord)
{
	CS_MSG_ADD_SINGLE_USERINFO_ACK SendMsg;
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
		CS_MSG_ADD_BATCH_USERINFO_ACK SendMsg;
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