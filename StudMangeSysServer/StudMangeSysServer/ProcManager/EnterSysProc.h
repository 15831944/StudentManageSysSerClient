#ifndef __ENTERSYSPROC_H__
#define __ENTERSYSPROC_H__

#include <string>
#include <WinSock2.h>
#include <mysql.h>
#include <Windows.h>
#include "ProtoDef.h"

using namespace std;

class EnterSysProc
{
public:
	EnterSysProc();
	~EnterSysProc();

	/*
	* ������Ϣ����
	*/
	void LoginRecvHandle(SOCKET SocketId, void* vpData, unsigned int DataLen);
	void RegisterRecvHandle(SOCKET SocketId, void* vpData, unsigned int DataLen);
	void ExitSysRecvHandle(SOCKET SocketId, void* vpData, unsigned int DataLen); //ֻ���û�����ϵͳ�ڣ����ǳ����ӻ���

	/*
	* �ظ���Ϣ����strRecord��Ҫ��ǰ������д������غ���������
	*/
	void LoginReplyHandle(SOCKET SocketId, MYSQL_RES *MysqlRes, string strRecord);
	void RegisterReplyHandle(SOCKET SocketId, MYSQL_RES *MysqlRes, string strRecord);
};

#endif