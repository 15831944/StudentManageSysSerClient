#include "UserInfoMgr.h"
#include "LockTools.h"
#include "StringTool.h"

UserInfoMgr::UserInfoMgr()
{
	initAllOperPer();

	m_maxUserInfoFieldID = 0;
	InitUserInfoFieldName();
}

UserInfoMgr::~UserInfoMgr()
{

}

void UserInfoMgr::initAllOperPer()
{
	m_mAllOperPer.insert(pair<OperPermission, string>(OPER_PER_LOGIN, "��¼Ȩ��"));
	m_mAllOperPer.insert(pair<OperPermission, string>(OPER_PER_REGISTER, "ע��Ȩ��"));

	m_mAllOperPer.insert(pair<OperPermission, string>(OPER_PER_ADDBATCHSCOREBYONESUBJECT, "�����������ӳɼ�"));
	m_mAllOperPer.insert(pair<OperPermission, string>(OPER_PER_ADDBATCHSCOREBYSUBJECTS, "�������п�Ŀ"));
	m_mAllOperPer.insert(pair<OperPermission, string>(OPER_PER_ADDSINGLESCOREBYONESUBJECT, "���Ƶ������ӳɼ�"));
	m_mAllOperPer.insert(pair<OperPermission, string>(OPER_PER_ADDSINGLESCOREBYSUBJECTS, "�������п�Ŀ�������ӳɼ�"));
	//m_mAllOperPer.insert(pair<OperPermission, string>(OPER_PER_UPDATEBATCHSCOREBYONESUBJECT, "�����������ĳɼ�"));
	//m_mAllOperPer.insert(pair<OperPermission, string>(OPER_PER_UPDATEBATCHSCOREBYSUBJECTS, "�������п�Ŀ�������ĳɼ�"));
	m_mAllOperPer.insert(pair<OperPermission, string>(OPER_PER_UPDATESINGLESCOREBYONESUBJECT, "���Ƶ������ĳɼ�"));
	m_mAllOperPer.insert(pair<OperPermission, string>(OPER_PER_UPDATESINGLESCOREBYSUBJECTS, "�������п�Ŀ�������ĳɼ�"));
	m_mAllOperPer.insert(pair<OperPermission, string>(OPER_PER_SELECTBATCHSCOREBYONESUBJECT, "����������ѯ�ɼ�"));
	m_mAllOperPer.insert(pair<OperPermission, string>(OPER_PER_SELECTBATCHSCOREBYSUBJECTS, "�������п�Ŀ������ѯ�ɼ�"));
	m_mAllOperPer.insert(pair<OperPermission, string>(OPER_PER_SELECTSINGLESCOREBYONESUBJECT, "���Ƶ�����ѯ�ɼ�"));
	m_mAllOperPer.insert(pair<OperPermission, string>(OPER_PER_SELECTSINGLESCOREBYSUBJECTS, "�������п�Ŀ������ѯ�ɼ�"));
	m_mAllOperPer.insert(pair<OperPermission, string>(OPER_PER_DELETEBATCHSCORE, "����ɾ���ɼ�"));
	m_mAllOperPer.insert(pair<OperPermission, string>(OPER_PER_DELETESINGLESCORE, "����ɾ���ɼ�"));
	m_mAllOperPer.insert(pair<OperPermission, string>(OPER_PER_ALTERADDONESCORESUBJECT, "���ӳɼ���Ŀ"));
	m_mAllOperPer.insert(pair<OperPermission, string>(OPER_PER_ALTERDELETEONESCORESUBJECT, "ɾ���ɼ���Ŀ"));

	m_mAllOperPer.insert(pair<OperPermission, string>(OPER_PER_ADDBATCHUSERINFO, "���������û���Ϣ"));
	m_mAllOperPer.insert(pair<OperPermission, string>(OPER_PER_ADDSINGLEUSERINFO, "���������û���Ϣ"));
	//m_mAllOperPer.insert(pair<OperPermission, string>(OPER_PER_UPDATEBATCHUSERINFOBYONE, "�����ֶ����������û���Ϣ"));
	//m_mAllOperPer.insert(pair<OperPermission, string>(OPER_PER_UPDATEBATCHUSERINFOBYMORE, "���̶ֹ��ֶ����������û���Ϣ"));
	m_mAllOperPer.insert(pair<OperPermission, string>(OPER_PER_UPDATESINGLEUSERINFOEBYONE, "�����ֶε��������û���Ϣ"));
	m_mAllOperPer.insert(pair<OperPermission, string>(OPER_PER_UPDATESINGLEUSERINFOBYMORE, "���̶ֹ��ֶε��������û���Ϣ"));
	m_mAllOperPer.insert(pair<OperPermission, string>(OPER_PER_SELECTBATCHUSERINFO, "������ѯ�û���Ϣ"));
	m_mAllOperPer.insert(pair<OperPermission, string>(OPER_PER_SELECTSINGLEUSERINFO, "������ѯ�û���Ϣ"));
	m_mAllOperPer.insert(pair<OperPermission, string>(OPER_PER_DELETEBATCHUSERINFO, "����ɾ���û���Ϣ"));
	m_mAllOperPer.insert(pair<OperPermission, string>(OPER_PER_DELETESINGLEUSERINFO, "����ɾ���û���Ϣ"));

	m_mAllOperPer.insert(pair<OperPermission, string>(OPER_PER_ADDAUTHORITY, "����ĳ�û�һ�ֻ��߶���Ȩ��"));
	m_mAllOperPer.insert(pair<OperPermission, string>(OPER_PER_DELETEAUTHORITY, "ɾ��ĳ�û�һ�ֻ��߶���Ȩ��"));
}

