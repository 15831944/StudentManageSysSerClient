#include "ScoreProc.h"
#include "SubjectsMgr.h"
#include "CheckTool.h"
#include "MsgPackageMgr.h"
#include "MysqlMgr.h"
#include "UserInfoMgr.h"
#include "SubjectsMgr.h"
#include "AuthorityMgr.h"

ScoreProc::ScoreProc()
{

}

ScoreProc::~ScoreProc()
{

}

void ScoreProc::CheckCanAlterSubjectRecvHandle(SOCKET SocketId, void* vpData, unsigned int DataLen)
{
	if ( NULL == vpData)
	{
		printf("%s  ��ϢΪ��\n", __FUNCTION__);
		return;
	}
	if (DataLen != sizeof(CS_MSG_GET_SUBJECTS_REQ))
	{
		printf("%s  ����DataLen[%u]���ԣ���ȷ����[%u]\n", __FUNCTION__, DataLen, sizeof(CS_MSG_GET_SUBJECTS_REQ));
		return;
	}

	CS_MSG_GET_SUBJECTS_REQ* RecvMsg = (CS_MSG_GET_SUBJECTS_REQ*)vpData;

	CS_MSG_GET_SUBJECTS_ACK SendMsg;
	SendMsg.bSucceed = true;

	vector<SubjectsType> vsSubjectsTmp;
	if (RecvMsg->sGetType == 1)//���ӿ�Ŀǰ��ȡ 
	{
		SendMsg.sGetType = 1;
		vsSubjectsTmp = SubjectsMgr::GetInstance()->GetVNotExistSubjects();
	}
	else if (RecvMsg->sGetType == 2)// ɾ����Ŀǰ��ȡ
	{
		SendMsg.sGetType = 2;
		vsSubjectsTmp = SubjectsMgr::GetInstance()->GetVExistSubjects();
	}
	else
	{
		SendMsg.bSucceed = false;
		printf("%s  RecvMsg->sGetType=[%d] error\n", __FUNCTION__, (int)RecvMsg->sGetType);
	}
	
	vector<int> viSubjectsTmp;
	for (vector<SubjectsType>::iterator iter = vsSubjectsTmp.begin(); iter != vsSubjectsTmp.end(); iter++)
	{
		viSubjectsTmp.push_back((int)(*iter));
	}

	string strSubjectsTmp = CheckTool::CombVecToStr(viSubjectsTmp);
	strcpy_s(SendMsg.cCanAlterSubjects, sizeof(SendMsg.cCanAlterSubjects), strSubjectsTmp.c_str());

	PackData packData = MsgPackageMgr::Pack(&SendMsg, sizeof(SendMsg), ASSIST_ID_GET_SUBJECTS_ACK);
	MsgPackageMgr::Send(SocketId, packData);
}

void ScoreProc::AlterSubjectRecvHandle(SOCKET SocketId, void* vpData, unsigned int DataLen)
{
	if ( NULL == vpData)
	{
		printf("%s  ��ϢΪ��\n", __FUNCTION__);
		return;
	}
	if (DataLen != sizeof(CS_MSG_ALTER_SUBJECTS_REQ))
	{
		printf("%s  ����DataLen[%u]���ԣ���ȷ����[%u]\n", __FUNCTION__, DataLen, sizeof(CS_MSG_ALTER_SUBJECTS_REQ));
		return;
	}

	CS_MSG_ALTER_SUBJECTS_REQ* RecvMsg = (CS_MSG_ALTER_SUBJECTS_REQ*)vpData;
	
	string sEnglishName = SubjectsMgr::GetInstance()->GetEnglishNameByType((SubjectsType)RecvMsg->sSubjectId);

	string strSqlTmp = "";
	if (RecvMsg->sGetType == 1)
	{
		strSqlTmp += " add " + sEnglishName + " tinyint"; 
	}
	else if (RecvMsg->sGetType == 2)
	{
		strSqlTmp += " drop " + sEnglishName;
	}
	else
	{
		CS_MSG_ALTER_SUBJECTS_ACK SendMsg;
		SendMsg.bSucceed = false;
		PackData packData = MsgPackageMgr::Pack(&SendMsg, sizeof(SendMsg), ASSIST_ID_ALTER_SUBJECTS_ACK);
		MsgPackageMgr::Send(SocketId, packData);
		return;
	}

	char strMysql[512];
	memset(strMysql, 0, sizeof(strMysql));
	sprintf_s(strMysql, sizeof(strMysql), "alter table studScore %s", strSqlTmp.c_str());

	string  strRecord = CheckTool::NumberToStr((int)RecvMsg->sGetType) + "|" + CheckTool::NumberToStr((int)RecvMsg->sSubjectId);

	MysqlMgr::GetInstance()->InputMsgQueue(strMysql, MYSQL_OPER_ALTER, ASSIST_ID_ALTER_SUBJECTS_ACK, SocketId, strRecord);
}

