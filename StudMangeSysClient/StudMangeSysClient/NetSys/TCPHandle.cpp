#include "TCPHandle.h"
#include "ProcMgr.h"

#pragma comment(lib, "Ws2_32.lib")      // Socket������õĶ�̬���ӿ�


/*
* socket�����������������:
* 1.�ڽ��ջ����С��Χ�ڣ�windows����4K��������˷��͵����ݺͿͻ��˽��ն��ں���������Ϣ�����ڣ������������������
* 2.����������˷�����Ϣ�ܴ󣬳������ջ��棬���ǿͻ���recv���õĽ��ճ���Ҳ�ܴ��㹻���ա�ϵͳ������е����ݶ������꣬recv�ٷ���
* 3.�������˷��͵����ݺܴ󣬳����ͻ���recv���õĽ��ճ��ȣ���ôrecv����ն�Σ������ض�Ρ������ӣ���Ҫ�ͻ����û��Լ�ʵ���������մ��룬�����ݽ�������
*/
DWORD WINAPI RecvThread(LPVOID IpParameter)
{
#if 0
	bool bIsWaitRecv = false;
	//char MaxRecvData[MaxRecvMSGLen];
	char* MaxRecvData = new char[MaxRecvMSGLen];
	memset(MaxRecvData, 0, sizeof(MaxRecvData));

	unsigned int DataLen = 0;
	unsigned int RemainLen = 0;
	int iMainId = MAIN_ID_INVALID;
	int iAssistId = ASSIST_ID_INVALID;
#endif
	while(1)
	{
		char recvBuf[DefaultRecvMSGLen];  
		memset(recvBuf, 0, sizeof(recvBuf));
		int res =  recv(TCPHandle::GetInstance()->GetSocketID(), recvBuf, DefaultRecvMSGLen, 0); //��������Ҫ���жϼ�⣬������˶��ߵ��쳣������Ҫ������
		if (-1 == res)
		{
			DWORD dwErr = GetLastError();
			cerr << "ReceiveMessageThread recv Error: " << dwErr << endl;
			if (WSAECONNRESET == dwErr)// Զ�������ر��������ӣ�һ��Ϊ�����ͻȻ�ر���
			{
				cerr << "RecvThread recv Error: An existing connection was forcibly closed by the remote hos" << endl;
				//delete MaxRecvData[];
			}
			system("pause");
			exit(0);
		}
#if 0
		//�������˷������ݹ��󣬵��ǿͻ������ý��յ�buffer���ȹ�С���⡣��TCPԭ�����н��ջ��棨windowsΪ4K������ʱ��ϵͳ���ν���ֱ�����ݽ�����recv�ٷ��أ�����recv�ӵĽ��ճ�����ʵ�������ù��󣬲����������������ݶ��ܴ�
		if (!bIsWaitRecv)
		{
			char strHead[15];
			memset(strHead, 0, sizeof(strHead));
			memset(MaxRecvData, 0, sizeof(MaxRecvData));

			memcpy(strHead, recvBuf, sizeof(HeadData));
			HeadData* pHeadData = (HeadData*)strHead;

			if (pHeadData->Len > DefaultRecvMSGLen)
				bIsWaitRecv = true;
			iMainId = pHeadData->MainID;
			iAssistId = pHeadData->AssistID;
			
			DataLen = pHeadData->Len - sizeof(HeadData);

			if (bIsWaitRecv)
			{	
				memcpy(MaxRecvData, &recvBuf[sizeof(HeadData)], DefaultRecvMSGLen-sizeof(HeadData));
				RemainLen = pHeadData->Len-DefaultRecvMSGLen;
				continue;
			}
			else
			{
				memcpy(MaxRecvData, &recvBuf[sizeof(HeadData)], DataLen);
			}
		}
		else
		{
			if (RemainLen > DefaultRecvMSGLen)
			{
				memcpy(&MaxRecvData[DataLen-RemainLen], recvBuf, DefaultRecvMSGLen);
				RemainLen -= DefaultRecvMSGLen;
				continue;
			}
			else
			{
				memcpy(&MaxRecvData[DataLen-RemainLen], recvBuf, RemainLen);
				RemainLen = 0;
				bIsWaitRecv = false;
			}
		}
#endif

		//���������������ɷ���˶��send���ͣ���Э�����������Ƿ������������������õȴ�����������ɲŴ����ʵ�ַ�ʽ������߲���ʱ����������
		char strHead[15];
		memset(strHead, 0, sizeof(strHead));
		memcpy(strHead, recvBuf, sizeof(HeadData)); //����ȫ
		HeadData* pHeadData = (HeadData*)strHead;

		char strData[DefaultRecvMSGLen];
		unsigned int DataLen = 0;
		memset(strData, 0, sizeof(strData));
		if (pHeadData->Len > sizeof(HeadData))
		{
			DataLen = pHeadData->Len-sizeof(HeadData);
			memcpy(strData, &recvBuf[sizeof(HeadData)], DataLen); //����ȫ
		}
		else if (pHeadData->Len == sizeof(HeadData))
			cout<<"Data Len == 0"<<endl;
		else
		{
			cout<<"Data Len error"<<endl;
			continue;
		}

		//���ݽ��̹�����������һ�����н��̡�ֻҪ�����еģ�������ϢͶ��ȥ
		ProcMgr::GetInstance()->Recv(strData, DataLen, /*pHeadData->MainID,*/ pHeadData->AssistID);
#if 0
		iMainId = MAIN_ID_INVALID;
		iAssistId = ASSIST_ID_INVALID;
		DataLen = 0;
		RemainLen = 0;
		bIsWaitRecv = false;
#endif
	}

	//delete MaxRecvData[];
	return 0;
}