void UserInfoMgr::InitUserInfoFieldName()
{
	m_mUserInfoFieldName.insert(pair<int, string>(++m_maxUserInfoFieldID, "account|�˺�"));
	m_mUserInfoFieldName.insert(pair<int, string>(++m_maxUserInfoFieldID, "password|����"));
	m_mUserInfoFieldName.insert(pair<int, string>(++m_maxUserInfoFieldID, "name|����"));
	m_mUserInfoFieldName.insert(pair<int, string>(++m_maxUserInfoFieldID, "sex|�Ա�"));
	m_mUserInfoFieldName.insert(pair<int, string>(++m_maxUserInfoFieldID, "Ident|��ݱ�ʶ"));
	m_mUserInfoFieldName.insert(pair<int, string>(++m_maxUserInfoFieldID, "major|רҵ"));
	m_mUserInfoFieldName.insert(pair<int, string>(++m_maxUserInfoFieldID, "grade|�༶"));
}

bool UserInfoMgr::InsertInfo(unsigned __int64 socketId, UserInfo userInfo)
{
	map<unsigned __int64, UserInfo>::iterator iter = m_mUserInfo.find(socketId);
	if (iter == m_mUserInfo.end())
	{
		UserInfoLock::GetInstance()->Lock();
		m_mUserInfo.insert(pair<unsigned __int64, UserInfo>(socketId, userInfo));
		UserInfoLock::GetInstance()->Unlock();

		return true;
	}
	
	return false;
}

bool UserInfoMgr::InsertInfo(unsigned __int64 socketId, string strIP, unsigned short sPort, PER_HANDLE_DATA* PerHandleData)
{
	map<unsigned __int64, UserInfo>::iterator iter = m_mUserInfo.find(socketId);
	if (iter == m_mUserInfo.end())
	{
		UserInfo userInfo;
		userInfo.strIP = strIP;
		userInfo.sPort = sPort;
		userInfo.PerHandleData = PerHandleData;
		
		UserInfoLock::GetInstance()->Lock();
		m_mUserInfo.insert(pair<unsigned __int64, UserInfo>(socketId, userInfo));
		UserInfoLock::GetInstance()->Unlock();

		printf("%s  socketId[%llu]  strIP[%s]  sPort[%d]\n", __FUNCTION__, socketId, strIP.c_str(), sPort);

		return true;
	}

	return false;
}

