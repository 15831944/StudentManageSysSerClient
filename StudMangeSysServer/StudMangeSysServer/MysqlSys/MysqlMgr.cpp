#include "MysqlMgr.h"
#include "LockTools.h"


MysqlMgr::MysqlMgr() : 
		m_sUser("root"),
		m_sPswd("qq3197213"),
		m_sHost("localhost"),
		m_sDB("StudMangeSystem")
{
	m_IsConn = false;
	mysql_init(&m_MysqlCont);
	m_MysqlRes = NULL;
}

MysqlMgr::~MysqlMgr()
{
	if (!m_MysqlRes)
		mysql_free_result(m_MysqlRes); //�ͷŽ����Դ

	if (m_IsConn)
		mysql_close(&m_MysqlCont); //�Ͽ�����
}

bool MysqlMgr::MysqlConn()
{
	if (m_IsConn)
	{
		printf("It has connected mysql��\n");
		return true;
	}

	bool bRes = false;
	if(mysql_real_connect(&m_MysqlCont, m_sHost.c_str(), m_sUser.c_str() ,m_sPswd.c_str(), m_sDB.c_str(), MysqlConnPort, NULL, 0))
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

void MysqlMgr::InputMsgQueue(string strMysql, MysqlOper mysqlOper, OperPermission operPer, unsigned __int64 SocketId)
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
	if (operPer <= OPER_PER_START || operPer >= OPER_PER_END)
	{
		printf("Ȩ�޲������ʹ���[%d] error\n", operPer);
		return;
	}

	//��ʱ�����ƶ������������Ҫ���ƣ�ֻ�ܰ�֮ǰѹ�����ɾ��
	MysqlMsgData mysqlMsgData;
	mysqlMsgData.strMysql = strMysql;
	mysqlMsgData.mysqlOper = mysqlOper;
	mysqlMsgData.operPer = operPer;
	mysqlMsgData.SocketId = SocketId;

	MysqlMsgLock::GetInstance()->Lock();
	m_MysqlMsgQueue.push(mysqlMsgData);
	MysqlMsgLock::GetInstance()->Unlock();
}

void MysqlMgr::MsgQueueHandle()
{
	//ÿ�δ���һ��
	if(!m_MysqlMsgQueue.empty())
	{
		MysqlMsgData recvMsgData = m_MysqlMsgQueue.front();

		int iRes=mysql_query(&m_MysqlCont, recvMsgData.strMysql.c_str());  //ִ��

		if (!iRes)
		{
			if (recvMsgData.mysqlOper > MYSQL_OPER_START && recvMsgData.mysqlOper < MYSQL_OPER_END)
			{
				m_MysqlRes = NULL;
				if (MYSQL_OPER_SELECT == recvMsgData.mysqlOper)
				{
					m_MysqlRes=mysql_store_result(&m_MysqlCont);  //�����ѯ�������ݵ�m_MysqlRes����ѯʧ��m_MysqlResΪNULL
				}

				ProcMgr::GetInstance()->GetReplyHandleMoniter().DispatchEvent((OperPermission)recvMsgData.operPer, (SOCKET)recvMsgData.SocketId, m_MysqlRes, iRes);
			}
			else
			{
				printf("mysql operator type error : [%d]\n", recvMsgData.mysqlOper);
			}
		}
		else
		{
			printf("Execute mysql failed : %s\n", recvMsgData.strMysql.c_str());
		}


		MysqlMsgLock::GetInstance()->Lock();
		m_MysqlMsgQueue.pop();
		MysqlMsgLock::GetInstance()->Unlock();
	}
}