TCPHandle::TCPHandle()
{
	m_IsConn = false;
	m_SocketId = 0; // �������ɵ�SOCKET����Ϊ0���߸�����
}

TCPHandle::~TCPHandle()
{
	TCPDisConn();
}

bool TCPHandle::TCPConn()
{
	if (m_IsConn)
	{
		printf(" TCP Have Connect\n");   
		return true;
	}

	// ����socket��̬���ӿ�(dll)  
	WORD wVersionRequested;  
	WSADATA wsaData;    // ��ṹ�����ڽ���Wjndows Socket�Ľṹ��Ϣ��  
	wVersionRequested = MAKEWORD( 2, 2 );   // ����2.2�汾��WinSock��  
	int err = WSAStartup( wVersionRequested, &wsaData );  
	if ( err != 0 ) 
	{   // ����ֵΪ���ʱ���Ǳ�ʾ�ɹ�����WSAStartup  
		return false;  
	}  
	if ( LOBYTE( wsaData.wVersion ) != 2 || HIBYTE( wsaData.wVersion ) != 2 ) 
	{ // ���汾���Ƿ���ȷ  
		WSACleanup( );  
		return false;   
	}  

	// ����socket������������ʽ�׽��֣������׽��ֺ�m_SocketId  
	m_SocketId = socket(AF_INET, SOCK_STREAM, 0);  
	if(m_SocketId == INVALID_SOCKET) 
	{   
		printf("Error at socket():%ld\n", WSAGetLastError());   
		WSACleanup();   
		return false;   
	}   

	// ���׽���m_SocketId��Զ����������  
	// int connect( SOCKET s,  const struct sockaddr* name,  int namelen);  
	// ��һ����������Ҫ�������Ӳ������׽���  
	// �ڶ����������趨����Ҫ���ӵĵ�ַ��Ϣ  
	// ��������������ַ�ĳ���  
	SOCKADDR_IN addrSrv;  
	addrSrv.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");      // ���ػ�·��ַ��127.0.0.1;   
	addrSrv.sin_family = AF_INET;  
	addrSrv.sin_port = htons(DefaultPort);  
	while(SOCKET_ERROR == connect(m_SocketId, (SOCKADDR*)&addrSrv, sizeof(SOCKADDR)))
	{  
		// �����û�����Ϸ�������Ҫ������  
		cout << "����������ʧ�ܣ��Ƿ��������ӣ���Y/N):";  
		char choice;  
		while(cin >> choice && (!((choice != 'Y' && choice == 'N') || (choice == 'Y' && choice != 'N'))))
		{  
			cout << "�����������������:";  
			cin.sync();  
			cin.clear();  
		}  
		if (choice == 'Y'){  
			continue;  
		}  
		else{  
			cout << "�˳�ϵͳ��..."<<endl;    
			return false;  
		}  
	}  
	cin.sync();  
	cout << "���ӷ���˳ɹ���\n"; 

	m_IsConn = true;
	return true;
}

bool TCPHandle::TCPDisConn()
{
	if (m_IsConn)
	{
		closesocket(m_SocketId);  
		WSACleanup();   // ��ֹ���׽��ֿ��ʹ�� 
		m_IsConn = false;
	}

	return true;
}

SOCKET TCPHandle::GetSocketID()
{
	return m_SocketId;
}

bool TCPHandle::Send(void* vpData, unsigned int DataLen, /*int iMianId,*/ int iAssistId)
{
	if (!m_IsConn)
	{
		cout<<"�ͻ��˻�û�����ӷ�������"<<endl;
		return false;
	}
	//vpData����ΪNULL�����Ƕ�Ӧ��DataLen����ҲΪ0���������
	if (NULL == vpData && DataLen > 0)
	{	
		cout<<"��ϢΪ�գ�������Ϣ���ȴ���0��"<<endl;
		return false;
	}

	HeadData hd;
	//hd.MainID = iMianId;
	hd.AssistID = iAssistId;
	hd.Len = sizeof(HeadData) + DataLen;
	
	if (hd.Len > DefaultSendMSGLen)
	{
		cout<<"������Ϣ�������ڳ���Ϊ��"<<hd.Len<<endl;
		return false;
	}

	char SendMsg[DefaultSendMSGLen];
	memset(SendMsg, 0, sizeof(SendMsg));
	memcpy(SendMsg, &hd, sizeof(HeadData));
	if (DataLen > 0)
		memcpy(&SendMsg[sizeof(HeadData)], vpData, DataLen);

	::send(m_SocketId, SendMsg, hd.Len, 0);  // ������Ϣ 

	return true;
}

void TCPHandle::SetIsConn(bool bIsConn)
{
	m_IsConn = bIsConn;
}

bool TCPHandle::GetIsConn()
{
	return m_IsConn;
}