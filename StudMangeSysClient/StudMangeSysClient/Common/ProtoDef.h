
#ifndef __PROTODEF_H__
#define __PROTODEF_H__

//�����Ĭ�϶˿ں�
#define  DefaultPort        6000

//�������ݳ���
#define  DefaultRecvMSGLen  2048
//����ճ���
#define  MaxRecvMSGLen      4098

//�������ݳ��� �ͻ��˸�����˷��͵�����һ�㲻��
#define  DefaultSendMSGLen  2048

//����Ȩ�޺� �����˶�Ӧ  ��¼��ע�����Ȩ�ޣ�ÿ����Ҷ�ӵ��
enum OperPermission
{
	OPER_PER_INVALID                          =    0,
	OPER_PER_START	                          =    0, //��Чֵ��ʼֵ
	OPER_PER_LOGIN                            =    1, 
	OPER_PER_REGISTER                         =    2,

	OPER_PER_ADDBATCHSCOREBYONESUBJECT        =    3, //�����������ӳɼ� -- ���ǰ༶��Ҳ���� ������Ա����ʦ��
	OPER_PER_ADDBATCHSCOREBYSUBJECTS          =    4, //�������п�Ŀ�������ӳɼ� -- ���ǰ༶��Ҳ���� ������Ա����ʦ��
	OPER_PER_ADDSINGLESCOREBYONESUBJECT       =    5, //���Ƶ������ӳɼ� ������Ա����ʦ��
	OPER_PER_ADDSINGLESCOREBYSUBJECTS         =    6, //�������п�Ŀ�������ӳɼ� ������Ա����ʦ��
	OPER_PER_UPDATEBATCHSCOREBYONESUBJECT     =    7, //�����������ĳɼ� -- �༶�ԣ���ѡ�ĸ��༶������ǰ����Ҫ�ȰѰ༶�г��� ������Ա����ʦ��
	OPER_PER_UPDATEBATCHSCOREBYSUBJECTS       =    8, //�������п�Ŀ�������ĳɼ� -- �༶�ԣ���ѡ�ĸ��༶ ������Ա����ʦ��
	OPER_PER_UPDATESINGLESCOREBYONESUBJECT    =    9, //���Ƶ������ĳɼ� ������Ա����ʦ��
	OPER_PER_UPDATESINGLESCOREBYSUBJECTS      =    10,//�������п�Ŀ�������ĳɼ� ������Ա����ʦ��
	OPER_PER_SELECTBATCHSCOREBYONESUBJECT     =    11,//����������ѯ�ɼ� -- �༶�ԣ���ѡ�ĸ��༶ ������Ա����ʦ��
	OPER_PER_SELECTBATCHSCOREBYSUBJECTS       =    12,//�������п�Ŀ������ѯ�ɼ� -- �༶�ԣ���ѡ�ĸ��༶ ������Ա����ʦ��
	OPER_PER_SELECTSINGLESCOREBYONESUBJECT    =    13,//���Ƶ�����ѯ�ɼ� ������Ա����ʦ�� ѧ����ѧ���Լ���
	OPER_PER_SELECTSINGLESCOREBYSUBJECTS      =    14,//�������п�Ŀ������ѯ�ɼ� ������Ա����ʦ�� ѧ����ѧ���Լ���
	OPER_PER_DELETEBATCHSCORE			      =    15,//����ɾ���ɼ� -- �༶�ԣ���ѡ�ĸ��༶ ������Ա����ʦ��
	OPER_PER_DELETESINGLESCORE		          =    16,//����ɾ���ɼ� ������Ա����ʦ��
	OPER_PER_ALTERADDONESCORESUBJECT		  =    17,//���ӳɼ���Ŀ ������Ա����ʦ��
	OPER_PER_ALTERDELETEONESCORESUBJECT		  =    18,//ɾ���ɼ���Ŀ ������Ա����ʦ��

