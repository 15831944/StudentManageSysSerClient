#include "AlterSubjectsSysProc.h"
#include "ProcMgr.h"
#include "TCPHandle.h"
#include "UserInfoMgr.h"
#include "CheckTool.h"
#include "StringTool.h"

AlterSubjectsSysProc::AlterSubjectsSysProc(ProcDef nProcDef) : BaseProc(nProcDef)
{
	initMapChoose();
}

AlterSubjectsSysProc::~AlterSubjectsSysProc()
{

}

bool AlterSubjectsSysProc::initMapChoose()
{
	m_mChoose.insert(pair<int, ChooseData>(GetMaxRealChoose(), ChooseData("���ӳɼ���Ŀ", OPER_PER_ALTERADDONESCORESUBJECT, PROC_DEF_ALTERSUBJECTSSYSPROC)));
	m_mChoose.insert(pair<int, ChooseData>(++GetMaxRealChoose(), ChooseData("ɾ���ɼ���Ŀ", OPER_PER_ALTERDELETEONESCORESUBJECT, PROC_DEF_ALTERSUBJECTSSYSPROC)));
	m_mChoose.insert(pair<int, ChooseData>(++GetMaxRealChoose(), ChooseData("����", OPER_PER_INVALID, PROC_DEF_SCORESYSPROC)));

	return true;
}

void AlterSubjectsSysProc::EndProc()
{
	__super::EndProc();
}

