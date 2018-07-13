#include "ServerMgr.h"
#include "ProcMgr.h"
#include "MsgHandle.h"
#include "MysqlMgr.h"
#include "NetSysMgr.h"

ServerMgr::ServerMgr()
{

}

ServerMgr::~ServerMgr()
{

}

void ServerMgr::ServerRun()
{
	//��ʼ����������ѹ�����Ľ��մ����������ݿⷵ�ؽ����Ĵ�����ѹ�뵽�����
	ProcMgr::GetInstance()->InitMoniter();


	//������Ϣ���д����̡߳���Ҫ����tcp���յ���Ϣ���к�mysql����Ϣ����
	HANDLE msgQueueThread = CreateThread(NULL, 0, MsgQueueThread, NULL, 0, NULL);
	CloseHandle(msgQueueThread);

	//�������ݿ�
	if (!MysqlMgr::GetInstance()->MysqlConn())
	{
		system("pause");
		exit(0);
	}

	//��ʼ��socket����ؽӿں����ݽṹ�����̼߳�ؿͻ������ӣ������̴߳���ϵͳ���պõ���Ϣ
	NetSysMgr::GetInstance()->StartNet();
}