#include "NetSysMgr.h"
#include "UserInfoMgr.h"
#include "MsgPackageMgr.h"
#include "MsgHandle.h"

#pragma comment(lib, "Ws2_32.lib")      // Socket������õĶ�̬���ӿ�  
#pragma comment(lib, "Kernel32.lib")    // IOCP��Ҫ�õ��Ķ�̬���ӿ� 

#pragma warning(disable:4996)

// �����̺߳���  
DWORD WINAPI ServerWorkThread(LPVOID IpParam)
{
	HANDLE CompletionPort = (HANDLE)IpParam;
	DWORD BytesTransferred;
	LPOVERLAPPED IpOverlapped;
	LPPER_HANDLE_DATA PerHandleData = NULL;
	LPPER_IO_DATA PerIoData = NULL;
	DWORD RecvBytes;
	DWORD Flags = 0;
	BOOL bRet = false;

	while (true)
	{
		bRet = GetQueuedCompletionStatus(CompletionPort, &BytesTransferred, (PULONG_PTR)&PerHandleData, (LPOVERLAPPED*)&IpOverlapped, INFINITE);
		PerIoData = (LPPER_IO_DATA)CONTAINING_RECORD(IpOverlapped, PER_IO_DATA, overlapped);

		bool bIsError = false;
		do 
		{
			if (bRet == 0)
			{
				DWORD dwErr = GetLastError();
				cerr << "GetQueuedCompletionStatus Error: " << dwErr << endl;
				if (WAIT_TIMEOUT == dwErr)// ��ʱ
				{
					// ��ʱ��ͨ������һ����Ϣ���ж��Ƿ���ߣ�������socket��Ͷ��WSARecv�����
					// ��Ϊ����ͻ��������쳣�Ͽ�(����ͻ��˱������߰ε����ߵ�)��ʱ�򣬷����������޷��յ��ͻ��˶Ͽ���֪ͨ��
					if (-1 == send(PerHandleData->socket, "", 0, 0))
					{
						cerr << "GetQueuedCompletionStatus Error: TimeOut & Bolt" << endl;
						bIsError = true;
					}
					break;
				}
				if (ERROR_NETNAME_DELETED == dwErr)// �ͻ����쳣�˳�
				{
					cerr << "GetQueuedCompletionStatus Error: The client exits with an exception " << endl;
					bIsError = true;
					break;
				} 
			}
			// ������׽������Ƿ��д�����  
			if (0 == BytesTransferred)
			{
				cerr << "�׽������Ƿ��д���BytesTransferred=%d " << BytesTransferred <<endl;
				bIsError = true;
				break;
			}

		} while (false);
		if (bIsError)
		{
			UserInfoMgr::GetInstance()->RemoveInfoBySocketId(PerHandleData->socket);

			closesocket(PerHandleData->socket);
			GlobalFree(PerHandleData);
			GlobalFree(PerIoData);
			continue;
		}

		// ��ʼ���ݴ����������Կͻ��˵�����  
		UnpackData unpackData= MsgPackageMgr::Unpack(PerIoData->buffer, DefaultRecvMSGLen);
		if (unpackData.AssistID != ASSIST_ID_INVALID)
		{
			MsgHandleMgr::GetInstance()->InputMsgQueue(PerHandleData->socket, unpackData.MsgData, unpackData.DataLen, (Assist_ID)unpackData.AssistID);
		}

		// Ϊ��һ���ص����ý�����I/O��������  
		ZeroMemory(&(PerIoData->overlapped), sizeof(OVERLAPPED)); // ����ڴ�  
		PerIoData->databuff.len = DefaultRecvMSGLen;
		PerIoData->databuff.buf = PerIoData->buffer;
		PerIoData->operationType = 0;    // read  
		WSARecv(PerHandleData->socket, &(PerIoData->databuff), 1, &RecvBytes, &Flags, &(PerIoData->overlapped), NULL);
	}

	return 0;
}

NetSysMgr::NetSysMgr()
{
	m_IsStart = false;
}

NetSysMgr::~NetSysMgr()
{

}