bool UserInfoMgr::RemoveInfoBySocketId(unsigned __int64 socketId)
{
	map<unsigned __int64, UserInfo>::iterator iter = m_mUserInfo.find(socketId);
	if (iter != m_mUserInfo.end())
	{
		UserInfoLock::GetInstance()->Lock();

		closesocket((SOCKET)iter->first);
		if (NULL != iter->second.PerHandleData)
			GlobalFree(iter->second.PerHandleData);
		if (NULL != iter->second.PerIoData)
			GlobalFree(iter->second.PerIoData);

		m_mUserInfo.erase(iter);

		UserInfoLock::GetInstance()->Unlock();

		printf("%s  socketId[%llu]\n", __FUNCTION__, socketId);
	}

	return true;
}

bool UserInfoMgr::RemoveInfoByUserId(unsigned int iUserId)
{
	map<unsigned __int64, UserInfo>::iterator iter = m_mUserInfo.begin();
	for (; iter != m_mUserInfo.end(); iter++)
	{
		if (iter->second.iUserId == iUserId)
		{
			UserInfoLock::GetInstance()->Lock();

			closesocket((SOCKET)iter->first);
			if (NULL != iter->second.PerHandleData)
				GlobalFree(iter->second.PerHandleData);
			if (NULL != iter->second.PerIoData)
				GlobalFree(iter->second.PerIoData);

			m_mUserInfo.erase(iter);

			UserInfoLock::GetInstance()->Unlock();

			printf("%s  iUserId[%u]\n", __FUNCTION__, iUserId);

			break;
		}
	}

	return true;
}

bool UserInfoMgr::InitNotNetDataBySocketId(unsigned __int64 socketId)
{
	map<unsigned __int64, UserInfo>::iterator iter = m_mUserInfo.find(socketId);
	if (iter != m_mUserInfo.end())
	{
		UserInfoLock::GetInstance()->Lock();
		m_mUserInfo[socketId].InitNotNetData();
		UserInfoLock::GetInstance()->Unlock();

		return true;
	}

	return false;
}

bool UserInfoMgr::SetUserIdBySocketId(unsigned __int64 socketId, unsigned int iUserId)
{
	map<unsigned __int64, UserInfo>::iterator iter = m_mUserInfo.find(socketId);
	if (iter != m_mUserInfo.end())
	{
		UserInfoLock::GetInstance()->Lock();
		m_mUserInfo[socketId].iUserId = iUserId;
		UserInfoLock::GetInstance()->Unlock();

		return true;
	}

	return false;
}

unsigned int UserInfoMgr::GetUserIdBySocketId(unsigned __int64 socketId)
{
	map<unsigned __int64, UserInfo>::iterator iter = m_mUserInfo.find(socketId);
	if (iter != m_mUserInfo.end())
	{
		UserInfoLock::GetInstance()->Lock();
		return m_mUserInfo[socketId].iUserId;
		UserInfoLock::GetInstance()->Unlock();
	}

	return 0; //ѧ�ſ϶�Ҫ���ô���0��
}

unsigned __int64 UserInfoMgr::GetSocketIdByAccount(string strAccount)
{
	map<unsigned __int64, UserInfo>::iterator iter = m_mUserInfo.begin();
	for (; iter != m_mUserInfo.end(); iter++)
	{
		if (iter->second.strAccount == strAccount)
		{
			UserInfoLock::GetInstance()->Lock();
			return iter->first;
			UserInfoLock::GetInstance()->Unlock();
		}
	}

	return 0;
}

bool UserInfoMgr::SetAuthorityBySocketId(unsigned __int64 socketId, string strAuthority)
{
	map<unsigned __int64, UserInfo>::iterator iter = m_mUserInfo.find(socketId);
	if (iter != m_mUserInfo.end())
	{
		UserInfoLock::GetInstance()->Lock();
		m_mUserInfo[socketId].strAuthority = strAuthority;
		UserInfoLock::GetInstance()->Unlock();

		return true;
	}

	return false;
}

