#include "UpdateScoreSysProc.h"
#include "TCPHandle.h"
#include "StringTool.h"
#include "CheckTool.h"
#include "ProcMgr.h"
#include "UserInfoMgr.h"

UpdateScoreSysProc::UpdateScoreSysProc(ProcDef nProcDef) : BaseProc(nProcDef)
{
	initMapChoose();
}

UpdateScoreSysProc::~UpdateScoreSysProc()
{

}

bool UpdateScoreSysProc::initMapChoose()
{
	//m_mChoose.insert(pair<int, ChooseData>(GetMaxRealChoose(), ChooseData("�����������ĳɼ�", OPER_PER_UPDATEBATCHSCOREBYONESUBJECT, PROC_DEF_UPDATESCORESYSPROC)));
	//m_mChoose.insert(pair<int, ChooseData>(++GetMaxRealChoose(), ChooseData("�������п�Ŀ�������ĳɼ�", OPER_PER_UPDATEBATCHSCOREBYSUBJECTS, PROC_DEF_UPDATESCORESYSPROC)));
	m_mChoose.insert(pair<int, ChooseData>(GetMaxRealChoose(), ChooseData("���Ƶ������ĳɼ�", OPER_PER_UPDATESINGLESCOREBYONESUBJECT, PROC_DEF_UPDATESCORESYSPROC)));
	m_mChoose.insert(pair<int, ChooseData>(++GetMaxRealChoose(), ChooseData("�������п�Ŀ�������ĳɼ�", OPER_PER_UPDATESINGLESCOREBYSUBJECTS, PROC_DEF_UPDATESCORESYSPROC)));
	m_mChoose.insert(pair<int, ChooseData>(++GetMaxRealChoose(), ChooseData("����", OPER_PER_INVALID, PROC_DEF_SCORESYSPROC)));

	return true;
}

void UpdateScoreSysProc::EndProc()
{
	__super::EndProc();
}

