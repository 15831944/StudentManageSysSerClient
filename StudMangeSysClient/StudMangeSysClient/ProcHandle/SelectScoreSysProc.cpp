#include "SelectScoreSysProc.h"
#include "TCPHandle.h"
#include "ProcMgr.h"
#include "StringTool.h"
#include "UserInfoMgr.h"
#include "CheckTool.h"

SelectScoreSysProc::SelectScoreSysProc(ProcDef nProcDef) : BaseProc(nProcDef)
{
	initMapChoose();
}

SelectScoreSysProc::~SelectScoreSysProc()
{

}

bool SelectScoreSysProc::initMapChoose()
{
	m_mChoose.insert(pair<int, ChooseData>(GetMaxRealChoose(), ChooseData("����������ѯ�ɼ�", OPER_PER_SELECTBATCHSCOREBYONESUBJECT, PROC_DEF_SELECTSCORESYSPROC)));
	m_mChoose.insert(pair<int, ChooseData>(++GetMaxRealChoose(), ChooseData("�������п�Ŀ������ѯ�ɼ�", OPER_PER_SELECTBATCHSCOREBYSUBJECTS, PROC_DEF_SELECTSCORESYSPROC)));
	m_mChoose.insert(pair<int, ChooseData>(++GetMaxRealChoose(), ChooseData("���Ƶ�����ѯ�ɼ�", OPER_PER_SELECTSINGLESCOREBYONESUBJECT, PROC_DEF_SELECTSCORESYSPROC)));
	m_mChoose.insert(pair<int, ChooseData>(++GetMaxRealChoose(), ChooseData("�������п�Ŀ������ѯ�ɼ�", OPER_PER_SELECTSINGLESCOREBYSUBJECTS, PROC_DEF_SELECTSCORESYSPROC)));
	m_mChoose.insert(pair<int, ChooseData>(++GetMaxRealChoose(), ChooseData("����", OPER_PER_INVALID, PROC_DEF_SCORESYSPROC)));

	return true;
}

void SelectScoreSysProc::EndProc()
{
	__super::EndProc();
}

void SelectScoreSysProc::StartRecv(void* vpData, unsigned int DataLen, /*int iMianId,*/ int iAssistId)
{
	bool bRes = false;
	switch(iAssistId)
	{
	case ASSIST_ID_GET_SUBJECTS_ACK:
		bRes = GetSubjectsAfterSelectScoreRecvHandle(vpData, DataLen);
		break;
	case ASSIST_ID_SELECT_SINGLE_SCORE_ACK:
		bRes = SelectSingleScoreRecvHandle(vpData, DataLen);
		break;
	case ASSIST_ID_SELECT_BATCH_SCORE_ACK:
		bRes = SelectBatchScoreRecvHandle(vpData, DataLen);
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

void SelectScoreSysProc::EndRecv()
{
	__super::EndRecv();
}

void SelectScoreSysProc::SwitchToOper(OperPermission CurOper)
{
	__super::SwitchToOper(CurOper);


	switch(CurOper)
	{
	case OPER_PER_SELECTSINGLESCOREBYONESUBJECT: //���Ƶ�����ѯ  ����ǰ������
	case OPER_PER_SELECTSINGLESCOREBYSUBJECTS: //�������п�Ŀ�������ĳɼ�  ����ǰ������
	case OPER_PER_SELECTBATCHSCOREBYONESUBJECT: //����������ѯ  ����ǰ������
	case OPER_PER_SELECTBATCHSCOREBYSUBJECTS: //�������п�Ŀ�������ĳɼ�  ����ǰ������
		GetSubjectsAfterSelectScoreChooseHandle();
		break;
	default:
		printf("%s û�иò�����\n", __FUNCTION__);
		ProcMgr::GetInstance()->ProcSwitch(GetMyProcDef(), true);  
		break;
	}
}


void SelectScoreSysProc::GetSubjectsAfterSelectScoreChooseHandle()
{
	//��ȡ���п�Ŀ
	CS_MSG_GET_SUBJECTS_REQ SendReq;
	SendReq.sGetType = 2;

	TCPHandle::GetInstance()->Send(&SendReq, sizeof(SendReq), /*MAIN_ID_LOGINREGISTER,*/ ASSIST_ID_GET_SUBJECTS_REQ);
}

void SelectScoreSysProc::SelectSingleScoreByOneSubjectChooseHandle(char* pStrExistSubjects)
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
		printf("%s û�п�Ŀ��������ӿ�Ŀ\n", __FUNCTION__);
		SetIEndFlag(1);
		return;
	}
	if (mStrChineseNameShow.size() != vecStrSubjectId.size())
	{
		printf("%s �������Ŀ�Ŀ������ԭ��Ŀ������ͬ\n", __FUNCTION__);
		SetIEndFlag(-1);
		return;
	}

	string strAccount;
	if (UserInfoMgr::GetInstance()->GetUserIdent() == 1)
	{
		strAccount = UserInfoMgr::GetInstance()->GetUserAccount();
	}
	else
	{
		cout<<"�����뱻��ѯ�ɼ�������û�����"<<endl;
		cin>>strAccount;
		if (!(CheckTool::CheckStringLen(strAccount, 30) && CheckTool::CheckStringByValid(strAccount, "A~Z|a~z|0~9")))
		{
			OperInputErrorHandle(false);
			return;
		}
	}

	printf("��ѡ��Ҫ��Ҫ��ѯ�����Ŀ�ĿID��\n");
	ShowSubjects(mStrChineseNameShow);
	string iChooseTmp;
	cin>>iChooseTmp;
	if (!CheckTool::CheckStringByValid(iChooseTmp, "0~9") || mStrChineseNameShow.find((int)atoi(iChooseTmp.c_str())) == mStrChineseNameShow.end())
	{
		OperInputErrorHandle(false);
		return;
	}

	CS_MSG_SELECT_SINGLE_SCORE_REQ SendReq;
	SendReq.sType = 1;
	SendReq.bSubjectCount = 1;
	SendReq.sSubjectId[0] = (unsigned char)atoi(iChooseTmp.c_str());
	strcpy_s(SendReq.cAccount, sizeof(SendReq.cAccount), strAccount.c_str());

	TCPHandle::GetInstance()->Send(&SendReq, sizeof(SendReq), /*MAIN_ID_LOGINREGISTER,*/ ASSIST_ID_SELECT_SINGLE_SCORE_REQ);
}