string UserInfoMgr::GetAuthorityBySocketId(unsigned __int64 socketId)
{
	map<unsigned __int64, UserInfo>::iterator iter = m_mUserInfo.find(socketId);
	if (iter != m_mUserInfo.end())
	{
		UserInfoLock::GetInstance()->Lock();
		return m_mUserInfo[socketId].strAuthority;
		UserInfoLock::GetInstance()->Unlock();
	}

	return ""; 
}

bool UserInfoMgr::SetAuthorityByAccount(string strAccount, string strAuthority)
{
	map<unsigned __int64, UserInfo>::iterator iter = m_mUserInfo.begin();
	for (; iter != m_mUserInfo.end(); iter++)
	{
		if (iter->second.strAccount == strAccount)
		{
			UserInfoLock::GetInstance()->Lock();
			iter->second.strAuthority = strAuthority;
			UserInfoLock::GetInstance()->Unlock();

			return true;
		}
	}

	return false;
}

string UserInfoMgr::GetAuthorityByAccount(string strAccount)
{
	map<unsigned __int64, UserInfo>::iterator iter = m_mUserInfo.begin();
	for (; iter != m_mUserInfo.end(); iter++)
	{
		if (iter->second.strAccount == strAccount)
		{
			UserInfoLock::GetInstance()->Lock();
			return iter->second.strAuthority;
			UserInfoLock::GetInstance()->Unlock();
		}
	}

	return "";
}

bool UserInfoMgr::SetNewAccountByAccount(string strAccount, string strNewAccount)
{
	map<unsigned __int64, UserInfo>::iterator iter = m_mUserInfo.begin();
	for (; iter != m_mUserInfo.end(); iter++)
	{
		if (iter->second.strAccount == strAccount)
		{
			UserInfoLock::GetInstance()->Lock();
			iter->second.strAccount = strNewAccount;
			UserInfoLock::GetInstance()->Unlock();

			return true;
		}
	}

	return false;
}

bool UserInfoMgr::FindAccount(string strAccount)
{
	map<unsigned __int64, UserInfo>::iterator iter = m_mUserInfo.begin();
	for (; iter != m_mUserInfo.end(); iter++)
	{
		if (iter->second.strAccount == strAccount)
		{
			return true;
		}
	}

	return false;	
}

bool UserInfoMgr::IsHaveOneAuthorityBySocketId(unsigned __int64 socketId, OperPermission OperPer)
{
	string strAuthority = GetAuthorityBySocketId(socketId);
	if (strAuthority.empty())
	{
		return false;
	}

	vector<string> vecStrAuthority = StringTool::Splite(strAuthority);

	for (unsigned i=0; i<vecStrAuthority.size(); i++)
	{
		if (OperPer == (OperPermission)atoi(vecStrAuthority.at(i).c_str()))
		{
			return true;
		}
	}

	return false;
}

bool UserInfoMgr::SetAccountBySocketId(unsigned __int64 socketId, string strAccount)
{
	map<unsigned __int64, UserInfo>::iterator iter = m_mUserInfo.find(socketId);
	if (iter != m_mUserInfo.end())
	{
		UserInfoLock::GetInstance()->Lock();
		m_mUserInfo[socketId].strAccount = strAccount;
		UserInfoLock::GetInstance()->Unlock();

		return true;
	}

	return false;
}

string UserInfoMgr::GetAccountBySocketId(unsigned __int64 socketId)
{
	map<unsigned __int64, UserInfo>::iterator iter = m_mUserInfo.find(socketId);
	if (iter != m_mUserInfo.end())
	{
		UserInfoLock::GetInstance()->Lock();
		return m_mUserInfo[socketId].strAccount;
		UserInfoLock::GetInstance()->Unlock();
	}

	return ""; 
}

bool UserInfoMgr::SetIdentBySocketId(unsigned __int64 socketId, short sIdent)
{
	map<unsigned __int64, UserInfo>::iterator iter = m_mUserInfo.find(socketId);
	if (iter != m_mUserInfo.end())
	{
		UserInfoLock::GetInstance()->Lock();
		m_mUserInfo[socketId].sIdent = sIdent;
		UserInfoLock::GetInstance()->Unlock();

		return true;
	}

	return false;
}

