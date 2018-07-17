
#ifndef __PROTODEF_H__
#define __PROTODEF_H__

#if 0
//Э����ID
enum Main_ID
{
	MAIN_ID_INVALID          =   0,
	MAIN_ID_START            =   0, //��Чֵ��ʼֵ
	MAIN_ID_LOGINREGISTER    =   1,
	MAIN_ID_SCORE            =   2,
	MAIN_ID_USERINFO         =   3,

	MAIN_ID_END                     //��Чֵ��ֵֹ
};
#endif

//Э�鸨��ID
enum Assist_ID
{
	ASSIST_ID_INVALID          =   0,
	ASSIST_ID_START            =   10000, //��Чֵ��ʼֵ
	ASSIST_ID_LOGIN_REQ        =   10001, //��¼����
	ASSIST_ID_LOGIN_ACK        =   10002, //��¼�ظ�
	ASSIST_ID_REGISTER_REQ     =   10003, //ע������
	ASSIST_ID_REGISTER_ACK     =   10004, //ע��ظ�
	ASSIST_ID_EXIT_SYS         =   10005, //�˳�ϵͳ


	ASSIST_ID_END                     //��Чֵ��ֵֹ
};


//Э��ͷ����
struct HeadData
{
//	int MainID;
	int AssistID;
	int Len;

	HeadData()
	{
		memset(this, 0, sizeof(HeadData));
	}
};

//��¼����   assist[1]
struct SC_MSG_LOGIN_REQ
{
	char cAccount[31];
	char cPWD[31];
//	int  OperPerId;


	SC_MSG_LOGIN_REQ()
	{
		memset(this, 0, sizeof(SC_MSG_LOGIN_REQ));
	}
};

//��¼�ظ�   assist[2]
struct CS_MSG_LOGIN_ACK
{
	bool bSucceed;
	unsigned int iUserId;
	char cAccount[31];
	char cName[31];
	short sIdent;
	short sSex;
	char cOperPer[150];

	CS_MSG_LOGIN_ACK()
	{
		memset(this, 0, sizeof(SC_MSG_LOGIN_REQ));
	}
};

//ע������   assist[3]
struct SC_MSG_REGISTER_REQ
{
	char cName[31];
	char cAccount[31];
	char cPWD[31];
	char cSex[3];
	char cIdent[3];
//	int  OperPerId;

	SC_MSG_REGISTER_REQ()
	{
		memset(this, 0, sizeof(SC_MSG_REGISTER_REQ));
	}
};

//ע��ظ�   assist[4]
struct CS_MSG_REGISTER_ACK
{
	bool bSucceed;
	unsigned int iUserId;
	char cAccount[31];
	char cName[31];
	short sIdent;
	short sSex;
	char cOperPer[150];

	CS_MSG_REGISTER_ACK()
	{
		memset(this, 0, sizeof(CS_MSG_REGISTER_ACK));
	}
};

//�˳�ϵͳ   assist[5]
struct CS_MSG_EXIT_SYS
{
	bool bExit;
	CS_MSG_EXIT_SYS()
	{
		memset(this, 0, sizeof(CS_MSG_EXIT_SYS));
	}
};


#endif