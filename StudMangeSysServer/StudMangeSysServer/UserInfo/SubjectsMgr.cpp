#include "SubjectsMgr.h"
#include "MysqlMgr.h"
#include "CheckTool.h"
#include "LockTools.h"

SubjectsMgr::SubjectsMgr()
{
	init();
}

SubjectsMgr::~SubjectsMgr()
{

}

void SubjectsMgr::init()
{
	m_mAllSubjects.insert(pair<SubjectsType, SubjectsData>(SUBJECTS_TYPE_CHINESE, SubjectsData("chinese", "����")));
	m_mAllSubjects.insert(pair<SubjectsType, SubjectsData>(SUBJECTS_TYPE_MATH, SubjectsData("math", "��ѧ")));
	m_mAllSubjects.insert(pair<SubjectsType, SubjectsData>(SUBJECTS_TYPE_ENGLISH, SubjectsData("english", "Ӣ��")));
	m_mAllSubjects.insert(pair<SubjectsType, SubjectsData>(SUBJECTS_TYPE_PHYSICS, SubjectsData("physics", "����")));
	m_mAllSubjects.insert(pair<SubjectsType, SubjectsData>(SUBJECTS_TYPE_CHEMISTRY, SubjectsData("chemistry", "��ѧ")));
	m_mAllSubjects.insert(pair<SubjectsType, SubjectsData>(SUBJECTS_TYPE_BIOLOGY, SubjectsData("biology", "����")));
	m_mAllSubjects.insert(pair<SubjectsType, SubjectsData>(SUBJECTS_TYPE_HISTORY, SubjectsData("history", "��ʷ")));
	m_mAllSubjects.insert(pair<SubjectsType, SubjectsData>(SUBJECTS_TYPE_POLITICS, SubjectsData("politics", "����")));
	m_mAllSubjects.insert(pair<SubjectsType, SubjectsData>(SUBJECTS_TYPE_GEOGRAPHY, SubjectsData("geography", "����")));
}

string SubjectsMgr::GetStrEnglishNameByStrType(string strType, string strSpl)
{
	if (strSpl.empty())
	{
		return strType;
	}

	vector<string> vStrType = CheckTool::Splite(strType, strSpl);
	vector<string> vStrRes;
	for (unsigned i=0; i<vStrType.size(); i++)
	{
		string sEName = GetEnglishNameByType((SubjectsType)atoi(vStrType.at(i).c_str()));
		if (!sEName.empty())
		{
			vStrRes.push_back(sEName);
		}
	}

	string sRes = CheckTool::CombVecToStr(vStrRes, strSpl);
	return sRes;
}

string SubjectsMgr::GetEnglishNameByType(SubjectsType sType)
{
	if (sType <= SUBJECTS_TYPE_START || sType >= SUBJECTS_TYPE_END)
	{
		return "";
	}

	return m_mAllSubjects[sType].strEnglishName;
}

string SubjectsMgr::GetChineseNameByType(SubjectsType sType)
{
	if (sType <= SUBJECTS_TYPE_START || sType >= SUBJECTS_TYPE_END)
	{
		return "";
	}

	return m_mAllSubjects[sType].strChineseName;
}

SubjectsType SubjectsMgr::GetTypeByEnglishName(string strName)
{
	map<SubjectsType, SubjectsData>::iterator iter = m_mAllSubjects.begin();
	for (; iter != m_mAllSubjects.end(); iter++)
	{
		if (CheckTool::ToLowercase(iter->second.strEnglishName) == CheckTool::ToLowercase(strName))
		{
			return iter->first;
		}
	}

	return SUBJECTS_TYPE_INVALID;
}

SubjectsType SubjectsMgr::GetTypeByChineseName(string strName)
{
	map<SubjectsType, SubjectsData>::iterator iter = m_mAllSubjects.begin();
	for (; iter != m_mAllSubjects.end(); iter++)
	{
		if (CheckTool::ToLowercase(iter->second.strChineseName) == CheckTool::ToLowercase(strName))
		{
			return iter->first;
		}
	}

	return SUBJECTS_TYPE_INVALID;
}

vector<SubjectsType> SubjectsMgr::GetVExistSubjects()
{
	return m_vExistSubjects;
}