	OPER_PER_ADDBATCHUSERINFO	              =    19,//���������û���Ϣ -- ���ǰ༶��Ҳ���ԣ��̶��ֶ����� ������Ա����ʦ��
	OPER_PER_ADDSINGLEUSERINFO		          =    20,//���������û���Ϣ -- �̶��ֶ����� ������Ա����ʦ��
	OPER_PER_UPDATEBATCHUSERINFOBYONE		  =    21,//�����ֶ����������û���Ϣ -- �༶�ԣ���ѡ�ĸ��༶ ������Ա����ʦ��
	OPER_PER_UPDATEBATCHUSERINFOBYMORE        =    22,//���̶ֹ��ֶ����������û���Ϣ -- �༶�ԣ���ѡ�ĸ��༶���̶��ֶθ��� ������Ա����ʦ��
	OPER_PER_UPDATESINGLEUSERINFOEBYONE       =    23,//�����ֶε��������û���Ϣ ������Ա��ѧ������ʦ ����ʦ����ʦ�Լ�����ѧ���� ѧ����ѧ���Լ���
	OPER_PER_UPDATESINGLEUSERINFOBYMORE       =    24,//���̶ֹ��ֶε��������û���Ϣ -- �̶��ֶθ��� ������Ա��ѧ������ʦ�� ��ʦ����ʦ�Լ�����ѧ���� ѧ����ѧ���Լ���
	OPER_PER_SELECTBATCHUSERINFO	          =    25,//������ѯ�û���Ϣ -- �༶�ԣ���ѡ�ĸ��༶����ѯ�̶��ֶ� ������Ա����ʦ��
	OPER_PER_SELECTSINGLEUSERINFO		      =    26,//������ѯ�û���Ϣ -- ��ѯ�̶��ֶ� ������Ա����ʦ��ѧ���Լ���
	OPER_PER_DELETEBATCHUSERINFO			  =    27,//����ɾ���û���Ϣ -- �༶�ԣ���ѡ�ĸ��༶ ������Ա����ʦ��
	OPER_PER_DELETESINGLEUSERINFO		      =    28,//����ɾ���û���Ϣ ������Ա����ʦ��

	OPER_PER_ADDAUTHORITBYONE		          =    29,//����ĳ�û�һ�ֻ��߶���Ȩ�� ������Ա��
	OPER_PER_DELETEAUTHORITBYONE	          =    30,//ɾ��ĳ�û�һ�ֻ��߶���Ȩ�� ������Ա��

	OPER_PER_END									  //��Чֵ��ֵֹ
};

//������ѯ�ɼ���������
enum SelectScoreCondition
{
	SELECT_SCORE_INVALID                      =    0,
	SELECT_SCORE_START	                      =    0, //��Чֵ��ʼֵ
	SELECT_SCORE_RANK     	                  =    1, //���� -- ��Ϊ����ͽ���
	SELECT_SCORE_RANGE    	                  =    2, //������Χ -- ��ʼ������������
	SELECT_SCORE_TOTAL    	                  =    3, //�ֲܷ�ѯ
	SELECT_SCORE_AVERAGE    	              =    4, //ƽ���ֲ�ѯ

	SELECT_SCORE_END							      //��Чֵ��ֵֹ
};

//�����ѯ�ɼ�����������
enum SelectScoreRank
{
	SELECT_SCORE_RANK_INVALID                  =    0,
	SELECT_SCORE_RANK_START	                   =    0, //��Чֵ��ʼֵ
	SELECT_SCORE_RANK_ASC	                   =    1, //����
	SELECT_SCORE_RANK_DESC					   =    2, //����

	SELECT_SCORE_RANK_END						      //��Чֵ��ֵֹ
};

//������ѯ�û���Ϣ��������
enum SelectUserInfoCondition
{
	SELECT_USERINFO_INVALID                   =    0,
	SELECT_USERINFO_START	                  =    0, //��Чֵ��ʼֵ
	SELECT_USERINFO_RANGE    	              =    1, //�û�id��Χ -- ��ʼid����id
	SELECT_USERINFO_GRADE    	              =    2, //�༶��ѯ
	SELECT_USERINFO_PART_NAME    	          =    3, //����ģ����ѯ
	SELECT_USERINFO_SEX         	          =    4, //�Ա��ѯ
	SELECT_USERINFO_COUNT         	          =    5, //ͳ�Ʋ�ѯ -- ��ѯ��ǰ���ܼ�¼��


	SELECT_USERINFO_END						         //��Чֵ��ֵֹ
};

//ѧ��
enum SubjectsType
{
	SUBJECTS_TYPE_INVALID                    =    0,
	SUBJECTS_TYPE_START	                     =    0, //��Чֵ��ʼֵ
	SUBJECTS_TYPE_CHINESE                    =    1, //����
	SUBJECTS_TYPE_MATH                       =    2, //��ѧ
	SUBJECTS_TYPE_ENGLISH                    =    3, //Ӣ��
	SUBJECTS_TYPE_PHYSICS                    =    4, //����
	SUBJECTS_TYPE_CHEMISTRY                  =    5, //��ѧ
	SUBJECTS_TYPE_BIOLOGY                    =    6, //����
	SUBJECTS_TYPE_HISTORY                    =    7, //��ʷ
	SUBJECTS_TYPE_POLITICS                   =    8, //����
	SUBJECTS_TYPE_GEOGRAPHY                  =    9, //����

	SUBJECTS_TYPE_END						         //��Чֵ��ֵֹ
};

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
	ASSIST_ID_INVALID								 =   0,
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