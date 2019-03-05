#include "MysqlMgr.h"
#include "LockTools.h"
#include "StringTool.h"


MysqlMgr::MysqlMgr() : 
		m_sUser("root"),
		m_sPswd("qq3197213"),
		m_sHost("localhost"),
		m_sDB("StudMangeSystem")
{
	m_IsConn = false;
	mysql_init(&m_MysqlCont);
	m_MysqlRes = NULL;

	InitializeCriticalSection(&_critical);
}

MysqlMgr::~MysqlMgr()
{
	if (!m_MysqlRes)
		mysql_free_result(m_MysqlRes); //�ͷŽ����Դ

	if (m_IsConn)
		mysql_close(&m_MysqlCont); //�Ͽ�����

	DeleteCriticalSection(&_critical);
}

bool MysqlMgr::MysqlConn()
{
	if (m_IsConn)
	{
		printf("It has connected mysql��\n");
		return true;
	}

	bool bRes = false;
	if(mysql_real_connect(&m_MysqlCont, m_sHost.c_str(), m_sUser.c_str() ,m_sPswd.c_str(), m_sDB.c_str(), MysqlConnPort, NULL, CLIENT_MULTI_STATEMENTS)) //��Ҫʹ�ô洢���̣����һ����������ҪCLIENT_MULTI_STATEMENTS��������Ϊ0
	{
		printf("mysql connect succeed!\n");
		m_IsConn = true;
		int iRes = mysql_query(&m_MysqlCont, "SET NAMES GBK");  //���ñ����ʽ,������cmd���޷���ʾ����
		if(!iRes)
		{
			bRes = true;
			printf("mysql encode set succeed��\n");
		}
		else
		{
			m_IsConn = false;
			mysql_close(&m_MysqlCont); //�Ͽ�����
			printf("mysql encode set failed, Disconnect the mysql��\n");
		}
	}
	else
	{
		cout<<"connect failed!"<<endl;
	}

	return bRes;
}

void MysqlMgr::InputMsgQueue(string strMysql, MysqlOper mysqlOper, Assist_ID AssistId, unsigned __int64 SocketId, string strRecord)
{
	if (strMysql.empty())
	{
		printf("���ݿ����Ϊ��\n");
		return;
	}
	if (mysqlOper <= MYSQL_OPER_START || mysqlOper >= MYSQL_OPER_END)
	{
		printf("���ݿ�������ʹ���[%d] error\n", mysqlOper);
		return;
	}
	if (AssistId <= ASSIST_ID_SPECIAL_START || (AssistId >= ASSIST_ID_SPECIAL_END && AssistId <= ASSIST_ID_START) || AssistId >= ASSIST_ID_END)
	{
		printf("Assistid���ʹ���[%d] error\n", AssistId);
		return;
	}

	//��ʱ�����ƶ������������Ҫ���ƣ�ֻ�ܰ�֮ǰѹ�����ɾ��
	MysqlMsgData mysqlMsgData;
	mysqlMsgData.strMysql = strMysql;
	mysqlMsgData.mysqlOper = mysqlOper;
	mysqlMsgData.AssistId = AssistId;
	mysqlMsgData.SocketId = SocketId;
	mysqlMsgData.strRecord = strRecord;

	MysqlMsgLock uLock(_critical);
	//MysqlMsgLock::GetInstance()->Lock();
	m_MysqlMsgQueue.push(mysqlMsgData);
	//MysqlMsgLock::GetInstance()->Unlock();
}

void MysqlMgr::MsgQueueHandle()
{
	//ÿ�δ���һ��
	if(!m_MysqlMsgQueue.empty())
	{
		MysqlMsgData recvMsgData = m_MysqlMsgQueue.front();

		int iRes=mysql_query(&m_MysqlCont, recvMsgData.strMysql.c_str());  //ִ��

		if (iRes)
			printf("Execute mysql failed : %s\n", recvMsgData.strMysql.c_str());

		if (recvMsgData.mysqlOper > MYSQL_OPER_START && recvMsgData.mysqlOper < MYSQL_OPER_END)
		{
			m_MysqlRes = NULL;
			if (MYSQL_OPER_SELECT == recvMsgData.mysqlOper)
			{
				m_MysqlRes=mysql_store_result(&m_MysqlCont);  //�����ѯ�������ݵ�m_MysqlRes����ѯʧ��m_MysqlResΪNULL
			}

			string strRecord = StringTool::NumberToStr(iRes);
			if (!recvMsgData.strRecord.empty())
			{
				strRecord += "|" + recvMsgData.strRecord;
			}

			ProcMgr::GetInstance()->GetReplyHandleMoniter().DispatchEvent((Assist_ID)recvMsgData.AssistId, (SOCKET)recvMsgData.SocketId, m_MysqlRes, strRecord);
		}
		else
		{
			printf("mysql operator type error : [%d]\n", recvMsgData.mysqlOper);
		}


		MysqlMsgLock uLock(_critical);
		//MysqlMsgLock::GetInstance()->Lock();
		m_MysqlMsgQueue.pop();
		//MysqlMsgLock::GetInstance()->Unlock();
	}
}