void SelectScoreSysProc::SelectSingleScoreBySubjectsChooseHandle(char* pStrExistSubjects)
{
	if (NULL == pStrExistSubjects)
	{
		printf("%s ��Ŀ�ֶ�ΪNULL\n", __FUNCTION__);
		SetIEndFlag(-1);
		return;
	}

	CS_MSG_SELECT_SINGLE_SCORE_REQ SendReq;
	SendReq.sType = 2;

	vector<string> vecStrSubjectId = StringTool::Splite(pStrExistSubjects, "|");
	vector<unsigned char> vecUcSubjectId; //��Ч�Ŀ�Ŀid
	for (unsigned i=0; i<vecStrSubjectId.size(); i++)
	{
		unsigned char sId = (unsigned char)atoi(vecStrSubjectId.at(i).c_str());
		if (UserInfoMgr::GetInstance()->CanFindSubjectsType((SubjectsType)sId))
		{
			vecUcSubjectId.push_back(sId);
		}
	}

	if (vecUcSubjectId.empty())
	{
		printf("%s û�п�Ŀ��������ӿ�Ŀ\n", __FUNCTION__);
		SetIEndFlag(1);
		return;
	}
	if (vecUcSubjectId.size() != vecStrSubjectId.size())
	{
		printf("%s �������Ŀ�Ŀ������ԭ��Ŀ������ͬ\n", __FUNCTION__);
		SetIEndFlag(-1);
		return;
	}

	string strAccount;
	if (UserInfoMgr::GetInstance()->GetUserIdent() == 1)
	{
		strAccount = UserInfoMgr::GetInstance()->GetUserAccount();
	}
	else
	{
		cout<<"�����뱻��ѯ�ɼ�������û�����"<<endl;
		cin>>strAccount;
		if (!(CheckTool::CheckStringLen(strAccount, 30) && CheckTool::CheckStringByValid(strAccount, "A~Z|a~z|0~9")))
		{
			OperInputErrorHandle(false);
			return;
		}
	}
	strcpy_s(SendReq.cAccount, sizeof(SendReq.cAccount), strAccount.c_str());

	SendReq.bSubjectCount = 0;
	for (;SendReq.bSubjectCount<vecUcSubjectId.size(); SendReq.bSubjectCount++)
	{
		SendReq.sSubjectId[SendReq.bSubjectCount] = vecUcSubjectId.at(SendReq.bSubjectCount);
	}

	TCPHandle::GetInstance()->Send(&SendReq, sizeof(SendReq), /*MAIN_ID_LOGINREGISTER,*/ ASSIST_ID_SELECT_SINGLE_SCORE_REQ);
}