short UserInfoMgr::GetIdentBySocketId(unsigned __int64 socketId)
{
	map<unsigned __int64, UserInfo>::iterator iter = m_mUserInfo.find(socketId);
	if (iter != m_mUserInfo.end())
	{
		UserInfoLock::GetInstance()->Lock();
		return m_mUserInfo[socketId].sIdent;
		UserInfoLock::GetInstance()->Unlock();
	}

	return 0; //�Ƿ�ֵ
}

bool UserInfoMgr::SetRegNeedCountBySocketId(unsigned __int64 socketId, short sRegNeedCount)
{
	map<unsigned __int64, UserInfo>::iterator iter = m_mUserInfo.find(socketId);
	if (iter != m_mUserInfo.end())
	{
		UserInfoLock::GetInstance()->Lock();
		m_mUserInfo[socketId].sRegNeedCount = sRegNeedCount;
		UserInfoLock::GetInstance()->Unlock();

		return true;
	}

	return false;
}

short UserInfoMgr::GetRegNeedCountBySocketId(unsigned __int64 socketId)
{
	map<unsigned __int64, UserInfo>::iterator iter = m_mUserInfo.find(socketId);
	if (iter != m_mUserInfo.end())
	{
		UserInfoLock::GetInstance()->Lock();
		return m_mUserInfo[socketId].sRegNeedCount;
		UserInfoLock::GetInstance()->Unlock();
	}

	return -1; 
}

bool UserInfoMgr::SetStrTmpDataBySocketId(unsigned __int64 socketId, string strTmpData)
{
	map<unsigned __int64, UserInfo>::iterator iter = m_mUserInfo.find(socketId);
	if (iter != m_mUserInfo.end())
	{
		UserInfoLock::GetInstance()->Lock();
		m_mUserInfo[socketId].strTmpData = strTmpData;
		UserInfoLock::GetInstance()->Unlock();

		return true;
	}

	return false;
}

string UserInfoMgr::GetStrTmpDataBySocketId(unsigned __int64 socketId)
{
	map<unsigned __int64, UserInfo>::iterator iter = m_mUserInfo.find(socketId);
	if (iter != m_mUserInfo.end())
	{
		UserInfoLock::GetInstance()->Lock();
		return m_mUserInfo[socketId].strTmpData;
		UserInfoLock::GetInstance()->Unlock();
	}

	return ""; 
}

bool UserInfoMgr::SetPerHandleDataBySocketId(unsigned __int64 socketId, PER_HANDLE_DATA* PerHandleData)
{
	map<unsigned __int64, UserInfo>::iterator iter = m_mUserInfo.find(socketId);
	if (iter != m_mUserInfo.end())
	{
		UserInfoLock::GetInstance()->Lock();
		m_mUserInfo[socketId].PerHandleData = PerHandleData;
		UserInfoLock::GetInstance()->Unlock();

		return true;
	}

	return false;
}

PER_HANDLE_DATA* UserInfoMgr::GetPerHandleDataBySocketId(unsigned __int64 socketId)
{
	map<unsigned __int64, UserInfo>::iterator iter = m_mUserInfo.find(socketId);
	if (iter != m_mUserInfo.end())
	{
		UserInfoLock::GetInstance()->Lock();
		return m_mUserInfo[socketId].PerHandleData;
		UserInfoLock::GetInstance()->Unlock();
	}

	return NULL; 
}

bool UserInfoMgr::SetPerIoDataBySocketId(unsigned __int64 socketId, LPPER_IO_OPERATION_DATA PerIoData)
{
	map<unsigned __int64, UserInfo>::iterator iter = m_mUserInfo.find(socketId);
	if (iter != m_mUserInfo.end())
	{
		UserInfoLock::GetInstance()->Lock();
		m_mUserInfo[socketId].PerIoData = PerIoData;
		UserInfoLock::GetInstance()->Unlock();

		return true;
	}

	return false;
}

