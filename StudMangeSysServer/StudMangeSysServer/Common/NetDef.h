#ifndef __NETDEF_H__
#define __NETDEF_H__

//#define WIN32_LEAN_AND_MEAN
#include <WinSock2.h>  
#include <string>
using namespace std;

//�����Ĭ�϶˿ں�
#define  DefaultPort        6000

//�������ݳ��� �ͻ��˸�����˷��͵�����һ�㲻��
#define  DefaultRecvMSGLen  2048

//�������ݳ���
#define  DefaultSendMSGLen  2048

//������Ϣ�����������
#define MaxRecvMsgCount     50

/**
* �ṹ�����ƣ�PER_IO_DATA
* �ṹ�幦�ܣ��ص�I/O��Ҫ�õ��Ľṹ�壬��ʱ��¼IO����
**/
//const int DataBuffSize = DefaultRecvMSGLen;
typedef struct
{
	OVERLAPPED overlapped;
	WSABUF databuff;
	char buffer[DefaultRecvMSGLen];
	int BufferLen;
	int operationType;
}PER_IO_OPERATEION_DATA, *LPPER_IO_OPERATION_DATA, *LPPER_IO_DATA, PER_IO_DATA;


/**
* �ṹ�����ƣ�PER_HANDLE_DATA
* �ṹ��洢����¼�����׽��ֵ����ݣ��������׽��ֵı������׽��ֵĶ�Ӧ�Ŀͻ��˵ĵ�ַ��
* �ṹ�����ã��������������Ͽͻ���ʱ����Ϣ�洢���ýṹ���У�֪���ͻ��˵ĵ�ַ�Ա��ڻطá�
**/
typedef struct
{
	SOCKET socket;
	SOCKADDR_STORAGE ClientAddr;
}PER_HANDLE_DATA, *LPPER_HANDLE_DATA;

//������Ϣ���нṹ��
typedef struct 
{
	unsigned __int64 SocketId;
	char RecvMsg[DefaultRecvMSGLen];
	unsigned int DataLen;
	int AssistID;
} RecvMsgData, *PtrRecvMsgData;


struct UnpackData
{
	char MsgData[DefaultRecvMSGLen];
	unsigned DataLen;
	int AssistID;

	UnpackData()
	{
		memset(this, 0, sizeof(UnpackData));
	}
};

struct PackData
{
	char FullMsgData[DefaultSendMSGLen];
	unsigned FullDataLen;

	PackData()
	{
		memset(this, 0, sizeof(PackData));
	}
};

struct UserInfo
{
	LPPER_IO_OPERATION_DATA PerIoData;
	PER_HANDLE_DATA* PerHandleData;

	//�û�����¼
	unsigned int iUserId;
	string strAccount;
	string strIP;
	unsigned short sPort;
	string strAuthority;
	short sIdent;


	//�û���̬���ݼ�¼
	short sRegNeedCount;  //
	string strTmpData; //��ʱ����

	UserInfo()
	{
		PerIoData = NULL;
		PerHandleData = NULL;

		iUserId = 0;
		strAccount = "";
		strIP = "";
		sPort = 0;
		strAuthority = "";
		sIdent = 1;


		sRegNeedCount = 0;
		strTmpData = "";
	}
};


#endif