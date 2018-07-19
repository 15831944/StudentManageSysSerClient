#include "AlterSubjectsSysProc.h"
#include "ProcMgr.h"
#include "TCPHandle.h"
#include "UserInfoMgr.h"
#include "CheckTool.h"

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
	case ASSIST_ID_GET_CAN_ALTER_AFTER_ALTER_SUBJECTS_ACK:
	case ASSIST_ID_ALTER_SUBJECTS_ACK:
		bRes = AlterSubjectRecvHandle(vpData, DataLen);
		break;
	default:
		printf("%s iAssistId[%d] error\n", __FUNCTION__, iAssistId);
		ProcMgr::GetInstance()->ProcSwitch(GetMyProcDef(), true);
		break;
	}


// 	if (bRes)
// 		ProcMgr::GetInstance()->ProcSwitch(GetNextProc()); //����ɹ����л�����һ������
// 
	short sMark = UserInfoMgr::GetInstance()->GetUserMark();
	if (0 == sMark && !bRes)
	{
		EndRecv();
		ProcMgr::GetInstance()->ProcSwitch(GetMyProcDef(), true);
	}
	else if (-1 == sMark && !bRes)
	{
		UserInfoMgr::GetInstance()->SetUserMark(0); //���
	}
	else if((0 == sMark || 1002 == sMark ) && bRes)
	{
		UserInfoMgr::GetInstance()->SetUserMark(0); //���
		EndRecv();
		ProcMgr::GetInstance()->ProcSwitch(GetNextProc()); //����ɹ����л�����һ������
	}
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
		AddSubjectChooseHandle();
		break;
	case OPER_PER_ALTERDELETEONESCORESUBJECT:
		DeleteSubjectChooseHandle();
		break;
	default:
		cout<<"û�иò�����"<<endl;
		ProcMgr::GetInstance()->ProcSwitch(GetMyProcDef(), true);  
		break;
	}
}

void AlterSubjectsSysProc::AddSubjectChooseHandle()
{
	//����������ӵĿ�Ŀ
	CS_MSG_GET_CAN_ALTER_AFTER_ALTER_SUBJECTS_REQ SendReq;
	SendReq.sGetType = 1;

	UserInfoMgr::GetInstance()->SetUserMark(1000); //���

	TCPHandle::GetInstance()->Send(&SendReq, sizeof(SendReq), /*MAIN_ID_LOGINREGISTER,*/ ASSIST_ID_GET_CAN_ALTER_AFTER_ALTER_SUBJECTS_REQ);
}

void AlterSubjectsSysProc::DeleteSubjectChooseHandle()
{
	//�������ɾ���Ŀ�Ŀ
	CS_MSG_GET_CAN_ALTER_AFTER_ALTER_SUBJECTS_REQ SendReq;
	SendReq.sGetType = 2;

	UserInfoMgr::GetInstance()->SetUserMark(1000); //���

	TCPHandle::GetInstance()->Send(&SendReq, sizeof(SendReq), /*MAIN_ID_LOGINREGISTER,*/ ASSIST_ID_GET_CAN_ALTER_AFTER_ALTER_SUBJECTS_REQ);
}

