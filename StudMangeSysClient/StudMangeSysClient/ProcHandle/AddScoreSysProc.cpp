#include "AddScoreSysProc.h"
#include "ProcMgr.h"
#include "TCPHandle.h"
#include "CheckTool.h"
#include "UserInfoMgr.h"
#include "StringTool.h"

AddScoreSysProc::AddScoreSysProc(ProcDef nProcDef) : BaseProc(nProcDef)
{
	initMapChoose();
}

AddScoreSysProc::~AddScoreSysProc()
{

}


bool AddScoreSysProc::initMapChoose()
{
	m_mChoose.insert(pair<int, ChooseData>(GetMaxRealChoose(), ChooseData("�����������ӳɼ�", OPER_PER_ADDBATCHSCOREBYONESUBJECT, PROC_DEF_ADDSCORESYSPROC)));
	m_mChoose.insert(pair<int, ChooseData>(++GetMaxRealChoose(), ChooseData("�������п�Ŀ�������ӳɼ�", OPER_PER_ADDBATCHSCOREBYSUBJECTS, PROC_DEF_ADDSCORESYSPROC)));
	m_mChoose.insert(pair<int, ChooseData>(++GetMaxRealChoose(), ChooseData("���Ƶ������ӳɼ�", OPER_PER_ADDSINGLESCOREBYONESUBJECT, PROC_DEF_ADDSCORESYSPROC)));
	m_mChoose.insert(pair<int, ChooseData>(++GetMaxRealChoose(), ChooseData("�������п�Ŀ�������ӳɼ�", OPER_PER_ADDSINGLESCOREBYSUBJECTS, PROC_DEF_ADDSCORESYSPROC)));
	m_mChoose.insert(pair<int, ChooseData>(++GetMaxRealChoose(), ChooseData("����", OPER_PER_INVALID, PROC_DEF_SCORESYSPROC)));

	return true;
}

void AddScoreSysProc::EndProc()
{
	__super::EndProc();
}

