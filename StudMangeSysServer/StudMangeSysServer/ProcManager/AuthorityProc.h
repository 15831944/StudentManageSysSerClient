#ifndef __AUTHORITBYPROC_H__
#define __AUTHORITBYPROC_H__

#include <string>
#include <WinSock2.h>
#include <mysql.h>
#include <Windows.h>
#include "ProtoDef.h"

using namespace std;

class AuthorityProc
{
public:
	AuthorityProc();
	~AuthorityProc();

	/*
	* ������Ϣ����
	*/
	void GetAuthoritRecvHandle(SOCKET SocketId, void* vpData, unsigned int DataLen); //��ȡ���л������Ȩ������
	void EditAuthoritRecvHandle(SOCKET SocketId, void* vpData, unsigned int DataLen); //��ɾȨ������

	/*
	* �ظ���Ϣ����strRecord��Ҫ��ǰ������д������غ���������
	*/
	void GetAuthoritReplyHandle(SOCKET SocketId, MYSQL_RES *MysqlRes, string strRecord); //��ȡ���л������Ȩ�޻ظ�
	void EditAuthoritReplyHandle(SOCKET SocketId, MYSQL_RES *MysqlRes, string strRecord); //��ɾȨ�޻ظ�  --����Ҫ�ص�¼����Ч
};

#endif