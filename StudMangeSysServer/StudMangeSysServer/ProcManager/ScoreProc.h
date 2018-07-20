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
	void CheckCanAlterSubjectRecvHandle(SOCKET SocketId, void* vpData, unsigned int DataLen); //��ȡ��Ŀ
	void AlterSubjectRecvHandle(SOCKET SocketId, void* vpData, unsigned int DataLen); //����������Ŀ����
	void AddSingleScoreRecvHandle(SOCKET SocketId, void* vpData, unsigned int DataLen); //�������ӳɼ����������ƺ�ȫ�ƣ�����


	/*
	* �ظ���Ϣ����strRecord��Ҫ��ǰ������д������غ���������
	*/
	void AlterSubjectReplyHandle(SOCKET SocketId, MYSQL_RES *MysqlRes, string strRecord); //���ݿ⴦��󣬷���������Ŀ���
	void AddSingleScoreReplyHandle(SOCKET SocketId, MYSQL_RES *MysqlRes, string strRecord); //���ݿ⴦��󣬷��ص������ӳɼ����������ƺ�ȫ�ƣ����
};

#endif