bool AlterSubjectsSysProc::AlterSubjectRecvHandle(void* vpData, unsigned int DataLen)
{
	if (OPER_PER_ALTERADDONESCORESUBJECT != GetCurOper() && OPER_PER_ALTERDELETEONESCORESUBJECT != GetCurOper())
	{
		printf("���ǽ��иò���[%d | %d]����ǰ���еĲ�����[%d] error\n", OPER_PER_ALTERADDONESCORESUBJECT, OPER_PER_ALTERDELETEONESCORESUBJECT, GetCurOper());
		UserInfoMgr::GetInstance()->SetUserMark(0); //���
		return false;
	}

	short sMark = UserInfoMgr::GetInstance()->GetUserMark();
	if (1000 == sMark) //����ɾ��Ŀ����
	{
		if (DataLen != sizeof(CS_MSG_GET_CAN_ALTER_AFTER_ALTER_SUBJECTS_ACK))
		{
			printf("DataLen[%u] error, It should be [%u]\n", DataLen, sizeof(CS_MSG_GET_CAN_ALTER_AFTER_ALTER_SUBJECTS_ACK));
			UserInfoMgr::GetInstance()->SetUserMark(0); //���
			return false;
		}
		CS_MSG_GET_CAN_ALTER_AFTER_ALTER_SUBJECTS_ACK* RecvMSG = (CS_MSG_GET_CAN_ALTER_AFTER_ALTER_SUBJECTS_ACK*) vpData;
		if (RecvMSG->bSucceed)
		{
			vector<string> vecStrTmp = CheckTool::Splite(RecvMSG->cCanAlterSubjects, "|");
			map<int, string> mStrShowTmp;
 			for (unsigned i=0; i<vecStrTmp.size(); i++)
			{
				int sId = (int)atoi(vecStrTmp.at(i).c_str());
				if (UserInfoMgr::GetInstance()->CanFindSubjectsType((SubjectsType)sId))
				{
					mStrShowTmp.insert(pair<int, string>(sId, UserInfoMgr::GetInstance()->GetChineseNameByType((SubjectsType)sId)));
				}
			}

			if (RecvMSG->sGetType == 1)
			{
				if (mStrShowTmp.empty())
				{
					printf("û�п����ӵĿ�Ŀ\n");
					UserInfoMgr::GetInstance()->SetUserMark(0); //���
					return true;
				}

				printf("��ѡ��Ҫ���ӵĿ�ĿID��\n");
				ShowSubjects(mStrShowTmp);

				string iChooseTmp;
				cin>>iChooseTmp;
				if (!CheckTool::CheckStringByValid(iChooseTmp, "0~9") || mStrShowTmp.find((int)atoi(iChooseTmp.c_str())) == mStrShowTmp.end())
				{
					UserInfoMgr::GetInstance()->SetUserMark(-1); //���
					OperInputErrorHandle();
					return false;
				}
				else
				{
					if (0 != m_iOperInputErrorLimit)
						m_iOperInputErrorLimit = 0;

					UserInfoMgr::GetInstance()->SetUserMark(1001); //���

					CS_MSG_ALTER_SUBJECTS_REQ SendReq;
					SendReq.sGetType = 1;
					SendReq.sSubjectId = (short)atoi(iChooseTmp.c_str());
					TCPHandle::GetInstance()->Send(&SendReq, sizeof(SendReq), /*MAIN_ID_LOGINREGISTER,*/ ASSIST_ID_ALTER_SUBJECTS_REQ);
				}
			}
			else if(RecvMSG->sGetType == 2)
			{
				if (mStrShowTmp.empty())
				{
					printf("û�п�ɾ���Ŀ�Ŀ\n");
					UserInfoMgr::GetInstance()->SetUserMark(0); //���
					return true;
				}

				printf("��ѡ��Ҫɾ���Ŀ�ĿID��\n");
				ShowSubjects(mStrShowTmp);

				string iChooseTmp;
				cin>>iChooseTmp;
				if (!CheckTool::CheckStringByValid(iChooseTmp, "0~9") || mStrShowTmp.find((int)atoi(iChooseTmp.c_str())) == mStrShowTmp.end())
				{
					UserInfoMgr::GetInstance()->SetUserMark(-1); //���
					OperInputErrorHandle();
					return false;
				}
				else
				{
					if (0 != m_iOperInputErrorLimit)
						m_iOperInputErrorLimit = 0;

					UserInfoMgr::GetInstance()->SetUserMark(1001); //���

					CS_MSG_ALTER_SUBJECTS_REQ SendReq;
					SendReq.sGetType = 2;
					SendReq.sSubjectId = (short)atoi(iChooseTmp.c_str());
					TCPHandle::GetInstance()->Send(&SendReq, sizeof(SendReq), /*MAIN_ID_LOGINREGISTER,*/ ASSIST_ID_ALTER_SUBJECTS_REQ);
				}
			}
			else
			{
				printf("%s  RecvMSG->sGetType=[%d] error\n", __FUNCTION__, (int)RecvMSG->sGetType);
				UserInfoMgr::GetInstance()->SetUserMark(0); //���
				return false;
			}

		}
		else
		{
			UserInfoMgr::GetInstance()->SetUserMark(0); //���
			printf("����ɾ��Ŀ���ز��ɹ�");
			return false;
		}

	}
	else if (1001 == sMark)
	{
		if (DataLen != sizeof(CS_MSG_ALTER_SUBJECTS_ACK))
		{
			printf("DataLen[%u] error, It should be [%u]\n", DataLen, sizeof(CS_MSG_ALTER_SUBJECTS_ACK));
			UserInfoMgr::GetInstance()->SetUserMark(0); //���
			return false;
		}
		CS_MSG_ALTER_SUBJECTS_ACK* RecvMSG = (CS_MSG_ALTER_SUBJECTS_ACK*) vpData;
		if (RecvMSG->bSucceed)
		{
			string sChineseName = UserInfoMgr::GetInstance()->GetChineseNameByType((SubjectsType)RecvMSG->sSubjectId);
			if (RecvMSG->sGetType == 1)
			{
				printf("���ӿ�Ŀ[%s]�ɹ�\n", sChineseName.c_str());
				UserInfoMgr::GetInstance()->SetUserMark(1002); //��ǣ��������
				return true;
			}
			else if (RecvMSG->sGetType == 2)
			{
				printf("ɾ����Ŀ[%s]�ɹ�\n", sChineseName.c_str());
				UserInfoMgr::GetInstance()->SetUserMark(1002); //��ǣ��������
				return true;
			}
			else
			{
				printf("%s  RecvMSG->sGetType=[%d] error\n", __FUNCTION__, (int)RecvMSG->sGetType);
				UserInfoMgr::GetInstance()->SetUserMark(0); //���
				return false;
			}
		}
		else
		{
			UserInfoMgr::GetInstance()->SetUserMark(0); //���
			printf("��ɾ��Ŀ����ʧ��");
			return false;
		}
	}
	else
	{

	}

	return true;
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