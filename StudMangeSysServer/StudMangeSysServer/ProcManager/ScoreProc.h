#ifndef __SCOREPROC_H__
#define __SCOREPROC_H__

#include <string>
#include <WinSock2.h>
#include <mysql.h>
#include <Windows.h>
#include "ProtoDef.h"

using namespace std;

class ScoreProc
{
public:
	ScoreProc();
	~ScoreProc();

	/*
	* ������Ϣ����
	*/
	void CheckCanAlterSubjectRecvHandle(SOCKET SocketId, void* vpData, unsigned int DataLen);
	void AlterSubjectRecvHandle(SOCKET SocketId, void* vpData, unsigned int DataLen);


	/*
	* �ظ���Ϣ����strRecord��Ҫ��ǰ������д������غ���������
	*/
	void AlterSubjectReplyHandle(SOCKET SocketId, MYSQL_RES *MysqlRes, string strRecord);
};

#endif