void SelectScoreSysProc::SelectBatchScoreByOneSubjectChooseHandle(char* pStrExistSubjects)
{
	if (NULL == pStrExistSubjects)
	{
		printf("%s ��Ŀ�ֶ�ΪNULL\n", __FUNCTION__);
		SetIEndFlag(-1);
		return;
	}

	CS_MSG_SELECT_BATCH_SCORE_REQ SendReq;
	SendReq.sType = 1;

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
		printf("%s û�п�Ŀ��������ӿ�Ŀ\n", __FUNCTION__);
		SetIEndFlag(1);
		return;
	}
	if (mStrChineseNameShow.size() != vecStrSubjectId.size())
	{
		printf("%s �������Ŀ�Ŀ������ԭ��Ŀ������ͬ\n", __FUNCTION__);
		SetIEndFlag(-1);
		return;
	}

	printf("��ѡ��Ҫ��Ҫ��ѯ�����Ŀ�ĿID��\n");
	ShowSubjects(mStrChineseNameShow);
	string iChooseTmp;
	cin>>iChooseTmp;
	if (!CheckTool::CheckStringByValid(iChooseTmp, "0~9") || mStrChineseNameShow.find((int)atoi(iChooseTmp.c_str())) == mStrChineseNameShow.end())
	{
		OperInputErrorHandle(false);
		return;
	}
	SendReq.bSubjectCount = 1;
	SendReq.sSubjectId[0] = (unsigned char)atoi(iChooseTmp.c_str());

	cout<<"�Ƿ���ݰ༶��ѯ��1--��  2--�񣩣�"<<endl;
	string strGradeChoose;
	string strGrade;
	cin>>strGradeChoose;
	if (!(CheckTool::CheckStringLen(strGradeChoose, 1) && CheckTool::CheckStringByValid(strGradeChoose, "1|2")))
	{
		OperInputErrorHandle(false);
		return;
	}
	else
	{
		if (strGradeChoose == "1") //�Ժ��Ż���ѡ�Ҳ������Ҫ�ӷ�����Ȼ�ȡ���а༶
		{
			cout<<"����������Ҫ��ѯ�İ༶���ƣ��� ����11-1����"<<endl;
			cin>>strGrade;
		}
	}
	if (strGrade.empty())
	{
		memset(SendReq.cGrade, 0, sizeof(SendReq.cGrade));
	}
	else
	{
		strcpy_s(SendReq.cGrade, sizeof(SendReq.cGrade), strGrade.c_str());
	}

	cout<<"�Ƿ���Ҫ������ѯ��ѯ��1--��  2--�񣩣�"<<endl;
	string strConditonChoose;
	string strConditon;
	cin>>strConditonChoose;
	if (!(CheckTool::CheckStringLen(strConditonChoose, 1) && CheckTool::CheckStringByValid(strConditonChoose, "1|2")))
	{
		OperInputErrorHandle(false);
		return;
	}
	else
	{
		if (strConditonChoose == "1") 
		{
			printf("��ѡ���ѯ�����������ʽΪ X|X|X��[ѡ�%d-%s  %d-%s  %d-%s  %d-%s]��\n", SELECT_SCORE_CONDITION_RANK, "����", SELECT_SCORE_CONDITION_RANGE, "������Χ", SELECT_SCORE_CONDITION_TOTAL, "�ܷ�", SELECT_SCORE_CONDITION_AVERAGE, "ƽ����");
			cin>>strConditon;
			if (!(CheckTool::CheckStringLen(strConditon, SELECT_SCORE_CONDITION_END*2-3) && CheckStringVaildRemoveSpl(strConditon, "|", "1|2|3|4"))) 
			{
				OperInputErrorHandle(false);
				return;
			}
		}
	}
	unsigned cConditionCount = 0;
	if (strConditon.empty())
	{
		memset(SendReq.cCondition, 0, sizeof(SendReq.cCondition));
	}
	else
	{
		vector<string> vecStrConditon = StringTool::Splite(strConditon, "|");
		if (vecStrConditon.size() >= SELECT_SCORE_CONDITION_END)
		{
			printf("%s ��ѯ��������\n", __FUNCTION__);
			OperInputErrorHandle(false);
			return;
		}
		for (; cConditionCount<vecStrConditon.size() && cConditionCount<SELECT_SCORE_CONDITION_END-1; cConditionCount++)
		{
			SendReq.cCondition[cConditionCount] = (unsigned char)atoi(vecStrConditon.at(cConditionCount).c_str());
		}
	}

	SendReq.bRankFlag = 0;
	if (FindOneInArray(SendReq.cCondition, cConditionCount, SELECT_SCORE_CONDITION_END-1, SELECT_SCORE_CONDITION_RANK))
	{
		printf("�������ѯ����ʽ��%d-%s  %d-%s����\n",SELECT_SCORE_RANK_ASC, "����", SELECT_SCORE_RANK_DESC, "����");
		string strRankFlag;
		cin>>strRankFlag;
		if (!(CheckTool::CheckStringLen(strRankFlag, 1) && CheckTool::CheckStringByValid(strRankFlag, "1|2")))
		{
			OperInputErrorHandle(false);
			return;
		}
		SendReq.bRankFlag = (unsigned char)atoi(strRankFlag.c_str());
	}

	SendReq.bScoreRange[2] = 0;
	if (FindOneInArray(SendReq.cCondition, cConditionCount, SELECT_SCORE_CONDITION_END-1, SELECT_SCORE_CONDITION_RANGE))
	{
		printf("��������Ҫ��ѯ�ķ�����Χ����ʽ XX-XX����\n");
		string strScoreRange;
		cin>>strScoreRange;
		vector<string> vecScoreRange = StringTool::Splite(strScoreRange, "-");
		if (!(vecScoreRange.size()==2 &&  CheckTool::CheckStringLen(vecScoreRange.at(0), 3) &&  CheckTool::CheckStringLen(vecScoreRange.at(1), 3) && \
			CheckTool::CheckStringByValid(vecScoreRange.at(0), "0~9") && CheckTool::CheckStringByValid(vecScoreRange.at(1), "0~9") && \
			(unsigned char)atoi(vecScoreRange.at(0).c_str())>=0 && (unsigned char)atoi(vecScoreRange.at(0).c_str())<=100 && \
			(unsigned char)atoi(vecScoreRange.at(1).c_str())>=0 && (unsigned char)atoi(vecScoreRange.at(1).c_str())<=100 && \
			(unsigned char)atoi(vecScoreRange.at(0).c_str())<=(unsigned char)atoi(vecScoreRange.at(1).c_str())))
		{
			OperInputErrorHandle(false);
			return;
		}
		SendReq.bScoreRange[0] = (unsigned char)atoi(vecScoreRange.at(0).c_str());
		SendReq.bScoreRange[1] = (unsigned char)atoi(vecScoreRange.at(1).c_str());
		SendReq.bScoreRange[2] = 1;
	}

	TCPHandle::GetInstance()->Send(&SendReq, sizeof(SendReq), /*MAIN_ID_LOGINREGISTER,*/ ASSIST_ID_SELECT_BATCH_SCORE_REQ);
}

