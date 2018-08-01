#ifndef __USERINFOPROC_H__
#define __USERINFOPROC_H__

#include <string>
#include <WinSock2.h>
#include <mysql.h>
#include <Windows.h>
#include "ProtoDef.h"

using namespace std;

class UserInfoProc
{
public:
	UserInfoProc();
	~UserInfoProc();

	/*
	* ������Ϣ����
	*/
	void AddSingleUserInfoRecvHandle(SOCKET SocketId, void* vpData, unsigned int DataLen); //���������û���Ϣ����
	void AddBatchUserInfoRecvHandle(SOCKET SocketId, void* vpData, unsigned int DataLen); //���������û���Ϣ����

	/*
	* �ظ���Ϣ����strRecord��Ҫ��ǰ������д������غ���������
	*/
	void AddSingleUserInfoReplyHandle(SOCKET SocketId, MYSQL_RES *MysqlRes, string strRecord); //���������û���Ϣ�ظ�
	void AddBatchUserInfoReplyHandle(SOCKET SocketId, MYSQL_RES *MysqlRes, string strRecord); //���������û���Ϣ�ظ�
};

#endif