void AddScoreSysProc::StartRecv(void* vpData, unsigned int DataLen, /*int iMianId,*/ int iAssistId)
{
	bool bRes = false;
	switch(iAssistId)
	{
	case ASSIST_ID_GET_SUBJECTS_ACK:
		bRes = GetSubjectsAfterAddSingleScoreRecvHandle(vpData, DataLen);
		break;
	case ASSIST_ID_ADD_SINGLE_SCORE_ACK:
		bRes = AddSingleScoreRecvHandle(vpData, DataLen);
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

void AddScoreSysProc::EndRecv()
{
	__super::EndRecv();
}

void AddScoreSysProc::SwitchToOper(OperPermission CurOper)
{
	__super::SwitchToOper(CurOper);


	switch(CurOper)
	{
	case OPER_PER_ADDBATCHSCOREBYONESUBJECT: //�����������ӳɼ�
		break;
	case OPER_PER_ADDBATCHSCOREBYSUBJECTS: //�������п�Ŀ�������ӳɼ�
		break;
	case OPER_PER_ADDSINGLESCOREBYONESUBJECT: //���Ƶ������ӳɼ�  ����ǰ������
	case OPER_PER_ADDSINGLESCOREBYSUBJECTS: //�������п�Ŀ�������ӳɼ�  ����ǰ������
		GetSubjectsAfterAddSingleScoreChooseHandle();
		break;
	default:
		printf("%s û�иò�����\n", __FUNCTION__);
		ProcMgr::GetInstance()->ProcSwitch(GetMyProcDef(), true);  
		break;
	}
}

void AddScoreSysProc::GetSubjectsAfterAddSingleScoreChooseHandle()
{
	//��ȡ���п�Ŀ
	CS_MSG_GET_SUBJECTS_REQ SendReq;
	SendReq.sGetType = 2;

	TCPHandle::GetInstance()->Send(&SendReq, sizeof(SendReq), /*MAIN_ID_LOGINREGISTER,*/ ASSIST_ID_GET_SUBJECTS_REQ);
}

//���Ƶ������ӳɼ�
void AddScoreSysProc::AddSingleScoreByOneSubjectChooseHandle(char* pStrExistSubjects)
{
	if (NULL == pStrExistSubjects)
	{
		printf("%s ��Ŀ�ֶ�ΪNULL\n", __FUNCTION__);
		SetIEndFlag(-1);
		return;
	}

	vector<string> vecStrSubjectId = StringTool::Splite(pStrExistSubjects, "|");
	map<int, string> mStrChineseNameShow; //��ʾ�Ŀ�Ŀ
	for (unsigned i=0; i<vecStrSubjectId.size(); i++)
	{
		int sId = (int)atoi(vecStrSubjectId.at(i).c_str());
		if (UserInfoMgr::GetInstance()->CanFindSubjectsType((SubjectsType)sId))
		{
			mStrChineseNameShow.insert(pair<int, string>(sId, UserInfoMgr::GetInstance()->GetChineseNameByType((SubjectsType)sId)));
		}
	}

	if (mStrChineseNameShow.empty())
	{
		printf("û�п�Ŀ��������ӿ�Ŀ\n");
		SetIEndFlag(1);
		return;
	}

	cout<<"��������ӷ���������û�����"<<endl;
	string strAccount;
	cin>>strAccount;
	if (!(CheckTool::CheckStringLen(strAccount, 30) && CheckTool::CheckStringByValid(strAccount, "A~Z|a~z|0~9")))
	{
		OperInputErrorHandle(false);
		return;
	}

	printf("��ѡ��Ҫ��ӷ����Ŀ�ĿID��\n");
	ShowSubjects(mStrChineseNameShow);
	string iChooseTmp;
	cin>>iChooseTmp;
	if (!CheckTool::CheckStringByValid(iChooseTmp, "0~9") || mStrChineseNameShow.find((int)atoi(iChooseTmp.c_str())) == mStrChineseNameShow.end())
	{
		OperInputErrorHandle(false);
		return;
	}

	printf("������ÿ�Ŀ������\n");
	string strScore;
	cin>>strScore;
	if (!(CheckTool::CheckStringLen(strScore, 3) && CheckTool::CheckStringByValid(strScore, "0~9") && (int)atoi(strScore.c_str()) >= 0 && (int)atoi(strScore.c_str()) <= 100))
	{
		OperInputErrorHandle(false);
		return;
	}

	CS_MSG_ADD_SINGLE_SCORE_REQ SendReq;
	SendReq.sType = 1;
	strcpy_s(SendReq.cAccount, sizeof(SendReq.cAccount), strAccount.c_str());
	strcpy_s(SendReq.sSubjectId, sizeof(SendReq.sSubjectId), iChooseTmp.c_str());
	strcpy_s(SendReq.sScore, sizeof(SendReq.sScore), strScore.c_str());

	TCPHandle::GetInstance()->Send(&SendReq, sizeof(SendReq), /*MAIN_ID_LOGINREGISTER,*/ ASSIST_ID_ADD_SINGLE_SCORE_REQ);
}

//�������п�Ŀ�������ӳɼ�
void AddScoreSysProc::AddSingleScoreBySubjectsChooseHandle(char* pStrExistSubjects)
{
	if (NULL == pStrExistSubjects)
	{
		printf("%s ��Ŀ�ֶ�ΪNULL\n", __FUNCTION__);
		SetIEndFlag(-1);
		return;
	}

	vector<string> vecStrSubjectId = StringTool::Splite(pStrExistSubjects, "|");
	vector<int> vecSubjectId;
	map<int, string> mStrChineseNameShow; //��ʾ�Ŀ�Ŀ
	for (unsigned i=0; i<vecStrSubjectId.size(); i++)
	{
		int sId = (int)atoi(vecStrSubjectId.at(i).c_str());
		if (UserInfoMgr::GetInstance()->CanFindSubjectsType((SubjectsType)sId))
		{
			mStrChineseNameShow.insert(pair<int, string>(sId, UserInfoMgr::GetInstance()->GetChineseNameByType((SubjectsType)sId)));
			vecSubjectId.push_back(sId);
		}
	}

	if (mStrChineseNameShow.empty())
	{
		printf("û�п�Ŀ��������ӿ�Ŀ\n");
		SetIEndFlag(1);
		return;
	}

	cout<<"����ӷ���������û�����"<<endl;
	string strAccount;
	cin>>strAccount;
	if (!(CheckTool::CheckStringLen(strAccount, 30) && CheckTool::CheckStringByValid(strAccount, "A~Z|a~z|0~9")))
	{
		OperInputErrorHandle(false);
		return;
	}
	
	printf("���մ����Һʹ��ϵ��µ�˳�������Ӧ��Ŀ��������ʽΪ XX|XX|XX����\n");
	ShowSubjects(mStrChineseNameShow, 5);
	string strScore;
	cin>>strScore;
	if (!(CheckTool::CheckStringLen(strScore, 90) && CheckScore(strScore)))
	{
		OperInputErrorHandle(false);
		return;
	}

	CS_MSG_ADD_SINGLE_SCORE_REQ SendReq;
	SendReq.sType = 2;
	strcpy_s(SendReq.cAccount, sizeof(SendReq.cAccount), strAccount.c_str());
	strcpy_s(SendReq.sSubjectId, sizeof(SendReq.sSubjectId), StringTool::CombVecToStr(vecSubjectId).c_str());
	strcpy_s(SendReq.sScore, sizeof(SendReq.sScore), strScore.c_str());

	TCPHandle::GetInstance()->Send(&SendReq, sizeof(SendReq), /*MAIN_ID_LOGINREGISTER,*/ ASSIST_ID_ADD_SINGLE_SCORE_REQ);
}

bool AddScoreSysProc::GetSubjectsAfterAddSingleScoreRecvHandle(void* vpData, unsigned int DataLen)
{
	if (OPER_PER_ADDSINGLESCOREBYONESUBJECT != GetCurOper() && OPER_PER_ADDSINGLESCOREBYSUBJECTS != GetCurOper())
	{
		printf("���ǽ��иò���[%d | %d]����ǰ���еĲ�����[%d] error\n", OPER_PER_ADDSINGLESCOREBYONESUBJECT, OPER_PER_ADDSINGLESCOREBYSUBJECTS, GetCurOper());
		SetIEndFlag(-1);
		return false;
	}
	if (DataLen != sizeof(CS_MSG_GET_SUBJECTS_ACK))
	{
		printf("DataLen[%u] error, It should be [%u]\n", DataLen, sizeof(CS_MSG_GET_SUBJECTS_ACK));
		SetIEndFlag(-1);
		return false;
	}

	CS_MSG_GET_SUBJECTS_ACK* RecvMSG = (CS_MSG_GET_SUBJECTS_ACK*) vpData;
	if (RecvMSG->bSucceed)
	{
		if (OPER_PER_ADDSINGLESCOREBYONESUBJECT == GetCurOper())
		{
			AddSingleScoreByOneSubjectChooseHandle(RecvMSG->cCanAlterSubjects);
		}
		else if(OPER_PER_ADDSINGLESCOREBYSUBJECTS == GetCurOper())
		{
			AddSingleScoreBySubjectsChooseHandle(RecvMSG->cCanAlterSubjects);
		}
		else
		{
			//printf("%s  RecvMSG->sGetType=[%d] error\n", __FUNCTION__, (int)RecvMSG->sGetType);
			SetIEndFlag(-1);
			return false;
		}

	}
	else
	{
		printf("����ɾ��Ŀ���ز��ɹ�");
		SetIEndFlag(-1);
		return false;
	}
	return true;
}

bool AddScoreSysProc::AddSingleScoreRecvHandle(void* vpData, unsigned int DataLen)
{
	if (OPER_PER_ADDSINGLESCOREBYONESUBJECT != GetCurOper() && OPER_PER_ADDSINGLESCOREBYSUBJECTS != GetCurOper())
	{
		printf("���ǽ��иò���[%d | %d]����ǰ���еĲ�����[%d] error\n", OPER_PER_ADDSINGLESCOREBYONESUBJECT, OPER_PER_ADDSINGLESCOREBYSUBJECTS, GetCurOper());
		SetIEndFlag(-1);
		return false;
	}
	if (DataLen != sizeof(CS_MSG_ADD_SINGLE_SCORE_ACK))
	{
		printf("DataLen[%u] error, It should be [%u]\n", DataLen, sizeof(CS_MSG_ADD_SINGLE_SCORE_ACK));
		SetIEndFlag(-1);
		return false;
	}

	CS_MSG_ADD_SINGLE_SCORE_ACK* RecvMSG = (CS_MSG_ADD_SINGLE_SCORE_ACK*) vpData;
	if (RecvMSG->bSucceed)
	{
		if (RecvMSG->sType == 1)
		{
			printf("���˺�[%s]���ӵ��Ʒ����ɹ�\n", RecvMSG->cAccount);
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
		printf("��ӷ���ʧ��");
		return false;
	}

	return true;
}


int AddScoreSysProc::ShowSubjects(map<int, string> mIStr, int iField)
{
	if (iField == 0)
		iField = 0xEFFFFFFF;
	else if (iField < 0)
		iField *= -1;

	int iCount = 0;
	for (map<int, string>::iterator iter = mIStr.begin(); iter != mIStr.end(); iter++)
	{
		if (iCount != 0 && iCount % iField == 0)
		{
			cout<<endl;
		}
		if (iCount != 0 && iCount % iField != 0)
		{
			cout<<"\t\t";
		}
		iCount++;
		printf("%d--%s", iter->first, iter->second.c_str());
	}
	if (iCount > 0)
	{
		cout<<endl;
	}

	return iCount;
}

bool AddScoreSysProc::CheckScore(string sScore)
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