void SelectScoreSysProc::SelectBatchScoreBySubjectsChooseHandle(char* pStrExistSubjects)
{
	if (NULL == pStrExistSubjects)
	{
		printf("%s ��Ŀ�ֶ�ΪNULL\n", __FUNCTION__);
		SetIEndFlag(-1);
		return;
	}
	
	CS_MSG_SELECT_BATCH_SCORE_REQ SendReq;
	SendReq.sType = 2;
	
	vector<string> vecStrSubjectId = StringTool::Splite(pStrExistSubjects, "|");
	vector<unsigned char> vecUcSubjectId; //��Ч�Ŀ�Ŀid
	for (unsigned i=0; i<vecStrSubjectId.size(); i++)
	{
		unsigned char sId = (unsigned char)atoi(vecStrSubjectId.at(i).c_str());
		if (UserInfoMgr::GetInstance()->CanFindSubjectsType((SubjectsType)sId))
		{
			vecUcSubjectId.push_back(sId);
		}
	}

	if (vecUcSubjectId.empty())
	{
		printf("%s û�п�Ŀ��������ӿ�Ŀ\n", __FUNCTION__);
		SetIEndFlag(1);
		return;
	}
	if (vecUcSubjectId.size() != vecStrSubjectId.size())
	{
		printf("%s �������Ŀ�Ŀ������ԭ��Ŀ������ͬ\n", __FUNCTION__);
		SetIEndFlag(-1);
		return;
	}

	SendReq.bSubjectCount = 0;
	for (;SendReq.bSubjectCount<vecUcSubjectId.size(); SendReq.bSubjectCount++)
	{
		SendReq.sSubjectId[SendReq.bSubjectCount] = vecUcSubjectId.at(SendReq.bSubjectCount);
	}

	cout<<"�Ƿ���ݰ༶��ѯ��1--��  2--�񣩣�"<<endl;
	string strGradeChoose;
	string strGrade;
	cin>>strGradeChoose;
	if (!(CheckTool::CheckStringLen(strGradeChoose, 1) && CheckTool::CheckStringByValid(strGradeChoose, "1|2")))
	{
		OperInputErrorHandle(false);
		return;
	}
	else
	{
		if (strGradeChoose == "1") //�Ժ��Ż���ѡ�Ҳ������Ҫ�ӷ�����Ȼ�ȡ���а༶
		{
			cout<<"����������Ҫ��ѯ�İ༶���ƣ��� ����11-1����"<<endl;
			cin>>strGrade;
		}
	}
	if (strGrade.empty())
	{
		memset(SendReq.cGrade, 0, sizeof(SendReq.cGrade));
	}
	else
	{
		strcpy_s(SendReq.cGrade, sizeof(SendReq.cGrade), strGrade.c_str());
	}

	cout<<"�Ƿ���Ҫ������ѯ��ѯ��1--��  2--�񣩣�"<<endl;
	string strConditonChoose;
	string strConditon;
	cin>>strConditonChoose;
	if (!(CheckTool::CheckStringLen(strConditonChoose, 1) && CheckTool::CheckStringByValid(strConditonChoose, "1|2")))
	{
		OperInputErrorHandle(false);
		return;
	}
	else
	{
		if (strConditonChoose == "1") 
		{
			printf("��ѡ���ѯ�����������ʽΪ X|X|X��[ѡ�%d-%s  %d-%s  %d-%s  %d-%s]��\n", SELECT_SCORE_CONDITION_RANK, "����", SELECT_SCORE_CONDITION_RANGE, "������Χ", SELECT_SCORE_CONDITION_TOTAL, "�ܷ�", SELECT_SCORE_CONDITION_AVERAGE, "ƽ����");
			cin>>strConditon;
			if (!(CheckTool::CheckStringLen(strConditon, SELECT_SCORE_CONDITION_END*2-3) && CheckStringVaildRemoveSpl(strConditon, "|", "1|2|3|4"))) 
			{
				OperInputErrorHandle(false);
				return;
			}
		}
	}
	unsigned cConditionCount = 0;
	if (strConditon.empty())
	{
		memset(SendReq.cCondition, 0, sizeof(SendReq.cCondition));
	}
	else
	{
		vector<string> vecStrConditon = StringTool::Splite(strConditon, "|");
		if (vecStrConditon.size() >= SELECT_SCORE_CONDITION_END)
		{
			printf("%s ��ѯ��������\n", __FUNCTION__);
			OperInputErrorHandle(false);
			return;
		}
		for (; cConditionCount<vecStrConditon.size() && cConditionCount<SELECT_SCORE_CONDITION_END-1; cConditionCount++)
		{
			SendReq.cCondition[cConditionCount] = (unsigned char)atoi(vecStrConditon.at(cConditionCount).c_str());
		}
	}

	SendReq.bRankFlag = 0;
	if (FindOneInArray(SendReq.cCondition, cConditionCount, SELECT_SCORE_CONDITION_END-1, SELECT_SCORE_CONDITION_RANK))
	{
		printf("�������ѯ����ʽ��%d-%s  %d-%s����\n",SELECT_SCORE_RANK_ASC, "����", SELECT_SCORE_RANK_DESC, "����");
		string strRankFlag;
		cin>>strRankFlag;
		if (!(CheckTool::CheckStringLen(strRankFlag, 1) && CheckTool::CheckStringByValid(strRankFlag, "1|2")))
		{
			OperInputErrorHandle(false);
			return;
		}
		SendReq.bRankFlag = (unsigned char)atoi(strRankFlag.c_str());
	}

	SendReq.bScoreRange[2] = 0;
	if (FindOneInArray(SendReq.cCondition, cConditionCount, SELECT_SCORE_CONDITION_END-1, SELECT_SCORE_CONDITION_RANGE))
	{
		printf("��������Ҫ��ѯ�ķ�����Χ����ʽ XX-XX����\n");
		string strScoreRange;
		cin>>strScoreRange;
		vector<string> vecScoreRange = StringTool::Splite(strScoreRange, "-");
		if (!(vecScoreRange.size()==2 &&  CheckTool::CheckStringLen(vecScoreRange.at(0), 3) &&  CheckTool::CheckStringLen(vecScoreRange.at(1), 3) && \
			CheckTool::CheckStringByValid(vecScoreRange.at(0), "0~9") && CheckTool::CheckStringByValid(vecScoreRange.at(1), "0~9") && \
			(unsigned char)atoi(vecScoreRange.at(0).c_str())>=0 && (unsigned char)atoi(vecScoreRange.at(0).c_str())<=100 && \
			(unsigned char)atoi(vecScoreRange.at(1).c_str())>=0 && (unsigned char)atoi(vecScoreRange.at(1).c_str())<=100 && \
			(unsigned char)atoi(vecScoreRange.at(0).c_str())<=(unsigned char)atoi(vecScoreRange.at(1).c_str())))
		{
			OperInputErrorHandle(false);
			return;
		}
		SendReq.bScoreRange[0] = (unsigned char)atoi(vecScoreRange.at(0).c_str());
		SendReq.bScoreRange[1] = (unsigned char)atoi(vecScoreRange.at(1).c_str());
		SendReq.bScoreRange[2] = 1;
	}

	TCPHandle::GetInstance()->Send(&SendReq, sizeof(SendReq), /*MAIN_ID_LOGINREGISTER,*/ ASSIST_ID_SELECT_BATCH_SCORE_REQ);
}