LPPER_IO_OPERATION_DATA UserInfoMgr::GetPerIoDataBySocketId(unsigned __int64 socketId)
{
	map<unsigned __int64, UserInfo>::iterator iter = m_mUserInfo.find(socketId);
	if (iter != m_mUserInfo.end())
	{
		UserInfoLock::GetInstance()->Lock();
		return m_mUserInfo[socketId].PerIoData;
		UserInfoLock::GetInstance()->Unlock();
	}

	return NULL; 
}

void UserInfoMgr::GetDefaultAuthorityByIdent(IdentType Ident, vector<OperPermission>& vecOper)
{
	vecOper.clear();
	if (IDENT_TYPE_STUDENT == Ident)
	{
		vecOper.push_back(OPER_PER_LOGIN);
		vecOper.push_back(OPER_PER_REGISTER);

		vecOper.push_back(OPER_PER_SELECTSINGLESCOREBYONESUBJECT);
		vecOper.push_back(OPER_PER_SELECTSINGLESCOREBYSUBJECTS);

		vecOper.push_back(OPER_PER_UPDATESINGLEUSERINFOEBYONE);
		vecOper.push_back(OPER_PER_UPDATESINGLEUSERINFOBYMORE);
		vecOper.push_back(OPER_PER_SELECTSINGLEUSERINFO);
	}
	else if (IDENT_TYPE_TEACHER == Ident)
	{
		vecOper.push_back(OPER_PER_LOGIN);
		vecOper.push_back(OPER_PER_REGISTER);

		vecOper.push_back(OPER_PER_ADDBATCHSCOREBYONESUBJECT);
		vecOper.push_back(OPER_PER_ADDBATCHSCOREBYSUBJECTS);
		vecOper.push_back(OPER_PER_ADDSINGLESCOREBYONESUBJECT);
		vecOper.push_back(OPER_PER_ADDSINGLESCOREBYSUBJECTS);
		vecOper.push_back(OPER_PER_UPDATEBATCHSCOREBYONESUBJECT);
		vecOper.push_back(OPER_PER_UPDATEBATCHSCOREBYSUBJECTS);
		vecOper.push_back(OPER_PER_UPDATESINGLESCOREBYONESUBJECT);
		vecOper.push_back(OPER_PER_UPDATESINGLESCOREBYSUBJECTS);
		vecOper.push_back(OPER_PER_SELECTBATCHSCOREBYONESUBJECT);
		vecOper.push_back(OPER_PER_SELECTBATCHSCOREBYSUBJECTS);
		vecOper.push_back(OPER_PER_SELECTSINGLESCOREBYONESUBJECT);
		vecOper.push_back(OPER_PER_SELECTSINGLESCOREBYSUBJECTS);
		vecOper.push_back(OPER_PER_DELETEBATCHSCORE);
		vecOper.push_back(OPER_PER_DELETESINGLESCORE);
		vecOper.push_back(OPER_PER_ALTERADDONESCORESUBJECT);
		vecOper.push_back(OPER_PER_ALTERDELETEONESCORESUBJECT);

		vecOper.push_back(OPER_PER_ADDBATCHUSERINFO);
		vecOper.push_back(OPER_PER_ADDSINGLEUSERINFO);
		vecOper.push_back(OPER_PER_UPDATEBATCHUSERINFOBYONE);
		vecOper.push_back(OPER_PER_UPDATEBATCHUSERINFOBYMORE);
		vecOper.push_back(OPER_PER_UPDATESINGLEUSERINFOEBYONE);
		vecOper.push_back(OPER_PER_UPDATESINGLEUSERINFOBYMORE);
		vecOper.push_back(OPER_PER_SELECTBATCHUSERINFO);
		vecOper.push_back(OPER_PER_SELECTSINGLEUSERINFO);
		vecOper.push_back(OPER_PER_DELETEBATCHUSERINFO);
		vecOper.push_back(OPER_PER_DELETESINGLEUSERINFO);
	}
	else if (IDENT_TYPE_ADMIN == Ident)
	{
		vecOper.push_back(OPER_PER_LOGIN);
		vecOper.push_back(OPER_PER_REGISTER);

		vecOper.push_back(OPER_PER_ADDBATCHSCOREBYONESUBJECT);
		vecOper.push_back(OPER_PER_ADDBATCHSCOREBYSUBJECTS);
		vecOper.push_back(OPER_PER_ADDSINGLESCOREBYONESUBJECT);
		vecOper.push_back(OPER_PER_ADDSINGLESCOREBYSUBJECTS);
		vecOper.push_back(OPER_PER_UPDATEBATCHSCOREBYONESUBJECT);
		vecOper.push_back(OPER_PER_UPDATEBATCHSCOREBYSUBJECTS);
		vecOper.push_back(OPER_PER_UPDATESINGLESCOREBYONESUBJECT);
		vecOper.push_back(OPER_PER_UPDATESINGLESCOREBYSUBJECTS);
		vecOper.push_back(OPER_PER_SELECTBATCHSCOREBYONESUBJECT);
		vecOper.push_back(OPER_PER_SELECTBATCHSCOREBYSUBJECTS);
		vecOper.push_back(OPER_PER_SELECTSINGLESCOREBYONESUBJECT);
		vecOper.push_back(OPER_PER_SELECTSINGLESCOREBYSUBJECTS);
		vecOper.push_back(OPER_PER_DELETEBATCHSCORE);
		vecOper.push_back(OPER_PER_DELETESINGLESCORE);
		vecOper.push_back(OPER_PER_ALTERADDONESCORESUBJECT);
		vecOper.push_back(OPER_PER_ALTERDELETEONESCORESUBJECT);

		vecOper.push_back(OPER_PER_ADDBATCHUSERINFO);
		vecOper.push_back(OPER_PER_ADDSINGLEUSERINFO);
		vecOper.push_back(OPER_PER_UPDATEBATCHUSERINFOBYONE);
		vecOper.push_back(OPER_PER_UPDATEBATCHUSERINFOBYMORE);
		vecOper.push_back(OPER_PER_UPDATESINGLEUSERINFOEBYONE);
		vecOper.push_back(OPER_PER_UPDATESINGLEUSERINFOBYMORE);
		vecOper.push_back(OPER_PER_SELECTBATCHUSERINFO);
		vecOper.push_back(OPER_PER_SELECTSINGLEUSERINFO);
		vecOper.push_back(OPER_PER_DELETEBATCHUSERINFO);
		vecOper.push_back(OPER_PER_DELETESINGLEUSERINFO);

		vecOper.push_back(OPER_PER_ADDAUTHORITY);
		vecOper.push_back(OPER_PER_DELETEAUTHORITY);
	}
	else
	{
		printf("%s  ��ݱ�ʶ����Ident[%d]\n", __FUNCTION__, Ident);
	}
}