void ScoreProc::AddSingleScoreRecvHandle(SOCKET SocketId, void* vpData, unsigned int DataLen)
{
	if ( NULL == vpData)
	{
		printf("%s  ��ϢΪ��\n", __FUNCTION__);
		return;
	}
	if (DataLen != sizeof(CS_MSG_ADD_SINGLE_SCORE_REQ))
	{
		printf("%s  ����DataLen[%u]���ԣ���ȷ����[%u]\n", __FUNCTION__, DataLen, sizeof(CS_MSG_ADD_SINGLE_SCORE_REQ));
		return;
	}

	CS_MSG_ADD_SINGLE_SCORE_REQ* RecvMsg = (CS_MSG_ADD_SINGLE_SCORE_REQ*)vpData;
	//�Ȳ�ѯ�����ӷ������û��Ƿ����

	char strMysql[512];
	memset(strMysql, 0, sizeof(strMysql));
	sprintf_s(strMysql, sizeof(strMysql), "select userID from userInfo where account='%s'", RecvMsg->cAccount);
	
	string strRecord = "~";
	strRecord += CheckTool::NumberToStr(RecvMsg->sType) + "~" + RecvMsg->cAccount + "~" + RecvMsg->sSubjectId + "~" + RecvMsg->sScore;
	UserInfoMgr::GetInstance()->SetRegNeedCountBySocketId(SocketId, 101); //���

	MysqlMgr::GetInstance()->InputMsgQueue(strMysql, MYSQL_OPER_SELECT, ASSIST_ID_ADD_SINGLE_SCORE_ACK, SocketId, strRecord);
}

void ScoreProc::AlterSubjectReplyHandle(SOCKET SocketId, MYSQL_RES *MysqlRes, string strRecord)
{
	vector<string> vStrRecord= CheckTool::Splite(strRecord);

	CS_MSG_ALTER_SUBJECTS_ACK SendMsg;
	SendMsg.bSucceed = true;
	if (vStrRecord.size() > 2 && (int)atoi(vStrRecord.at(0).c_str()) == 0)
	{
		SendMsg.sGetType = (short)atoi(vStrRecord.at(1).c_str());
		SendMsg.sSubjectId = (short)atoi(vStrRecord.at(2).c_str());
		
		if (SendMsg.sGetType == 1)
		{
			SubjectsMgr::GetInstance()->AddOneExistSubject((SubjectsType)SendMsg.sSubjectId);
		}
		else if (SendMsg.sGetType == 2)
		{
			SubjectsMgr::GetInstance()->DeleteOneExistSubject((SubjectsType)SendMsg.sSubjectId);
		}
		else
		{
			printf("%s  SendMsg.sGetType[%d] error\n", __FUNCTION__, SendMsg.sGetType);
			SendMsg.bSucceed = false;
		}
	}
	else 
	{
		printf("%s  ���ݿ����ִ��ʧ��\n", __FUNCTION__);
		SendMsg.bSucceed = false;
	}

	PackData packData = MsgPackageMgr::Pack(&SendMsg, sizeof(SendMsg), ASSIST_ID_ALTER_SUBJECTS_ACK);
	MsgPackageMgr::Send(SocketId, packData);
}