bool SelectScoreSysProc::GetSubjectsAfterSelectScoreRecvHandle(void* vpData, unsigned int DataLen)
{

	if (OPER_PER_SELECTSINGLESCOREBYONESUBJECT != GetCurOper() && OPER_PER_SELECTSINGLESCOREBYSUBJECTS != GetCurOper() && OPER_PER_SELECTBATCHSCOREBYONESUBJECT != GetCurOper() && OPER_PER_SELECTBATCHSCOREBYSUBJECTS != GetCurOper())
	{
		printf("���ǽ��иò���[%d | %d | %d | %d]����ǰ���еĲ�����[%d] error\n", OPER_PER_SELECTSINGLESCOREBYONESUBJECT, OPER_PER_SELECTSINGLESCOREBYSUBJECTS, OPER_PER_SELECTBATCHSCOREBYONESUBJECT, OPER_PER_SELECTBATCHSCOREBYSUBJECTS,GetCurOper());
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
		if (OPER_PER_SELECTSINGLESCOREBYONESUBJECT == GetCurOper())
		{
			SelectSingleScoreByOneSubjectChooseHandle(RecvMSG->cCanAlterSubjects);
		}
		else if(OPER_PER_SELECTSINGLESCOREBYSUBJECTS == GetCurOper())
		{
			SelectSingleScoreBySubjectsChooseHandle(RecvMSG->cCanAlterSubjects);
		}
		else if(OPER_PER_SELECTBATCHSCOREBYONESUBJECT == GetCurOper())
		{
			SelectBatchScoreByOneSubjectChooseHandle(RecvMSG->cCanAlterSubjects);
		}
		else if(OPER_PER_SELECTBATCHSCOREBYSUBJECTS == GetCurOper())
		{
			SelectBatchScoreBySubjectsChooseHandle(RecvMSG->cCanAlterSubjects);
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
		printf("�ɲ�ѯ��Ŀ���ز��ɹ�");
		SetIEndFlag(-1);
		return false;
	}
	return true;
}

bool SelectScoreSysProc::SelectSingleScoreRecvHandle(void* vpData, unsigned int DataLen)
{
	if (OPER_PER_SELECTSINGLESCOREBYONESUBJECT != GetCurOper() && OPER_PER_SELECTSINGLESCOREBYSUBJECTS != GetCurOper() )
	{
		printf("���ǽ��иò���[%d | %d]����ǰ���еĲ�����[%d] error\n", OPER_PER_SELECTSINGLESCOREBYONESUBJECT, OPER_PER_SELECTSINGLESCOREBYSUBJECTS,GetCurOper());
		SetIEndFlag(-1);
		return false;
	}
	if (DataLen != sizeof(SC_MSG_SELECT_SINGLE_SCORE_ACK))
	{
		printf("DataLen[%u] error, It should be [%u]\n", DataLen, sizeof(SC_MSG_SELECT_SINGLE_SCORE_ACK));
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

	vector<string> vSelectFieldShow;
	vector<string> vSelectResultShow;
	vSelectFieldShow.push_back("ѧ��");
	vSelectResultShow.push_back(StringTool::NumberToStr((int)RecvMSG->uUserid));
	vSelectFieldShow.push_back("����");
	vSelectResultShow.push_back(RecvMSG->cName);
	vSelectFieldShow.push_back("�˺�");
	vSelectResultShow.push_back(RecvMSG->cAccount);
	vSelectFieldShow.push_back("�༶");
	vSelectResultShow.push_back(RecvMSG->cGrade);

	unsigned char cInvalidCount = 0;
	for (unsigned char i=0; i<RecvMSG->bSubjectCount; i++)
	{
		SubjectsType sSubjectId = (SubjectsType)RecvMSG->bSubjectId[i];
		string strChineseName;
		string strScore = StringTool::NumberToStr((int)RecvMSG->bScore[i]);
		if (UserInfoMgr::GetInstance()->CanFindSubjectsType(sSubjectId))
		{
			strChineseName = UserInfoMgr::GetInstance()->GetChineseNameByType(sSubjectId);

			vSelectFieldShow.push_back(strChineseName);
			vSelectResultShow.push_back(strScore);
			cInvalidCount++;
		}
	}

	if (cInvalidCount != RecvMSG->bSubjectCount)
	{
		printf("%s ��Ŀ�����ʹ�����Ŀ������һ��\n", __FUNCTION__);
		SetIEndFlag(-1);
		return false;
	}
	//��ʾ��ѯ�ɼ�
	ShowSelectSingleScoreResult(vSelectFieldShow, vSelectResultShow, true);
	SetIEndFlag(1);

	return true;
}

bool SelectScoreSysProc::SelectBatchScoreRecvHandle(void* vpData, unsigned int DataLen)
{
	if (OPER_PER_SELECTBATCHSCOREBYONESUBJECT != GetCurOper() && OPER_PER_SELECTBATCHSCOREBYSUBJECTS != GetCurOper() )
	{
		printf("���ǽ��иò���[%d | %d]����ǰ���еĲ�����[%d] error\n", OPER_PER_SELECTBATCHSCOREBYONESUBJECT, OPER_PER_SELECTBATCHSCOREBYSUBJECTS,GetCurOper());
		SetIEndFlag(-1);
		return false;
	}
	if (DataLen != sizeof(SC_MSG_SELECT_BATCH_SCORE_ACK))
	{
		printf("DataLen[%u] error, It should be [%u]\n", DataLen, sizeof(SC_MSG_SELECT_BATCH_SCORE_ACK));
		SetIEndFlag(-1);
		return false;
	}

	SC_MSG_SELECT_BATCH_SCORE_ACK* RecvMSG = (SC_MSG_SELECT_BATCH_SCORE_ACK*) vpData;
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
	unsigned char iDataRecordCount = RecvMSG->bDataRecord[0];
	if (iDataRecordCount > MAXBATCHREQACKCOUNT)
	{
		printf("%s �������ݼ�¼����\n", __FUNCTION__);
		SetIEndFlag(-1);
		return false;
	}
	if (RecvMSG->bResCode == 0 && iDataRecordCount == 0 && RecvMSG->bDataRecord[2] == 1) //�������������һ���ǸպôﵽMAXBATCHREQACKCOUNT�ı�������¼�������Ϊ�˲���������־
	{
		SetIEndFlag(1);
		return true;
	}

	bool bShow = false;
	if (RecvMSG->bDataRecord[1] == 1)
	{
		bShow = true;
	}

	for (unsigned char i=0; i<iDataRecordCount; i++)
	{
		vector<string> vSelectFieldShow;
		vector<string> vSelectResultShow;

		if (RecvMSG->bRankFlag == 2) //����
		{
			vSelectFieldShow.push_back("����");
			vSelectResultShow.push_back(StringTool::NumberToStr((int)RecvMSG->sRank[i]));
		}

		vSelectFieldShow.push_back("ѧ��");
		vSelectResultShow.push_back(StringTool::NumberToStr((int)RecvMSG->uUserid[i]));
		
		vSelectFieldShow.push_back("����");
		vSelectResultShow.push_back(RecvMSG->cName[i]);

		vSelectFieldShow.push_back("�˺�");
		vSelectResultShow.push_back(RecvMSG->cAccount[i]);

		vSelectFieldShow.push_back("�༶");
		vSelectResultShow.push_back(RecvMSG->cGrade[i]);

		unsigned char cInvalidCount = 0;
		for (unsigned char j=0; j<RecvMSG->bSubjectCount; j++)
		{
			SubjectsType sSubjectId = (SubjectsType)RecvMSG->bSubjectId[i][j];
			string strChineseName;
			string strScore = StringTool::NumberToStr((int)RecvMSG->bScore[i][j]);
			if (UserInfoMgr::GetInstance()->CanFindSubjectsType(sSubjectId))
			{
				strChineseName = UserInfoMgr::GetInstance()->GetChineseNameByType(sSubjectId);

				vSelectFieldShow.push_back(strChineseName);
				vSelectResultShow.push_back(strScore);
				cInvalidCount++;
			}
		}
		if (cInvalidCount != RecvMSG->bSubjectCount)
		{
			printf("%s ��Ŀ�����ʹ�����Ŀ������һ��\n", __FUNCTION__);
			SetIEndFlag(-1);
			return false;
		}
		
		if (FindOneInArray(RecvMSG->cCondition, SELECT_SCORE_CONDITION_END-1, SELECT_SCORE_CONDITION_END-1, SELECT_SCORE_CONDITION_TOTAL))
		{
			vSelectFieldShow.push_back("�ܷ�");
			vSelectResultShow.push_back(StringTool::NumberToStr((int)RecvMSG->bSum[i]));
		}
		
		if (FindOneInArray(RecvMSG->cCondition, SELECT_SCORE_CONDITION_END-1, SELECT_SCORE_CONDITION_END-1, SELECT_SCORE_CONDITION_AVERAGE))
		{
			vSelectFieldShow.push_back("ƽ����");
			vSelectResultShow.push_back(StringTool::NumberToStr((int)RecvMSG->bAverage[i]));
		}

		//��ʾ��ѯ�ɼ�
		ShowSelectSingleScoreResult(vSelectFieldShow, vSelectResultShow, bShow);
		if (bShow)
		{
			bShow = false;
		}
	}

	//����
	if (RecvMSG->bDataRecord[2] == 1)
	{
		printf(">>�ܲ�ѯ��%u������\n", (RecvMSG->bDataRecord[1]-1)*MAXBATCHREQACKCOUNT + RecvMSG->bDataRecord[0]);
		SetIEndFlag(1);
	}

	return true;
}

int SelectScoreSysProc::ShowSubjects(map<int, string> mIStr, int iField)
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

void SelectScoreSysProc::ShowSelectSingleScoreResult(vector<string> vSelectFieldShow, vector<string> vSelectResultShow, bool bShow)
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

bool SelectScoreSysProc::FindOneInArray(unsigned char* pArray, unsigned char iCount, unsigned char iMaxCount, unsigned char Obj)
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

bool SelectScoreSysProc::CheckStringVaildRemoveSpl(string str, string strSpl, string strVaild)
{
	vector<string> vecStr = StringTool::Splite(str, strSpl);
	string strTmp;
	for (unsigned i=0; i<vecStr.size(); i++)
	{
		strTmp += vecStr.at(i);
	}
	
	return CheckTool::CheckStringByValid(strTmp, strVaild);
}