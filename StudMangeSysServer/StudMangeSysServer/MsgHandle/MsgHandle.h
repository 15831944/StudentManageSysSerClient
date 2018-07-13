#ifndef __MSGHANDLE_H__
#define __MSGHANDLE_H__

#include <queue>
#include "NetDef.h" //��WinSock2.h��������windows.hͷ�ļ�֮ǰ����
#include <Windows.h> 
#include "xSingleton.h"
#include "ProtoDef.h"

//��Ϣ���д����߳�
DWORD WINAPI MsgQueueThread(LPVOID IpParameter); 

class MsgHandleMgr : public xSingleton<MsgHandleMgr>
{
public:
	MsgHandleMgr();
	~MsgHandleMgr();

	void InputMsgQueue(unsigned __int64 SocketId, char* RecvMsg, unsigned int DataLen, Assist_ID AssistId);

	void MsgQueueHandle();

private:
	queue<RecvMsgData> m_RecvMsgQueue;
};


#endif