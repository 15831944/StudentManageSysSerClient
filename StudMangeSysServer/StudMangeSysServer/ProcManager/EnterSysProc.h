#ifndef __ENTERSYSPROC_H__
#define __ENTERSYSPROC_H__


#include <WinSock2.h>
#include <mysql.h>
#include <Windows.h>
#include "ProtoDef.h"



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

	/*
	* �ظ���Ϣ����iResΪ0Ϊ����ִ��sql���
	*/
	void LoginReplyHandle(SOCKET SocketId, MYSQL_RES *MysqlRes, int iRes);
	void RegisterReplyHandle(SOCKET SocketId, MYSQL_RES *MysqlRes, int iRes);
};

#endif