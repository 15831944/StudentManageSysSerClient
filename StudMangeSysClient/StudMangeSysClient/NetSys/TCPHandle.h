
#ifndef __TCPHANDLE_H__
#define __TCPHANDLE_H__

#include <winsock2.h>  
#include <Windows.h>  
#include "ProtoDef.h"
#include "xSingleton.h"

//�����̺߳���������Ҳ��������
DWORD WINAPI RecvThread(LPVOID IpParameter); 

class TCPHandle : public xSingleton<TCPHandle>
{
public:
	TCPHandle();
	~TCPHandle();

	bool TCPConn();

	SOCKET GetSocketID();

	bool Send(void* vpData, unsigned int DataLen, /*int iMianId,*/ int iAssistId);

	void SetIsConn(bool bIsConn);
	bool GetIsConn();
private:
	bool m_IsConn;
	SOCKET m_SocketId;      // ���ӳɹ�����׽���
};

#endif