vector<SubjectsType> SubjectsMgr::GetVNotExistSubjects()
{
	vector<SubjectsType> vExistSubjectsTmp;

	map<SubjectsType, SubjectsData>::iterator mIter = m_mAllSubjects.begin();
	for (; mIter != m_mAllSubjects.end(); mIter++)
	{
		bool bHave = false;
		for (vector<SubjectsType>::iterator vIter = m_vExistSubjects.begin(); vIter != m_vExistSubjects.end(); vIter++)
		{
			if (mIter->first == *vIter)
			{
				bHave = true;
				break;
			}
		}

		if (!bHave)
		{
			vExistSubjectsTmp.push_back(mIter->first);
		}
	}

	return vExistSubjectsTmp;
}

bool SubjectsMgr::DeleteOneExistSubject(SubjectsType sType)
{
	if (sType <= SUBJECTS_TYPE_START || sType >= SUBJECTS_TYPE_END || m_mAllSubjects.find(sType) == m_mAllSubjects.end())
	{
		return false;
	}

	for (vector<SubjectsType>::iterator vIter = m_vExistSubjects.begin(); vIter != m_vExistSubjects.end(); vIter++)
	{
		if (sType == *vIter)
		{
			ExistSubjectsLock::GetInstance()->Lock();
			m_vExistSubjects.erase(vIter);
			ExistSubjectsLock::GetInstance()->Unlock();
			break;
		}
	}

	//����ԭ������������ɾ���ģ�������true����Ϊ����ȷʵû����
	return true;
}

bool SubjectsMgr::AddOneExistSubject(SubjectsType sType)
{
	if (sType <= SUBJECTS_TYPE_START || sType >= SUBJECTS_TYPE_END || m_mAllSubjects.find(sType) == m_mAllSubjects.end())
	{
		return false;
	}
	
	bool isHave = false;
	for (vector<SubjectsType>::iterator vIter = m_vExistSubjects.begin(); vIter != m_vExistSubjects.end(); vIter++)
	{
		if (sType == *vIter)
		{
			isHave = true;
			break;
		}
	}

	if (!isHave)
	{
		ExistSubjectsLock::GetInstance()->Lock();
		m_vExistSubjects.push_back(sType);
		ExistSubjectsLock::GetInstance()->Unlock();
	}

	return true;
}

void SubjectsMgr::GetExistSubjectHandle()
{
	char strMysql[512];
	memset(strMysql, 0, sizeof(strMysql));
	sprintf_s(strMysql, sizeof(strMysql), "select * from studScore where userID=%u", 10000);

	MysqlMgr::GetInstance()->InputMsgQueue(strMysql, MYSQL_OPER_SELECT, ASSIST_ID_SPECIAL_GET_EXIAT_SUBJECTS, 0);
}

void SubjectsMgr::GetExistSubjectReplyHandle(SOCKET SocketId, MYSQL_RES *MysqlRes, string strRecord)
{
	vector<string> vStrRecord= CheckTool::Splite(strRecord);
	if (NULL == MysqlRes || (vStrRecord.size() > 0 && (int)atoi(vStrRecord.at(0).c_str()) != 0))
	{
		printf("%s  ���ݿ����ִ��ʧ��\n", __FUNCTION__);
		return;
	}
	
	//û��select��¼��Ҳ�᷵��field����

	int j = mysql_num_fields(MysqlRes);
	if (j > 0)
	{
		MYSQL_FIELD *fd;
		char column[32];

		ExistSubjectsLock::GetInstance()->Lock();
		for(int i = 0; fd = mysql_fetch_field(MysqlRes);i++)
		{
			memset(column, 0, sizeof(column));
			strcpy_s(column, sizeof(column), fd->name);
			string str = CheckTool::ToLowercase(string(column));
			if (str == string("userid")) //���˲���Ҫ���ֶ�
			{
				continue;
			}
			
			SubjectsType sType = GetTypeByEnglishName(str);
			if (SUBJECTS_TYPE_INVALID != sType)
			{
				m_vExistSubjects.push_back(sType);
			}
		}
		ExistSubjectsLock::GetInstance()->Unlock();

		printf("%s  ��ȡ�Ѵ��ڿ�Ŀ�ɹ���\n", __FUNCTION__);
	}
	else
	{
		printf("%s  error������С��1\n", __FUNCTION__);
	}
}