void UserInfoMgr::GetAuthorityByStrAuthority(string strAuthority, vector<OperPermission>& vecOper, unsigned char bType, string strSpl)
{
	vecOper.clear();
	if (strAuthority.empty() || bType < 1 || bType > 2)
	{
		return;
	}

	vector<string> vecStrMyAuthority = StringTool::Splite(strAuthority, strSpl);
	vector<OperPermission> vecOperTmp;
	for (unsigned i =0; i<vecStrMyAuthority.size(); i++)
	{
		vecOperTmp.push_back((OperPermission)atoi(vecStrMyAuthority.at(i).c_str()));
	}

	if (2 == bType)
	{
		for (map<OperPermission, string>::iterator iter=m_mAllOperPer.begin(); iter != m_mAllOperPer.end(); iter++)
		{
			bool bExist = false;
			bool bCountiune = false;
			for (unsigned i=0; i<vecOperTmp.size(); i++)
			{
				if (iter->first == OPER_PER_ADDAUTHORITY || iter->first == OPER_PER_DELETEAUTHORITY)
				{
					bCountiune = true;
					break;
				}
				if (vecOperTmp.at(i) == iter->first)
				{
					bExist = true;
					break;
				}
			}
			if (!bExist && !bCountiune)
			{
				vecOper.push_back(iter->first);
			}
		}
	}
	else
	{
		vecOper.assign(vecOperTmp.begin(), vecOperTmp.end());
	}
}

