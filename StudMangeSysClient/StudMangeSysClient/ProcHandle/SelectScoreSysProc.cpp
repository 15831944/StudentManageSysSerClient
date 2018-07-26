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
		printf("û�п�Ŀ��������ӿ�Ŀ\n");
		SetIEndFlag(1);
		return;
	}

	cout<<"�����뱻��ѯ�ɼ�������û�����"<<endl;
	string strAccount;
	cin>>strAccount;
	if (!(CheckTool::CheckStringLen(strAccount, 30) && CheckTool::CheckStringByValid(strAccount, "A~Z|a~z|0~9")))
	{
		OperInputErrorHandle(false);
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
		printf("û�п�Ŀ��������ӿ�Ŀ\n");
		SetIEndFlag(1);
		return;
	}

	cout<<"�����뱻��ѯ�ɼ�������û�����"<<endl;
	string strAccount;
	cin>>strAccount;
	if (!(CheckTool::CheckStringLen(strAccount, 30) && CheckTool::CheckStringByValid(strAccount, "A~Z|a~z|0~9")))
	{
		OperInputErrorHandle(false);
		return;
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

}

void SelectScoreSysProc::SelectBatchScoreBySubjectsChooseHandle(char* pStrExistSubjects)
{

}

bool SelectScoreSysProc::GetSubjectsAfterSelectScoreRecvHandle(void* vpData, unsigned int DataLen)
{

	if (OPER_PER_SELECTSINGLESCOREBYONESUBJECT != GetCurOper() && OPER_PER_SELECTSINGLESCOREBYSUBJECTS != GetCurOper() && OPER_PER_SELECTBATCHSCOREBYONESUBJECT != GetCurOper() && OPER_PER_SELECTBATCHSCOREBYSUBJECTS != GetCurOper())
	{
		printf("���ǽ��иò���[%d | %d | %d | %d]����ǰ���еĲ�����[%d] error\n", OPER_PER_SELECTSINGLESCOREBYONESUBJECT, OPER_PER_SELECTSINGLESCOREBYSUBJECTS, OPER_PER_SELECTBATCHSCOREBYONESUBJECT, OPER_PER_SELECTBATCHSCOREBYSUBJECTS,GetCurOper());
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
	if (DataLen != sizeof(CS_MSG_SELECT_SINGLE_SCORE_ACK))
	{
		printf("DataLen[%u] error, It should be [%u]\n", DataLen, sizeof(CS_MSG_SELECT_SINGLE_SCORE_ACK));
		SetIEndFlag(-1);
		return false;
	}

	CS_MSG_SELECT_SINGLE_SCORE_ACK* RecvMSG = (CS_MSG_SELECT_SINGLE_SCORE_ACK*) vpData;

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
	ShowSelectSingleScoreResult(vSelectFieldShow, vSelectResultShow, true, true);
	SetIEndFlag(1);

	return true;
}

bool SelectScoreSysProc::SelectBatchScoreRecvHandle(void* vpData, unsigned int DataLen)
{

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

void SelectScoreSysProc::ShowSelectSingleScoreResult(vector<string> vSelectFieldShow, vector<string> vSelectResultShow, bool bShowDescribe, bool bShowLine)
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

	if (bShowLine)
		printf("-------------------------------\n");
	if (bShowDescribe)
		printf("��ѯ�����ʾ��\n");

	cout<<strFieldShow<<endl;
	cout<<strResult<<endl;
}