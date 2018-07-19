
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
	/*
	* ������Լ�ʹ��
	*/
	ASSIST_ID_INVALID								 =   0,
	ASSIST_ID_SPECIAL_START                          =   0,     //������Чֵ��ʼֵ
	ASSIST_ID_SPECIAL_GET_EXIAT_SUBJECTS             =   1,     //��ȡ���ڵĿ�Ŀ


	ASSIST_ID_SPECIAL_END                                 ,     //������Чֵ��ֵֹ


	/*------------------------------------------------------------------------------*/

	/*
	* ��ͻ��˽���
	*/
	ASSIST_ID_START									 =   10000, //��Чֵ��ʼֵ
	ASSIST_ID_LOGIN_REQ								 =   10001, //��¼����
	ASSIST_ID_LOGIN_ACK								 =   10002, //��¼�ظ�
	ASSIST_ID_REGISTER_REQ							 =   10003, //ע������
	ASSIST_ID_REGISTER_ACK							 =   10004, //ע��ظ�
	ASSIST_ID_EXIT_SYS_REQ							 =   10005, //�˳�ϵͳ
	ASSIST_ID_GET_CAN_ALTER_AFTER_ALTER_SUBJECTS_REQ =   10006, //������Ŀǰ����������Ŀ�Ŀ����
	ASSIST_ID_GET_CAN_ALTER_AFTER_ALTER_SUBJECTS_ACK =   10007, //������Ŀǰ����������Ŀ�Ŀ�ظ�
	ASSIST_ID_ALTER_SUBJECTS_REQ					 =   10008, //������Ŀ����
	ASSIST_ID_ALTER_SUBJECTS_ACK					 =   10009, //������Ŀ�ظ�


	ASSIST_ID_END												//��Чֵ��ֵֹ
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

//��¼����   assist[10001]
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

//��¼�ظ�   assist[10002]
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

//ע������   assist[10003]
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

//ע��ظ�   assist[10004]
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

//�˳�ϵͳ   assist[10005]
struct CS_MSG_EXIT_SYS_REQ
{
	bool bExit;
	CS_MSG_EXIT_SYS_REQ()
	{
		memset(this, 0, sizeof(CS_MSG_EXIT_SYS_REQ));
	}
};

//������Ŀǰ����������Ŀ�Ŀ����   assist[10006]
struct CS_MSG_GET_CAN_ALTER_AFTER_ALTER_SUBJECTS_REQ
{
	short sGetType; //1���ӿ�Ŀǰ��ȡ  2ɾ����Ŀǰ��ȡ
	CS_MSG_GET_CAN_ALTER_AFTER_ALTER_SUBJECTS_REQ()
	{
		memset(this, 0, sizeof(CS_MSG_GET_CAN_ALTER_AFTER_ALTER_SUBJECTS_REQ));
	}
};

//������Ŀǰ����������Ŀ�Ŀ�ظ�   assist[10007]
struct CS_MSG_GET_CAN_ALTER_AFTER_ALTER_SUBJECTS_ACK
{
	bool bSucceed;
	short sGetType; //1���ӿ�Ŀǰ��ȡ  2ɾ����Ŀǰ��ȡ
	char cCanAlterSubjects[60];
	CS_MSG_GET_CAN_ALTER_AFTER_ALTER_SUBJECTS_ACK()
	{
		memset(this, 0, sizeof(CS_MSG_GET_CAN_ALTER_AFTER_ALTER_SUBJECTS_ACK));
	}
};

//������Ŀ����   assist[10008]
struct CS_MSG_ALTER_SUBJECTS_REQ
{
	short sGetType; //1���ӿ�Ŀ  2ɾ����Ŀ
	short sSubjectId;
	CS_MSG_ALTER_SUBJECTS_REQ()
	{
		memset(this, 0, sizeof(CS_MSG_ALTER_SUBJECTS_REQ));
	}
};

//������Ŀ�ظ�   assist[10009]
struct CS_MSG_ALTER_SUBJECTS_ACK
{
	bool bSucceed;
	short sGetType;
	short sSubjectId;
	CS_MSG_ALTER_SUBJECTS_ACK()
	{
		memset(this, 0, sizeof(CS_MSG_ALTER_SUBJECTS_ACK));
	}
};

#endif