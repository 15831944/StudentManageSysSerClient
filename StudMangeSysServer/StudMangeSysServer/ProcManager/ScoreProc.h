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
	* 接收消息处理
	*/
	void CheckCanAlterSubjectRecvHandle(SOCKET SocketId, void* vpData, unsigned int DataLen); //获取科目
	void AlterSubjectRecvHandle(SOCKET SocketId, void* vpData, unsigned int DataLen); //接收增减科目请求
	void AddSingleScoreRecvHandle(SOCKET SocketId, void* vpData, unsigned int DataLen); //单条增加成绩（包括单科和全科）请求
	void AddBatchScoreRecvHandle(SOCKET SocketId, void* vpData, unsigned int DataLen); //批量增加成绩（包括单科和全科或者多科）请求
	void SelectSingleScoreRecvHandle(SOCKET SocketId, void* vpData, unsigned int DataLen); //单条查询成绩（包括单科和全科）请求
	void SelectBatchScoreRecvHandle(SOCKET SocketId, void* vpData, unsigned int DataLen); //批量查询成绩（包括单科和全科）请求
	void UpdateSingleScoreRecvHandle(SOCKET SocketId, void* vpData, unsigned int DataLen); //单条更改成绩（包括单科和全科）请求
	void DeleteScoreRecvHandle(SOCKET SocketId, void* vpData, unsigned int DataLen); //删除成绩（包括单条和批量）请求


	/*
	* 回复消息处理。strRecord需要从前面操作中带到返回函数的数据
	*/
	void AlterSubjectReplyHandle(SOCKET SocketId, MYSQL_RES *MysqlRes, string strRecord); //数据库处理后，返回增减科目结果
	void AddSingleScoreReplyHandle(SOCKET SocketId, MYSQL_RES *MysqlRes, string strRecord); //数据库处理后，返回单条增加成绩（包括单科和全科）结果
	void AddBatchScoreReplyHandle(SOCKET SocketId, MYSQL_RES *MysqlRes, string strRecord); //数据库处理后，返回批量增加成绩（包括单科和全科或者多科）结果
	void SelectSingleScoreReplyHandle(SOCKET SocketId, MYSQL_RES *MysqlRes, string strRecord); //数据库处理后，返回单条查询成绩（包括单科和全科）结果
	void SelectBatchScoreReplyHandle(SOCKET SocketId, MYSQL_RES *MysqlRes, string strRecord); //数据库处理后，返回批量查询成绩（包括单科和全科）结果
	void UpdateSingleScoreReplyHandle(SOCKET SocketId, MYSQL_RES *MysqlRes, string strRecord); //数据库处理后，返回单条更改成绩（包括单科和全科）结果
	void DeleteScoreReplyHandle(SOCKET SocketId, MYSQL_RES *MysqlRes, string strRecord); //数据库处理后，返回删除成绩（包括单条和批量）结果
};

#endif