#include "MsgPackageMgr.h"
#include "ProtoDef.h"

bool MsgPackageMgr::m_IsStartNet = false;

MsgPackageMgr::MsgPackageMgr()
{
	
}	

MsgPackageMgr::~MsgPackageMgr()
{

}

UnpackData MsgPackageMgr::Unpack( char* OriData, unsigned int OriDataLen)
{
	//ע���������������Ȼ�ͻ��˰�һ�㲻�����ý��պ�����tcp����ȡ���ݣ����ܻ����������հ��������߿ͻ��˳���Լ�����������Ͷ�η���

	UnpackData unpackData;
	if (NULL == OriData ) //�а�ͷ��һ������Ϊ��
	{
		printf("���յ����ݰ�Ϊ��\n");
		return unpackData;
	}
	if (0 == OriDataLen || OriDataLen > DefaultRecvMSGLen)
	{
		printf("���յ����ݰ����ȴ���OriDataLen[%u]\n", OriDataLen);
		return unpackData;
	}

	char strHead[15];
	memset(strHead, 0, sizeof(strHead));
	memcpy(strHead, OriData, sizeof(HeadData)); //����ȫ
	HeadData* pHeadData = (HeadData*)strHead;

	if (pHeadData->Len > sizeof(HeadData))
	{
		unpackData.AssistID = pHeadData->AssistID;
		unpackData.DataLen = pHeadData->Len-sizeof(HeadData);
		memcpy(unpackData.MsgData, &OriData[sizeof(HeadData)], unpackData.DataLen); //����ȫ
	}
	else if (pHeadData->Len == sizeof(HeadData))
	{
		unpackData.AssistID = pHeadData->AssistID;
		cout<<"Data Len == 0"<<endl;
	}
	else
	{
		cout<<"Data Len error"<<endl;
	}

	return unpackData;
}

PackData MsgPackageMgr::Pack(void* vpData, unsigned int DataLen, Assist_ID iAssistId)
{
	PackData packData;
	if (NULL == vpData && 0 != DataLen)
	{
		printf("������ϢΪ�գ����ǳ��Ȳ�Ϊ0 DataLen[%u]\n", DataLen);
		return packData;
	}
	if (iAssistId <= ASSIST_ID_START || iAssistId >= ASSIST_ID_END)
	{
		printf("iAssistId[%d] error\n", iAssistId);
		return packData;
	}

	HeadData hd;
	hd.AssistID = iAssistId;
	hd.Len = sizeof(HeadData) + DataLen;

	if (hd.Len > DefaultSendMSGLen)
	{
		cout<<"������Ϣ�������ڳ���Ϊ��"<<hd.Len<<endl;
		return packData;
	}

	packData.FullDataLen = hd.Len;
	memcpy(packData.FullMsgData, &hd, sizeof(HeadData));
	if (DataLen > 0)
		memcpy(&packData.FullMsgData[sizeof(HeadData)], vpData, DataLen);

	return packData;
}

bool MsgPackageMgr::Send(unsigned __int64 SocketId, PackData &packData)
{
	bool bRes = true;

	do 
	{
		if (!m_IsStartNet)
		{
			printf("����û�п���\n");
			bRes = false;
			break;
		}

		if (packData.FullDataLen > 0)
		{
			::send((SOCKET)SocketId, packData.FullMsgData, packData.FullDataLen, 0);  // ������Ϣ
			bRes = true;
		}
		else
		{
			printf("����ʧ�ܣ����ݳ���Ϊ0\n");
			bRes = false;
		}
	} while (false);

	return bRes;
}

void MsgPackageMgr::SetStartNet(bool bIsStartNet)
{
	m_IsStartNet = bIsStartNet;
}