void NetSysMgr::StartNet()
{
	if (m_IsStart)
	{
		cout<<"socket and iocp have start"<<endl;
		return;
	}

	// ����socket��̬���ӿ�  
	WORD wVersionRequested = MAKEWORD(2, 2); // ����2.2�汾��WinSock��  
	WSADATA wsaData;    // ����Windows Socket�Ľṹ��Ϣ  
	DWORD err = WSAStartup(wVersionRequested, &wsaData);

	if (0 != err)
	{  // ����׽��ֿ��Ƿ�����ɹ�  
		cerr << "Request Windows Socket Library Error!\n";
		system("pause");
		exit(0);
	}
	if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2)
	{// ����Ƿ�����������汾���׽��ֿ�  
		WSACleanup();
		cerr << "Request Windows Socket Version 2.2 Error!\n";
		system("pause");
		exit(0);
	}

	// ����IOCP���ں˶���  
	/**
	* ��Ҫ�õ��ĺ�����ԭ�ͣ�
	* HANDLE WINAPI CreateIoCompletionPort(
	*    __in   HANDLE FileHandle,     // �Ѿ��򿪵��ļ�������߿վ����һ���ǿͻ��˵ľ��
	*    __in   HANDLE ExistingCompletionPort, // �Ѿ����ڵ�IOCP���
	*    __in   ULONG_PTR CompletionKey,   // ��ɼ���������ָ��I/O��ɰ���ָ���ļ�
	*    __in   DWORD NumberOfConcurrentThreads // ��������ͬʱִ������߳�����һ���ƽ���CPU������*2
	* );
	**/
	HANDLE completionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
	if (NULL == completionPort)
	{    // ����IO�ں˶���ʧ��  
		cerr << "CreateIoCompletionPort failed. Error:" << GetLastError() << endl;
		system("pause");
		exit(0);
	}

	// ����IOCP�߳�--�߳����洴���̳߳�  

	// ȷ���������ĺ�������  
	SYSTEM_INFO mySysInfo;
	GetSystemInfo(&mySysInfo);

	// ���ڴ������ĺ������������߳�  
	for (DWORD i = 0; i < (mySysInfo.dwNumberOfProcessors * 2); ++i)
	{
		// �����������������̣߳�������ɶ˿ڴ��ݵ����߳�  
		HANDLE ThreadHandle = CreateThread(NULL, 0, ServerWorkThread, completionPort, 0, NULL);
		if (NULL == ThreadHandle)
		{
			cerr << "Create Thread Handle failed. Error:" << GetLastError() << endl;
			system("pause");
			exit(0);
		}
		CloseHandle(ThreadHandle);
	}

	// ������ʽ�׽���  
	SOCKET srvSocket = socket(AF_INET, SOCK_STREAM, 0);

	// ��SOCKET������  
	SOCKADDR_IN srvAddr;
	srvAddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	srvAddr.sin_family = AF_INET;
	srvAddr.sin_port = htons(DefaultPort);
	int bindResult = bind(srvSocket, (SOCKADDR*)&srvAddr, sizeof(SOCKADDR));
	if (SOCKET_ERROR == bindResult)
	{
		cerr << "Bind failed. Error:" << GetLastError() << endl;
		system("pause");
		exit(0);
	}

	// ��SOCKET����Ϊ����ģʽ  
	int listenResult = listen(srvSocket, 10);
	if (SOCKET_ERROR == listenResult)
	{
		cerr << "Listen failed. Error: " << GetLastError() << endl;
		system("pause");
		exit(0);
	}

	m_IsStart = true;
	MsgPackageMgr::SetStartNet(m_IsStart);

	// ��ʼ����IO����  
	cout << "����������׼�����������ڵȴ��ͻ��˵Ľ���...\n";

	while (true)
	{
		PER_HANDLE_DATA * PerHandleData = NULL;
		SOCKADDR_IN saRemote;
		int RemoteLen;
		SOCKET AcceptSocket;

		// �������ӣ���������ɶˣ����������AcceptEx()  
		RemoteLen = sizeof(saRemote);
		AcceptSocket = accept(srvSocket, (SOCKADDR*)&saRemote, &RemoteLen);
		if (SOCKET_ERROR == AcceptSocket)
		{   // ���տͻ���ʧ��  
			cerr << "Accept Socket Error: " << GetLastError() << endl;
			system("pause");
			exit(0);
		}

		// �����������׽��ֹ����ĵ����������Ϣ�ṹ  �����а壬����̷��ʺ͸��ģ�
		PerHandleData = (LPPER_HANDLE_DATA)GlobalAlloc(GPTR, sizeof(PER_HANDLE_DATA));  // �ڶ���Ϊ���PerHandleData����ָ����С���ڴ�  
		PerHandleData->socket = AcceptSocket;
		memcpy(&PerHandleData->ClientAddr, &saRemote, RemoteLen);  
		printf("new user connect: socketId[%llu]  IP[%s]  Port[%d]\n", (unsigned __int64)AcceptSocket, inet_ntoa(saRemote.sin_addr), saRemote.sin_port);
		//���������û���Ϣ
		UserInfoMgr::GetInstance()->InsertInfo(AcceptSocket, inet_ntoa(saRemote.sin_addr), saRemote.sin_port);

		// �������׽��ֺ���ɶ˿ڹ���  
		CreateIoCompletionPort((HANDLE)(PerHandleData->socket), completionPort, (DWORD)PerHandleData, 0);


		// ��ʼ�ڽ����׽����ϴ���I/Oʹ���ص�I/O����  
		// ���½����׽�����Ͷ��һ�������첽  
		// WSARecv��WSASend������ЩI/O������ɺ󣬹������̻߳�ΪI/O�����ṩ����      
		// ��I/O��������(I/O�ص�)  
		LPPER_IO_OPERATION_DATA PerIoData = NULL;
		PerIoData = (LPPER_IO_OPERATION_DATA)GlobalAlloc(GPTR, sizeof(PER_IO_OPERATEION_DATA));
		ZeroMemory(&(PerIoData->overlapped), sizeof(OVERLAPPED));
		PerIoData->databuff.len = DefaultRecvMSGLen;
		PerIoData->databuff.buf = PerIoData->buffer;
		PerIoData->operationType = 0;    // read  

		DWORD RecvBytes;
		DWORD Flags = 0;
		WSARecv(PerHandleData->socket, &(PerIoData->databuff), 1, &RecvBytes, &Flags, &(PerIoData->overlapped), NULL);
	}
}