void AlterSubjectsSysProc::StartRecv(void* vpData, unsigned int DataLen, /*int iMianId,*/ int iAssistId)
{
	bool bRes = false;
	switch(iAssistId)
	{
	case ASSIST_ID_GET_SUBJECTS_ACK:
		bRes = GetSubjectsAfterAlterSubjectRecvHandle(vpData, DataLen);
		break;
	case ASSIST_ID_ALTER_SUBJECTS_ACK:
		bRes = AlterSubjectRecvHandle(vpData, DataLen);
		break;
	default:
		printf("%s iAssistId[%d] error\n", __FUNCTION__, iAssistId);
		//ProcMgr::GetInstance()->ProcSwitch(GetMyProcDef(), true);
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

void AlterSubjectsSysProc::EndRecv()
{
	__super::EndRecv();
}

void AlterSubjectsSysProc::SwitchToOper(OperPermission CurOper)
{
	__super::SwitchToOper(CurOper);

	switch(CurOper)
	{
	case OPER_PER_ALTERADDONESCORESUBJECT:
		GetSubjectsAfterAddSubjectChooseHandle();
		break;
	case OPER_PER_ALTERDELETEONESCORESUBJECT:
		GetSubjectsAfterDeleteSubjectChooseHandle();
		break;
	default:
		printf("%s û�иò�����\n", __FUNCTION__);
		ProcMgr::GetInstance()->ProcSwitch(GetMyProcDef(), true);  
		break;
	}
}

void AlterSubjectsSysProc::GetSubjectsAfterAddSubjectChooseHandle()
{
	//����������ӵĿ�Ŀ
	CS_MSG_GET_SUBJECTS_REQ SendReq;
	SendReq.sGetType = 1;


	TCPHandle::GetInstance()->Send(&SendReq, sizeof(SendReq), /*MAIN_ID_LOGINREGISTER,*/ ASSIST_ID_GET_SUBJECTS_REQ);
}

void AlterSubjectsSysProc::GetSubjectsAfterDeleteSubjectChooseHandle()
{
	//�������ɾ���Ŀ�Ŀ
	CS_MSG_GET_SUBJECTS_REQ SendReq;
	SendReq.sGetType = 2;


	TCPHandle::GetInstance()->Send(&SendReq, sizeof(SendReq), /*MAIN_ID_LOGINREGISTER,*/ ASSIST_ID_GET_SUBJECTS_REQ);
}

void AlterSubjectsSysProc::AddSubjectChooseHandle(char* pCanAlterSubjects)
{
	if (NULL == pCanAlterSubjects)
	{
		printf("%s ���ؿ����ӿ�ĿΪNULL\n", __FUNCTION__);
		SetIEndFlag(-1);
		return;
	}

	vector<string> vecStrTmp = StringTool::Splite(pCanAlterSubjects, "|");
	map<int, string> mStrShowTmp; //��ʾ�Ŀ�Ŀ
	for (unsigned i=0; i<vecStrTmp.size(); i++)
	{
		int sId = (int)atoi(vecStrTmp.at(i).c_str());
		if (UserInfoMgr::GetInstance()->CanFindSubjectsType((SubjectsType)sId))
		{
			mStrShowTmp.insert(pair<int, string>(sId, UserInfoMgr::GetInstance()->GetChineseNameByType((SubjectsType)sId)));
		}
	}

	if (mStrShowTmp.empty())
	{
		printf("û�п����ӵĿ�Ŀ\n");
		SetIEndFlag(1);
		return;
	}

	printf("��ѡ��Ҫ���ӵĿ�ĿID��\n");
	ShowSubjects(mStrShowTmp);
	
	string iChooseTmp;
	cin>>iChooseTmp;
	if (!CheckTool::CheckStringByValid(iChooseTmp, "0~9") || mStrShowTmp.find((int)atoi(iChooseTmp.c_str())) == mStrShowTmp.end())
	{
		OperInputErrorHandle(false);
		return;
	}
	else
	{
		if (0 != m_iOperInputErrorLimit)
			m_iOperInputErrorLimit = 0;


		CS_MSG_ALTER_SUBJECTS_REQ SendReq;
		SendReq.sGetType = 1;
		SendReq.sSubjectId = (short)atoi(iChooseTmp.c_str());
		TCPHandle::GetInstance()->Send(&SendReq, sizeof(SendReq), /*MAIN_ID_LOGINREGISTER,*/ ASSIST_ID_ALTER_SUBJECTS_REQ);
	}
}

void AlterSubjectsSysProc::DeleteSubjectChooseHandle(char* pCanAlterSubjects)
{
	if (NULL == pCanAlterSubjects)
	{
		printf("%s ���ؿ�ɾ����ĿΪNULL\n", __FUNCTION__);
		SetIEndFlag(-1);
		return;
	}

	vector<string> vecStrTmp = StringTool::Splite(pCanAlterSubjects, "|");
	map<int, string> mStrShowTmp; //��ʾ�Ŀ�Ŀ
	for (unsigned i=0; i<vecStrTmp.size(); i++)
	{
		int sId = (int)atoi(vecStrTmp.at(i).c_str());
		if (UserInfoMgr::GetInstance()->CanFindSubjectsType((SubjectsType)sId))
		{
			mStrShowTmp.insert(pair<int, string>(sId, UserInfoMgr::GetInstance()->GetChineseNameByType((SubjectsType)sId)));
		}
	}

	if (mStrShowTmp.empty())
	{
		printf("û�п�ɾ���Ŀ�Ŀ\n");
		SetIEndFlag(1);
		return;
	}

	printf("��ѡ��Ҫɾ���Ŀ�ĿID��\n");
	ShowSubjects(mStrShowTmp);
	
	string iChooseTmp;
	cin>>iChooseTmp;
	if (!CheckTool::CheckStringByValid(iChooseTmp, "0~9") || mStrShowTmp.find((int)atoi(iChooseTmp.c_str())) == mStrShowTmp.end())
	{
		OperInputErrorHandle(false);
		return;
	}
	else
	{
		if (0 != m_iOperInputErrorLimit)
			m_iOperInputErrorLimit = 0;


		CS_MSG_ALTER_SUBJECTS_REQ SendReq;
		SendReq.sGetType = 2;
		SendReq.sSubjectId = (short)atoi(iChooseTmp.c_str());
		TCPHandle::GetInstance()->Send(&SendReq, sizeof(SendReq), /*MAIN_ID_LOGINREGISTER,*/ ASSIST_ID_ALTER_SUBJECTS_REQ);
	}
}

bool AlterSubjectsSysProc::GetSubjectsAfterAlterSubjectRecvHandle(void* vpData, unsigned int DataLen)
{
	if (OPER_PER_ALTERADDONESCORESUBJECT != GetCurOper() && OPER_PER_ALTERDELETEONESCORESUBJECT != GetCurOper())
	{
		printf("���ǽ��иò���[%d | %d]����ǰ���еĲ�����[%d] error\n", OPER_PER_ALTERADDONESCORESUBJECT, OPER_PER_ALTERDELETEONESCORESUBJECT, GetCurOper());
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
		if (RecvMSG->sGetType == 1)
		{
			AddSubjectChooseHandle(RecvMSG->cCanAlterSubjects);
		}
		else if(RecvMSG->sGetType == 2)
		{
			DeleteSubjectChooseHandle(RecvMSG->cCanAlterSubjects);
		}
		else
		{
			printf("%s  RecvMSG->sGetType=[%d] error\n", __FUNCTION__, (int)RecvMSG->sGetType);
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

bool AlterSubjectsSysProc::AlterSubjectRecvHandle(void* vpData, unsigned int DataLen)
{
	if (OPER_PER_ALTERADDONESCORESUBJECT != GetCurOper() && OPER_PER_ALTERDELETEONESCORESUBJECT != GetCurOper())
	{
		printf("���ǽ��иò���[%d | %d]����ǰ���еĲ�����[%d] error\n", OPER_PER_ALTERADDONESCORESUBJECT, OPER_PER_ALTERDELETEONESCORESUBJECT, GetCurOper());
		SetIEndFlag(-1);
		return false;
	}
	if (DataLen != sizeof(CS_MSG_ALTER_SUBJECTS_ACK))
	{
		printf("DataLen[%u] error, It should be [%u]\n", DataLen, sizeof(CS_MSG_ALTER_SUBJECTS_ACK));
		SetIEndFlag(-1);
		return false;
	}

	CS_MSG_ALTER_SUBJECTS_ACK* RecvMSG = (CS_MSG_ALTER_SUBJECTS_ACK*) vpData;
	if (RecvMSG->bSucceed)
	{
		string sChineseName = UserInfoMgr::GetInstance()->GetChineseNameByType((SubjectsType)RecvMSG->sSubjectId);
		if (RecvMSG->sGetType == 1)
		{
			printf("���ӿ�Ŀ[%s]�ɹ�\n", sChineseName.c_str());
			SetIEndFlag(1);
			return true;
		}
		else if (RecvMSG->sGetType == 2)
		{
			printf("ɾ����Ŀ[%s]�ɹ�\n", sChineseName.c_str());
			SetIEndFlag(1);
			return true;
		}
		else
		{
			printf("%s  RecvMSG->sGetType=[%d] error\n", __FUNCTION__, (int)RecvMSG->sGetType);
			SetIEndFlag(-1);
			return false;
		}
	}
	else
	{
		SetIEndFlag(-1);
		printf("��ɾ��Ŀ����ʧ��");
		return false;
	}
}

int AlterSubjectsSysProc::ShowSubjects(map<int, string> mIStr)
{
	int iCount = 0;
	for (map<int, string>::iterator iter = mIStr.begin(); iter != mIStr.end(); iter++)
	{
		if (iCount != 0 && iCount % 2 == 0)
		{
			cout<<endl;
		}
		if (iCount % 2 == 1)
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