string UserInfoMgr::GetEnglishUserInfoFieldByField(int iFeildId)
{
	if (iFeildId < 1 || iFeildId > m_maxUserInfoFieldID)
	{
		return "";
	}

	string strFieldName = m_mUserInfoFieldName[iFeildId];
	vector<string> strVec = StringTool::Splite(strFieldName);
	if (strVec.size() != 2)
	{
		return "";
	}

	return strVec.at(0);
}

string UserInfoMgr::GetChineseUserInfoFieldByField(int iFeildId)
{
	if (iFeildId < 1 || iFeildId > m_maxUserInfoFieldID)
	{
		return "";
	}

	string strFieldName = m_mUserInfoFieldName[iFeildId];
	vector<string> strVec = StringTool::Splite(strFieldName);
	if (strVec.size() != 2)
	{
		return "";
	}

	return strVec.at(1);
}

bool UserInfoMgr::GetCanUpdateField(short iMyIdent, string strMyAccount, short iObjIdent, string strObjAccount, bool* pCanUpdate, int iCanUpdateMaxCount)
{
	if (pCanUpdate == NULL || iCanUpdateMaxCount < 8) //�û��ֶ�id��1-7�� �ֶ�id��Ϊ�����±����Գ�����СΪ8
	{
		return false;
	}
	memset(pCanUpdate, false, iCanUpdateMaxCount*sizeof(bool));
	if (iMyIdent < 1 || iMyIdent > 3 || iObjIdent < 1 || iObjIdent > 3)
	{
		return false;
	}

	bool bIsMine = false;
	if (iMyIdent == iObjIdent && strMyAccount == strObjAccount)
	{
		bIsMine = true;
	}

	bool bRes = true;
	if (iMyIdent == 1)
	{
		if (bIsMine) //�ɸ��� 1-account 2-password 3-name 4-sex
		{
			pCanUpdate[1] = true;
			pCanUpdate[2] = true;
			pCanUpdate[3] = true;
			pCanUpdate[4] = true;
		}
		else
		{
			bRes = false;
		}
	}
	else if (iMyIdent == 2)
	{
		if (bIsMine) //�ɸ��� 1-account 2-password 3-name 4-sex
		{
			pCanUpdate[1] = true;
			pCanUpdate[2] = true;
			pCanUpdate[3] = true;
			pCanUpdate[4] = true;
		}
		else if (iObjIdent == 1) //�ɸ��� 1-account 3-name 4-sex 6-major 7-grade
		{
			pCanUpdate[1] = true;
			pCanUpdate[3] = true;
			pCanUpdate[4] = true;
			pCanUpdate[6] = true;
			pCanUpdate[7] = true;
		}
		else
		{
			bRes = false;
		}
	}
	else
	{
		if (bIsMine) //�ɸ��� 1-account 2-password 3-name 4-sex 6-major 7-grade
		{
			pCanUpdate[1] = true;
			pCanUpdate[2] = true;
			pCanUpdate[3] = true;
			pCanUpdate[4] = true;
			pCanUpdate[6] = true;
			pCanUpdate[7] = true;
		}
		else if (iObjIdent == 1 || iObjIdent == 2) //�ɸ��� 1-account 2-password 3-name 4-sex 5-Ident 6-major 7-grade
		{
			pCanUpdate[1] = true;
			pCanUpdate[2] = true;
			pCanUpdate[3] = true;
			pCanUpdate[4] = true;
			pCanUpdate[5] = true;
			pCanUpdate[6] = true;
			pCanUpdate[7] = true;
		}
		else
		{
			bRes = false;
		}
	}

	return bRes;
}