void ScoreProc::AddSingleScoreReplyHandle(SOCKET SocketId, MYSQL_RES *MysqlRes, string strRecord)
{
	bool bIsFailEnd = false;
	do 
	{
		short sRegNeedCount = UserInfoMgr::GetInstance()->GetRegNeedCountBySocketId(SocketId);
		if (sRegNeedCount == 101)
		{
			vector<string> vStrRecord= CheckTool::Splite(strRecord, "~");
			if (vStrRecord.size() != 5)
			{
				printf("%s  ���ݿ����ִ��ʧ�ܣ�sRegNeedCount[%d]\n", __FUNCTION__,sRegNeedCount);
				bIsFailEnd = true;
				break;
			}

			vector<string> vStrRes = CheckTool::Splite(vStrRecord.at(0), "|");
			vector<string> vStrType = CheckTool::Splite(vStrRecord.at(1), "|");
			vector<string> vStrAccount = CheckTool::Splite(vStrRecord.at(2), "|");
			//vector<string> vStrSubjectId = CheckTool::Splite(vStrRecord.at(3), "|");
			vector<string> vStrScore = CheckTool::Splite(vStrRecord.at(4), "|");

			string strEnglishName = SubjectsMgr::GetInstance()->GetStrEnglishNameByStrType(vStrRecord.at(3), "|");
			vector<string> vStrSubjectEnglishName = CheckTool::Splite(strEnglishName, "|");

			if (vStrRes.size() < 1 || (int)atoi(vStrRes.at(0).c_str()) != 0)
			{
				printf("%s  ���ݿ��������sRegNeedCount[%d]\n", __FUNCTION__,sRegNeedCount);
				bIsFailEnd = true;
				break;
			}

			if (vStrType.size() > 0 && vStrAccount.size() > 0 && vStrSubjectEnglishName.size() > 0 && vStrScore.size() > 0 && vStrSubjectEnglishName.size()==vStrScore.size())
			{
				string strSubjectEnglishName = CheckTool::CombVecToStr(vStrSubjectEnglishName, ",");
				string strScore = CheckTool::CombVecToStr(vStrScore, ",");
				string strUpdateSet = CheckTool::CombToSqlUpdateSetStr(strSubjectEnglishName, strScore, ",");

				unsigned rowsNum = (unsigned)mysql_num_rows(MysqlRes);
				if (1 == rowsNum) //����Ѿ�ע��
				{
					MYSQL_ROW sql_row;
					int j = mysql_num_fields(MysqlRes);
					sql_row=mysql_fetch_row(MysqlRes);
					if (1 == j)
					{
						unsigned userid = (unsigned)atoi(sql_row[0]);

						//����ɼ���ǰ�ȼ��ɼ����Ƿ��Ѿ��ж�Ӧ���id
						char strMysql[512];
						memset(strMysql, 0, sizeof(strMysql));
						sprintf_s(strMysql, sizeof(strMysql), "call AddSingleScoreHaveRegister('%s', %u, '%s', '%s')", strSubjectEnglishName.c_str(), userid, strScore.c_str(), strUpdateSet.c_str());

						UserInfoMgr::GetInstance()->SetRegNeedCountBySocketId(SocketId, 102); //��ǣ�����Ѿ�ע������²���ɼ���
						string strRecordTmp = "";
						strRecordTmp += CheckTool::NumberToStr((int)atoi(vStrType.at(0).c_str())) + "|" + vStrAccount.at(0);

						MysqlMgr::GetInstance()->InputMsgQueue(strMysql, MYSQL_OPER_INSERT, ASSIST_ID_ADD_SINGLE_SCORE_ACK, SocketId, strRecordTmp);
					}
					else
					{
						printf("%s  ���ݿⷵ�ؽ��������������\n", __FUNCTION__);
						bIsFailEnd = true;
						break;
					}
				}
				else if (0 == rowsNum) //���û��ע�ᣬҪ�Ȱ�æע�ᣬע��ɹ����ó�ʼ��Ȩ��
				{
					string strAccount = vStrAccount.at(0);
					string strName = strAccount; //���ƺ����Լ��޸�
					string strPwd = "123456";
					string strSex = "0";
					string strIdent = "1";

					string strInsertInfo = "''" + strAccount + "'',''" + strName + "'',''" + strPwd + "'',''" + strSex + "'',''" + strIdent + "''";

					//�������ʹ��Ĭ��Ȩ��
					vector<OperPermission> vecOper;
					AuthorityMgr::GetDefaultAuthorityByIdent((IdentType)atoi(strIdent.c_str()), vecOper);
					string strAuthority = CheckTool::CombVecToStr(vecOper);

					char strMysql[512];
					memset(strMysql, 0, sizeof(strMysql));
					//sprintf_s(strMysql, sizeof(strMysql), "insert into userInfo(account, name, password, sex, Ident) values('%s', '%s', '%s', %s, %s)", strAccount.c_str(), strName.c_str(), strPwd.c_str(), strSex.c_str(), strIdent.c_str());
					sprintf_s(strMysql, sizeof(strMysql), "call AddSingleScoreNotRegister('%s', '%s','%s', '%s','%s')", strInsertInfo.c_str(), strAccount.c_str(), strAuthority.c_str(), strSubjectEnglishName.c_str(), strScore.c_str());

					UserInfoMgr::GetInstance()->SetRegNeedCountBySocketId(SocketId, 102); //��ǣ�����Ѿ�ע������²���ɼ���

// 					string strRecordTmp = "";
// 					strRecordTmp += CheckTool::NumberToStr((int)atoi(vStrType.at(0).c_str())) + "|" + vStrAccount.at(0) + "|" + strSubjectEnglishName + "|" + strScore;
					string strRecordTmp = "";
					strRecordTmp += CheckTool::NumberToStr((int)atoi(vStrType.at(0).c_str())) + "|" + vStrAccount.at(0);

					MysqlMgr::GetInstance()->InputMsgQueue(strMysql, MYSQL_OPER_INSERT, ASSIST_ID_ADD_SINGLE_SCORE_ACK, SocketId, strRecordTmp);
				}
				else
				{
					printf("%s  ���ݿⷵ�ؽ�����󣬼�¼������\n", __FUNCTION__);
					bIsFailEnd = true;
					break;
				}

			}
			else
			{
				printf("%s  �ͻ��������������\n", __FUNCTION__);
				bIsFailEnd = true;
				break;
			}
		}
		else if (sRegNeedCount == 102)
		{
			vector<string> vStrRecord= CheckTool::Splite(strRecord);
			if (vStrRecord.size() < 3 || (int)atoi(vStrRecord.at(0).c_str()) != 0)
			{
				printf("%s  ���ݿ����ִ��ʧ�ܣ�sRegNeedCount[%d]\n", __FUNCTION__,sRegNeedCount);
				bIsFailEnd = true;
				break;
			}
			
			CS_MSG_ADD_SINGLE_SCORE_ACK SendMsg;
			SendMsg.bSucceed = true;
			SendMsg.sType = (short)atoi(vStrRecord.at(1).c_str());
			strcpy_s(SendMsg.cAccount, sizeof(SendMsg.cAccount), vStrRecord.at(2).c_str());
			
			UserInfoMgr::GetInstance()->SetRegNeedCountBySocketId(SocketId, 0); //��ʼ��

			PackData packData = MsgPackageMgr::Pack(&SendMsg, sizeof(SendMsg), ASSIST_ID_ADD_SINGLE_SCORE_ACK);
			MsgPackageMgr::Send(SocketId, packData);
		}
// 		else if (sRegNeedCount == 103)
// 		{
// 			vector<string> vStrRecord= CheckTool::Splite(strRecord);
// 			if (vStrRecord.size() < 5 || (int)atoi(vStrRecord.at(0).c_str()) != 0)
// 			{
// 				printf("%s  ���ݿ����ִ��ʧ�ܣ�sRegNeedCount[%d]\n", __FUNCTION__,sRegNeedCount);
// 				bIsFailEnd = true;
// 				break;
// 			}
// 
// 			short sType = (short)atoi(vStrRecord.at(1).c_str());
// 			string strAccount = vStrRecord.at(2);
// 			string strSubjectEngName = vStrRecord.at(3);
// 			string strScore = vStrRecord.at(4);
// 
// 			//����ɼ���
// 			char strMysql[512];
// 			memset(strMysql, 0, sizeof(strMysql));
// 			sprintf_s(strMysql, sizeof(strMysql), "insert into studScore(userID, %s) (select userID,%s from userInfo where account='%s')",strSubjectEngName.c_str(), strScore.c_str(), strAccount.c_str());
// 
// 			UserInfoMgr::GetInstance()->SetRegNeedCountBySocketId(SocketId, 102); //��ǣ�����Ѿ�ע������²���ɼ���
// 			string strRecordTmp = "";
// 			strRecordTmp +=  vStrRecord.at(1) + "|" + vStrRecord.at(2) ;
// 
// 			MysqlMgr::GetInstance()->InputMsgQueue(strMysql, MYSQL_OPER_INSERT, ASSIST_ID_ADD_SINGLE_SCORE_ACK, SocketId, strRecordTmp);
// 
// 		}
		else
		{
			printf("%s  sRegNeedCount[%d] error\n", __FUNCTION__,sRegNeedCount);
			bIsFailEnd = true;
			break;
		}
	} while (false);

	if (bIsFailEnd)
	{
		CS_MSG_ADD_SINGLE_SCORE_ACK SendMsg;
		SendMsg.bSucceed = false;

		UserInfoMgr::GetInstance()->SetRegNeedCountBySocketId(SocketId, 0); //��ʼ��

		PackData packData = MsgPackageMgr::Pack(&SendMsg, sizeof(SendMsg), ASSIST_ID_ADD_SINGLE_SCORE_ACK);
		MsgPackageMgr::Send(SocketId, packData);
	}
}