void UpdateScoreSysProc::StartRecv(void* vpData, unsigned int DataLen, /*int iMianId,*/ int iAssistId)
{
	__super::StartRecv(vpData, DataLen, iAssistId);

	bool bRes = false;
	switch(iAssistId)
	{
	case ASSIST_ID_GET_SUBJECTS_ACK:
		bRes = GetSubjectsAfterUpdateSingleScoreRecvHandle(vpData, DataLen);
		break;
	case ASSIST_ID_SELECT_SINGLE_SCORE_ACK:
		bRes = GetSubjectsScoreAfterUpdateSingleScoreRecvHandle(vpData, DataLen);
		break;
	case ASSIST_ID_UPDATE_SINGLE_SCORE_ACK:
		bRes = UpdateSingleScoreRecvHandle(vpData, DataLen);
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

void UpdateScoreSysProc::EndRecv()
{
	__super::EndRecv();
}

void UpdateScoreSysProc::SwitchToOper(OperPermission CurOper)
{
	__super::SwitchToOper(CurOper);


	switch(CurOper)
	{
	case OPER_PER_UPDATESINGLESCOREBYONESUBJECT: //���Ƶ������ĳɼ�  ����ǰ������
	case OPER_PER_UPDATESINGLESCOREBYSUBJECTS: //�������п�Ŀ�������ĳɼ�  ����ǰ������
		GetSubjectsAfterUpdateSingleScoreChooseHandle();
		break;
	default:
		printf("%s û�иò�����\n", __FUNCTION__);
		ProcMgr::GetInstance()->ProcSwitch(GetMyProcDef(), true);  
		break;
	}
}

void UpdateScoreSysProc::GetSubjectsAfterUpdateSingleScoreChooseHandle()
{
	//��ȡ���п�Ŀ
	CS_MSG_GET_SUBJECTS_REQ SendReq;
	SendReq.sGetType = 2;

	TCPHandle::GetInstance()->Send(&SendReq, sizeof(SendReq), /*MAIN_ID_LOGINREGISTER,*/ ASSIST_ID_GET_SUBJECTS_REQ);
}

void UpdateScoreSysProc::GetSubjectsScoreAfterUpdateSingleScoreChooseHandle(char* pStrExistSubjects)
{
	if (NULL == pStrExistSubjects)
	{
		printf("%s ��Ŀ�ֶ�ΪNULL\n", __FUNCTION__);
		SetIEndFlag(-1);
		return;
	}

	CS_MSG_SELECT_SINGLE_SCORE_REQ SendReq;
	SendReq.sType = 2;

	cout<<"��������ķ���������û�����"<<endl;
	string strAccount;
	cin>>strAccount;
	if (!(CheckTool::CheckStringLen(strAccount, 30) && CheckTool::CheckStringByValid(strAccount, "A~Z|a~z|0~9")))
	{
		OperInputErrorHandle(false);
		return;
	}
	strcpy_s(SendReq.cAccount, sizeof(SendReq.cAccount), strAccount.c_str());

	vector<string> vecStrSubjectId = StringTool::Splite(pStrExistSubjects, "|");
	for (unsigned i=0, j=0; i<vecStrSubjectId.size()&&j<MAXSUBJECTSCOUNT; i++)
	{
		unsigned char sId = (unsigned char)atoi(vecStrSubjectId.at(i).c_str());
		if (UserInfoMgr::GetInstance()->CanFindSubjectsType((SubjectsType)sId))
		{	
			SendReq.sSubjectId[j] = sId;
			SendReq.bSubjectCount = ++j;
		}
		else
		{
			printf("%s ��ĿID[%d]���Ϸ�\n", __FUNCTION__,(int)sId);
		}
	}
	
	TCPHandle::GetInstance()->Send(&SendReq, sizeof(SendReq), /*MAIN_ID_LOGINREGISTER,*/ ASSIST_ID_SELECT_SINGLE_SCORE_REQ);
}

void UpdateScoreSysProc::UpdateSingleScoreByOneSubjectChooseHandle(char* pName, char* pAccount, char* pGrade, unsigned char* pSubjectsId, unsigned char* pScore, unsigned char bSubjectCount)
{
	if (NULL == pAccount)
	{
		printf("%s �˺�ΪNULL\n", __FUNCTION__);
		SetIEndFlag(-1);
		return;
	}
	if (NULL == pSubjectsId || 0 == bSubjectCount || NULL == pScore)
	{
		printf("%s ��Ŀid�����ΪNULL���߱������û�û�п�Ŀ�ֶ�\n", __FUNCTION__);
		SetIEndFlag(-1);
		return;
	}
	string strName;
	string strGrade;
	if (NULL == pName)
	{
		strName = "NULL";
	}
	else
	{
		strName = pName;
	}
	if (NULL == pGrade)
	{
		strGrade = "NULL";
	}
	else
	{
		strGrade = pGrade;
	}


	//����ʾ�������û����ڵķ������
	printf("�༶[%s] �˺�[%s] �û�����[%s]���гɼ����������\n", strGrade.c_str(), pAccount, strName.c_str());
	ShowSubjects(pSubjectsId, pScore, bSubjectCount, 3);
	printf("��ѡ����Ҫ���ĵĿ�ĿID��\n");
	string iChooseSubjectsId;
	cin>>iChooseSubjectsId;
	unsigned char bSubjectsId = (unsigned char)atoi(iChooseSubjectsId.c_str());
	if (!CheckTool::CheckStringByValid(iChooseSubjectsId, "0~9") || !FindSubjectsId(pSubjectsId, bSubjectCount, bSubjectsId))
	{
		OperInputErrorHandle(false);
		return;
	}

	printf("������ÿ�Ŀ�µķ�����\n");
	string strScore;
	cin>>strScore;
	if (!(CheckTool::CheckStringLen(strScore, 3) && CheckTool::CheckStringByValid(strScore, "0~9") && (int)atoi(strScore.c_str()) >= 0 && (int)atoi(strScore.c_str()) <= 100))
	{
		OperInputErrorHandle(false);
		return;
	}

	CS_MSG_UPDATE_SINGLE_SCORE_REQ SendReq;
	SendReq.sType = 1;
	SendReq.sSubjectId[0] = bSubjectsId;
	SendReq.bScore[0] = (unsigned char)atoi(strScore.c_str());
	SendReq.bSubjectCount = 1;
	strcpy_s(SendReq.cAccount, sizeof(SendReq.cAccount), pAccount);

	TCPHandle::GetInstance()->Send(&SendReq, sizeof(SendReq), /*MAIN_ID_LOGINREGISTER,*/ ASSIST_ID_UPDATE_SINGLE_SCORE_REQ);
}

void UpdateScoreSysProc::UpdateSingleScoreBySubjectsChooseHandle(char* pName, char* pAccount, char* pGrade, unsigned char* pSubjectsId, unsigned char* pScore, unsigned char bSubjectCount)
{
	if (NULL == pAccount)
	{
		printf("%s �˺�ΪNULL\n", __FUNCTION__);
		SetIEndFlag(-1);
		return;
	}
	if (NULL == pSubjectsId || 0 == bSubjectCount || NULL == pScore)
	{
		printf("%s ��Ŀid�����ΪNULL���߱������û�û�п�Ŀ�ֶ�\n", __FUNCTION__);
		SetIEndFlag(-1);
		return;
	}
	string strName;
	string strGrade;
	if (NULL == pName)
	{
		strName = "NULL";
	}
	else
	{
		strName = pName;
	}
	if (NULL == pGrade)
	{
		strGrade = "NULL";
	}
	else
	{
		strGrade = pGrade;
	}


	//����ʾ�������û����ڵķ������
	printf("�༶[%s] �˺�[%s] �û�����[%s]���гɼ����������\n", strGrade.c_str(), pAccount, strName.c_str());
	ShowSubjects(pSubjectsId, pScore, bSubjectCount, 3);

	printf("���մ����Һʹ��ϵ��µ�˳�������Ӧ��Ŀ���·�������ʽΪ XX|XX|XX����\n");
	string strScore;
	cin>>strScore;
	if (!(CheckTool::CheckStringLen(strScore, 90) && CheckScore(strScore)))
	{
		OperInputErrorHandle(false);
		return;
	}

	unsigned char bScore[MAXSUBJECTSCOUNT];
	StringTool::StrSpliteToUcArray(bScore, MAXSUBJECTSCOUNT, strScore, "|");

	CS_MSG_UPDATE_SINGLE_SCORE_REQ SendReq;
	SendReq.sType = 2;
	SendReq.bSubjectCount = bSubjectCount;
	memcpy(SendReq.sSubjectId, pSubjectsId, sizeof(unsigned char)*MAXSUBJECTSCOUNT);
	memcpy(SendReq.bScore, bScore, sizeof(unsigned char)*MAXSUBJECTSCOUNT);
	strcpy_s(SendReq.cAccount, sizeof(SendReq.cAccount), pAccount);

	TCPHandle::GetInstance()->Send(&SendReq, sizeof(SendReq), /*MAIN_ID_LOGINREGISTER,*/ ASSIST_ID_UPDATE_SINGLE_SCORE_REQ);
}

bool UpdateScoreSysProc::GetSubjectsAfterUpdateSingleScoreRecvHandle(void* vpData, unsigned int DataLen)
{
	if (OPER_PER_UPDATESINGLESCOREBYONESUBJECT != GetCurOper() && OPER_PER_UPDATESINGLESCOREBYSUBJECTS != GetCurOper())
	{
		printf("%s ���ǽ��иò���[%d | %d]����ǰ���еĲ�����[%d] error\n", __FUNCTION__, OPER_PER_UPDATESINGLESCOREBYONESUBJECT, OPER_PER_UPDATESINGLESCOREBYSUBJECTS, GetCurOper());
		SetIEndFlag(-1);
		return false;
	}
	if (DataLen != sizeof(SC_MSG_GET_SUBJECTS_ACK))
	{
		printf("DataLen[%u] error, It should be [%u]\n", DataLen, sizeof(SC_MSG_GET_SUBJECTS_ACK));
		SetIEndFlag(-1);
		return false;
	}

	SC_MSG_GET_SUBJECTS_ACK* RecvMSG = (SC_MSG_GET_SUBJECTS_ACK*) vpData;
	if (RecvMSG->bSucceed)
	{
		GetSubjectsScoreAfterUpdateSingleScoreChooseHandle(RecvMSG->cCanAlterSubjects);
	}
	else
	{
		printf("%s �ɸ��Ŀ�Ŀ���ز��ɹ�\n", __FUNCTION__);
		SetIEndFlag(-1);
		return false;
	}
	return true;
}

bool UpdateScoreSysProc::GetSubjectsScoreAfterUpdateSingleScoreRecvHandle(void* vpData, unsigned int DataLen)
{
	if (OPER_PER_UPDATESINGLESCOREBYONESUBJECT != GetCurOper() && OPER_PER_UPDATESINGLESCOREBYSUBJECTS != GetCurOper())
	{
		printf("%s ���ǽ��иò���[%d | %d]����ǰ���еĲ�����[%d] error\n", __FUNCTION__, OPER_PER_UPDATESINGLESCOREBYONESUBJECT, OPER_PER_UPDATESINGLESCOREBYSUBJECTS, GetCurOper());
		SetIEndFlag(-1);
		return false;
	}
	if (DataLen != sizeof(SC_MSG_SELECT_SINGLE_SCORE_ACK))
	{
		printf("%s DataLen[%u] error, It should be [%u]\n", DataLen, sizeof(SC_MSG_SELECT_SINGLE_SCORE_ACK));
		SetIEndFlag(-1);
		return false;
	}
	SC_MSG_SELECT_SINGLE_SCORE_ACK* RecvMSG = (SC_MSG_SELECT_SINGLE_SCORE_ACK*) vpData;

	if (RecvMSG->bResCode == 1)
	{
		printf("%s ������ѯʧ��\n", __FUNCTION__);
		SetIEndFlag(-1);
		return false;
	}
	else if (RecvMSG->bResCode == 2)
	{
		printf("%s û���û���Ϣ�����û�������Ϣ\n", __FUNCTION__);
		SetIEndFlag(-1);
		return false;
	}
	else if (RecvMSG->bResCode == 3)
	{
		printf("%s �����쳣\n", __FUNCTION__);
		SetIEndFlag(-1);
		return false;
	}

	if (OPER_PER_UPDATESINGLESCOREBYONESUBJECT == GetCurOper())
	{
		UpdateSingleScoreByOneSubjectChooseHandle(RecvMSG->cName, RecvMSG->cAccount, RecvMSG->cGrade,RecvMSG->bSubjectId, RecvMSG->bScore, RecvMSG->bSubjectCount);
	}
	else if (OPER_PER_UPDATESINGLESCOREBYSUBJECTS == GetCurOper())
	{
		UpdateSingleScoreBySubjectsChooseHandle(RecvMSG->cName, RecvMSG->cAccount, RecvMSG->cGrade,RecvMSG->bSubjectId, RecvMSG->bScore, RecvMSG->bSubjectCount);
	}
	else
	{
		printf("%s ���ǽ��иò���[%d | %d]����ǰ���еĲ�����[%d] error\n",__FUNCTION__ ,OPER_PER_UPDATESINGLESCOREBYONESUBJECT, OPER_PER_UPDATESINGLESCOREBYSUBJECTS, GetCurOper());
		SetIEndFlag(-1);
		return false;
	}

	return true;
}

bool UpdateScoreSysProc::UpdateSingleScoreRecvHandle(void* vpData, unsigned int DataLen)
{
	if (OPER_PER_UPDATESINGLESCOREBYONESUBJECT != GetCurOper() && OPER_PER_UPDATESINGLESCOREBYSUBJECTS != GetCurOper())
	{
		printf("%s ���ǽ��иò���[%d | %d]����ǰ���еĲ�����[%d] error\n", __FUNCTION__, OPER_PER_UPDATESINGLESCOREBYONESUBJECT, OPER_PER_UPDATESINGLESCOREBYSUBJECTS, GetCurOper());
		SetIEndFlag(-1);
		return false;
	}
	if (DataLen != sizeof(SC_MSG_UPDATE_SINGLE_SCORE_ACK))
	{
		printf("%s DataLen[%u] error, It should be [%u]\n", DataLen, sizeof(SC_MSG_UPDATE_SINGLE_SCORE_ACK));
		SetIEndFlag(-1);
		return false;
	}
	SC_MSG_UPDATE_SINGLE_SCORE_ACK* RecvMSG = (SC_MSG_UPDATE_SINGLE_SCORE_ACK*) vpData;

	if (RecvMSG->bSucceed)
	{
		if (RecvMSG->sType == 1)
		{
			printf("���˺�[%s]���ĵ��Ʒ����ɹ�\n", RecvMSG->cAccount);
			SetIEndFlag(1);
		}
		else if (RecvMSG->sType == 2)
		{
			printf("���˺�[%s]�����������п�Ŀ�����ɹ�\n", RecvMSG->cAccount);
			SetIEndFlag(1);
		}
		else
		{
			printf("%s  RecvMSG->sType=[%d] error\n", __FUNCTION__, (int)RecvMSG->sType);
			SetIEndFlag(-1);
			return false;
		}
	}
	else
	{
		SetIEndFlag(-1);
		printf("���ķ���ʧ��");
		return false;
	}

	return true;
}


bool UpdateScoreSysProc::ShowSubjects(unsigned char* pSubjectsId, unsigned char* pScore, unsigned char bSubjectCount, int iField)
{
	if (!pSubjectsId || !pScore || 0 == bSubjectCount || bSubjectCount>MAXSUBJECTSCOUNT)
	{
		printf("%s �������bSubjectCount=%u\n", __FUNCTION__, (unsigned)bSubjectCount);
		return false;
	}

	if (iField == 0)
		iField = 0xEFFFFFFF;
	else if (iField < 0)
		iField *= -1;

	m_vecStrChineseName.clear();
	for (unsigned char i=0; i<bSubjectCount; i++)
	{
		if (UserInfoMgr::GetInstance()->CanFindSubjectsType((SubjectsType)pSubjectsId[i]))
		{
			m_vecStrChineseName.push_back(UserInfoMgr::GetInstance()->GetChineseNameByType((SubjectsType)pSubjectsId[i]));
		}
	}

	if (m_vecStrChineseName.size() != bSubjectCount)
	{
		printf("%s �����Ŀ�Ŀ�����ʹ�����������ȣ�����Ŀ�Ŀ����bSubjectCount=%u��������[%u]\n", __FUNCTION__, (unsigned)bSubjectCount, m_vecStrChineseName.size());
		return false;
	}

	unsigned char iCount = 0;
	for (; iCount<bSubjectCount; iCount++)
	{
		if (iCount != 0 && iCount % iField == 0)
		{
			cout<<endl;
		}
		if (iCount != 0 && iCount % iField != 0)
		{
			cout<<"\t\t";
		}
		printf("%d--%s[%u]", pSubjectsId[iCount], m_vecStrChineseName.at(iCount).c_str(), (unsigned)pScore[iCount]);
	}
	if (iCount > 0)
	{
		cout<<endl;
	}

	return true;
}

bool UpdateScoreSysProc::FindSubjectsId(unsigned char* pSubjectsId, unsigned char bSubjectCount, unsigned char SubjectsId)
{
	bool bRes = false;
	if (!pSubjectsId || bSubjectCount==0 || bSubjectCount>MAXSUBJECTSCOUNT)
	{
		return bRes;
	}

	for (unsigned char i=0; i<bSubjectCount; i++)
	{
		if (SubjectsId == pSubjectsId[i])
		{
			bRes = true;
			break;
		}
	}

	return bRes;
}

bool UpdateScoreSysProc::CheckScore(string sScore)
{
	if (sScore.empty())
	{
		return true;
	}

	vector<string> vScore = StringTool::Splite(sScore, "|");

	for (unsigned i=0; i < vScore.size(); i++)
	{	
		if (!CheckTool::CheckStringByValid(vScore.at(i), "0~9"))
		{
			return false;
		}
		int iScore = (int)atoi(vScore.at(i).c_str());
		if (iScore < 0 || iScore > 100)
		{
			return false;
		}
	}

	return true;
}