
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

//����Ŀ��
#define MAXSUBJECTSCOUNT 20
//������ѯ������󷵻ؼ�¼��
#define MAXBATCHSELECTACKCOUNT 10

//����Ȩ�޺� �����˶�Ӧ  ��¼��ע�����Ȩ�ޣ�ÿ����Ҷ�ӵ��
enum OperPermission
{
	OPER_PER_INVALID                          =    0,
	OPER_PER_START	                          =    0, //��Чֵ��ʼֵ
	OPER_PER_LOGIN                            =    1, 
	OPER_PER_REGISTER                         =    2,

	OPER_PER_ADDBATCHSCOREBYONESUBJECT        =    3, //�����������ӳɼ� -- ���ǰ༶��Ҳ���� ������Ա����ʦ��
	OPER_PER_ADDBATCHSCOREBYSUBJECTS          =    4, //�������п�Ŀ�����߶�ƣ��������ӳɼ� -- ���ǰ༶��Ҳ���� ������Ա����ʦ��
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
	SELECT_SCORE_CONDITION_INVALID                 =    0,
	SELECT_SCORE_CONDITION_START	               =    0, //��Чֵ��ʼֵ
	SELECT_SCORE_CONDITION_RANK     	           =    1, //���� -- ��Ϊ����ͽ���
	SELECT_SCORE_CONDITION_RANGE    	           =    2, //������Χ -- ��ʼ������������
	SELECT_SCORE_CONDITION_TOTAL    	           =    3, //�ֲܷ�ѯ
	SELECT_SCORE_CONDITION_AVERAGE    	           =    4, //ƽ���ֲ�ѯ

	SELECT_SCORE_CONDITION_END							   //��Чֵ��ֵֹ
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
	ASSIST_ID_EXIT_SYS_REQ							 =   10005, //�˳�ϵͳ -- �ò������Э��ID
	ASSIST_ID_GET_SUBJECTS_REQ						 =   10006, //��ȡʣ������п�Ŀ����
	ASSIST_ID_GET_SUBJECTS_ACK						 =   10007, //��ȡʣ������п�Ŀ�ظ�
	ASSIST_ID_ALTER_SUBJECTS_REQ					 =   10008, //������Ŀ����
	ASSIST_ID_ALTER_SUBJECTS_ACK					 =   10009, //������Ŀ�ظ�
	ASSIST_ID_ADD_SINGLE_SCORE_REQ					 =   10010, //������ӳɼ�����
	ASSIST_ID_ADD_SINGLE_SCORE_ACK					 =   10011, //������ӳɼ��ظ�
	ASSIST_ID_SELECT_SINGLE_SCORE_REQ			     =   10012, //������ѯ�ɼ�����
	ASSIST_ID_SELECT_SINGLE_SCORE_ACK			     =   10013, //������ѯ�ɼ��ظ�
	ASSIST_ID_SELECT_BATCH_SCORE_REQ			     =   10014, //������ѯ�ɼ�����
	ASSIST_ID_SELECT_BATCH_SCORE_ACK			     =   10015, //������ѯ�ɼ��ظ�
	ASSIST_ID_UPDATE_SINGLE_SCORE_REQ			     =   10016, //�������³ɼ�����
	ASSIST_ID_UPDATE_SINGLE_SCORE_ACK			     =   10017, //�������³ɼ��ظ�
	ASSIST_ID_DELETE_SCORE_REQ					     =   10018, //ɾ���ɼ�����
	ASSIST_ID_DELETE_SCORE_ACK					     =   10019, //ɾ���ɼ��ظ�
	ASSIST_ID_ADD_BATCH_SCORE_REQ					 =   10020, //������ӳɼ�����
	ASSIST_ID_ADD_BATCH_SCORE_ACK					 =   10021, //������ӳɼ��ظ�


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
	char cIdent[3]; //ֻ��ע��ѧ������ʦ�� 1ѧ�� 2��ʦ
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


//�˳�ϵͳ   assist[10005] -- �ò������Э��
struct CS_MSG_EXIT_SYS_REQ
{
	bool bExit;
	CS_MSG_EXIT_SYS_REQ()
	{
		memset(this, 0, sizeof(CS_MSG_EXIT_SYS_REQ));
	}
};

//��ȡʣ������п�Ŀ����   assist[10006]
struct CS_MSG_GET_SUBJECTS_REQ
{
	short sGetType; //1ʣ���Ŀ��δ���ӣ�  2���п�Ŀ
	CS_MSG_GET_SUBJECTS_REQ()
	{
		memset(this, 0, sizeof(CS_MSG_GET_SUBJECTS_REQ));
	}
};

//��ȡʣ������п�Ŀ�ظ�   assist[10007]
struct CS_MSG_GET_SUBJECTS_ACK
{
	bool bSucceed;
	short sGetType; //1ʣ���Ŀ��δ��ӣ�  2���п�Ŀ
	char cCanAlterSubjects[60];
	CS_MSG_GET_SUBJECTS_ACK()
	{
		memset(this, 0, sizeof(CS_MSG_GET_SUBJECTS_ACK));
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

//������ӳɼ�����   assist[10010]
struct CS_MSG_ADD_SINGLE_SCORE_REQ
{
	short sType; //1���Ƶ������ӳɼ� 2�������п�Ŀ�������ӳɼ�
	char cAccount[31]; //ͨ���˺���ӳɼ�  ������ݿ�û������˺ţ��͵ô����µ��˺š����Բ���ͨ��userid��userid���Զ����ӵ�
	char sSubjectId[60]; //ÿ��IDʹ�÷ָ���|����
	char sScore[90]; //ÿ�Ʒ���ʹ�÷ָ���|����
	CS_MSG_ADD_SINGLE_SCORE_REQ()
	{
		memset(this, 0, sizeof(CS_MSG_ADD_SINGLE_SCORE_REQ));
	}
};

//������ӳɼ��ظ�   assist[10011]
struct CS_MSG_ADD_SINGLE_SCORE_ACK
{
	bool bSucceed;
	short sType; //1���Ƶ������ӳɼ� 2�������п�Ŀ�������ӳɼ�
	char cAccount[31];
	CS_MSG_ADD_SINGLE_SCORE_ACK()
	{
		memset(this, 0, sizeof(CS_MSG_ADD_SINGLE_SCORE_ACK));
	}
};

//������ѯ�ɼ�����   assist[10012]
struct CS_MSG_SELECT_SINGLE_SCORE_REQ
{
	short sType; //1���Ƶ�����ѯ�ɼ� 2�������п�Ŀ������ѯ�ɼ�
	char cAccount[31]; //ͨ���˺Ų�ѯ�ɼ�  userid���Զ����ӵģ����Բ�����userid
	unsigned char sSubjectId[MAXSUBJECTSCOUNT]; //ÿ����ĿID
	unsigned char bSubjectCount; //��Ŀ��
	CS_MSG_SELECT_SINGLE_SCORE_REQ()
	{
		memset(this, 0, sizeof(CS_MSG_SELECT_SINGLE_SCORE_REQ));
	}
};

//������ѯ�ɼ��ظ�   assist[10013]
struct CS_MSG_SELECT_SINGLE_SCORE_ACK
{
	short sType; //1���Ƶ�����ѯ�ɼ� 2�������п�Ŀ������ѯ�ɼ�
	unsigned int uUserid; //�û�id
	char cName[31]; //����
	char cAccount[31]; //�˺�
	char cGrade[31]; //�༶
	unsigned char bSubjectId[MAXSUBJECTSCOUNT]; //ÿ����ĿID
	unsigned char bScore[MAXSUBJECTSCOUNT]; //ÿ�Ʒ���
	unsigned char bSubjectCount; //��Ŀ��
	unsigned char bResCode; //0�ɹ� 1ʧ�� 2���ݿ�û���˺���Ϣ����û�гɼ���Ϣ 3�����쳣
	CS_MSG_SELECT_SINGLE_SCORE_ACK()
	{
		memset(this, 0, sizeof(CS_MSG_SELECT_SINGLE_SCORE_ACK));
	}
};

//������ѯ�ɼ�����   assist[10014]
struct CS_MSG_SELECT_BATCH_SCORE_REQ
{
	short sType; //1����������ѯ�ɼ� 2�������п�Ŀ������ѯ�ɼ�
	char cGrade[31]; //�༶ -- Ϊ�����ѯ���а༶
	unsigned char sSubjectId[MAXSUBJECTSCOUNT]; //ÿ����ĿID
	unsigned char bSubjectCount; //��Ŀ��
	unsigned char cCondition[5]; //��ѯ������ÿһ����Ԫ��һ������ 
	unsigned char bRankFlag; //0û������ 1���� 2���� ���Ǹ����ܷ֣�ûѡ�ܷ�������Ҳ������������
	unsigned char bScoreRange[3]; //������Χ��������Ԫ��Ϊ0����û���ã�Ϊ1����������
	CS_MSG_SELECT_BATCH_SCORE_REQ()
	{
		memset(this, 0, sizeof(CS_MSG_SELECT_BATCH_SCORE_REQ));
	}
};

//������ѯ�ɼ��ظ� -- ÿ��10������   assist[10015]
struct CS_MSG_SELECT_BATCH_SCORE_ACK
{
	short sType; //1����������ѯ�ɼ� 2�������п�Ŀ������ѯ�ɼ�
	unsigned char cCondition[5]; //��ѯ������ÿһ����Ԫ��һ������  -- ��������ֻ������ͷ�����Χ��ѯ����������
	unsigned char bRankFlag; //0û������ 1���� 2���� ȫ�Ƹ����ܷ֣�ûѡ�ܷ�������Ҳ�����������򣬵��Ƹ������Ŀ����
	unsigned int uUserid[MAXBATCHSELECTACKCOUNT]; //�û�id
	char cName[MAXBATCHSELECTACKCOUNT][31]; //����
	char cAccount[MAXBATCHSELECTACKCOUNT][31]; //�˺� 
	char cGrade[MAXBATCHSELECTACKCOUNT][31]; //�༶ 
	short sRank[MAXBATCHSELECTACKCOUNT]; //����
	unsigned short bSum[MAXBATCHSELECTACKCOUNT]; //�ܷ�
	unsigned char bAverage[MAXBATCHSELECTACKCOUNT]; //ƽ����
	unsigned char bSubjectId[MAXBATCHSELECTACKCOUNT][MAXSUBJECTSCOUNT]; //ÿ����ĿID
	unsigned char bScore[MAXBATCHSELECTACKCOUNT][MAXSUBJECTSCOUNT]; //ÿ�Ʒ���
	unsigned char bSubjectCount; //��Ŀ��
	unsigned char bDataRecord[3]; //[0]��ѯ�����¼����<= MAXBATCHSELECTACKCOUNT�� [1]������ţ���1��ʼ�� [2]������ϱ�־ 0û����� 1���
	unsigned char bResCode; //0�ɹ� 1ʧ�� 2���ݿ�û���˺���Ϣ����û�гɼ���Ϣ 3�����쳣
	CS_MSG_SELECT_BATCH_SCORE_ACK()
	{
		memset(this, 0, sizeof(CS_MSG_SELECT_BATCH_SCORE_ACK));
	}
};

//�������³ɼ�����   assist[10016]
struct CS_MSG_UPDATE_SINGLE_SCORE_REQ
{
	short sType; //1���Ƶ������³ɼ� 2�������п�Ŀ�������³ɼ�
	char cAccount[31]; //ͨ���˺Ÿ��ĳɼ�  userid���Զ����ӵģ����Բ�����userid
	unsigned char sSubjectId[MAXSUBJECTSCOUNT]; //ÿ����ĿID
	unsigned char bScore[MAXSUBJECTSCOUNT]; //ÿ�Ʒ���
	unsigned char bSubjectCount; //��Ŀ��
	CS_MSG_UPDATE_SINGLE_SCORE_REQ()
	{
		memset(this, 0, sizeof(CS_MSG_UPDATE_SINGLE_SCORE_REQ));
	}
};

//�������³ɼ��ظ�   assist[10017]
struct CS_MSG_UPDATE_SINGLE_SCORE_ACK
{
	bool bSucceed;
	short sType; //1���Ƶ������³ɼ� 2�������п�Ŀ�������³ɼ�
	char cAccount[31]; //ͨ���˺Ÿ��ĳɼ� 
	CS_MSG_UPDATE_SINGLE_SCORE_ACK()
	{
		memset(this, 0, sizeof(CS_MSG_UPDATE_SINGLE_SCORE_ACK));
	}
};

//ɾ���ɼ�����   assist[10018]
struct CS_MSG_DELETE_SCORE_REQ
{
	short sType; //1����ɾ���ɼ� 2����ɾ���ɼ�
	char cAccount[31]; //����ͨ���˺�ɾ���ɼ�  userid���Զ����ӵģ����Բ�����userid
	unsigned int uUserid[3]; //�û�id��Χ����������ɾ����������Ԫ��Ϊ0����û���ã�Ϊ1����������
	CS_MSG_DELETE_SCORE_REQ()
	{
		memset(this, 0, sizeof(CS_MSG_DELETE_SCORE_REQ));
	}
};

//ɾ���ɼ��ظ�   assist[10019]
struct CS_MSG_DELETE_SCORE_ACK
{
	bool bSucceed;
	short sType; //1����ɾ���ɼ� 2����ɾ���ɼ�
	CS_MSG_DELETE_SCORE_ACK()
	{
		memset(this, 0, sizeof(CS_MSG_DELETE_SCORE_ACK));
	}
};

//������ӳɼ����� -- ÿ��10������    assist[10020]
struct CS_MSG_ADD_BATCH_SCORE_REQ
{
	short sType; //1�����������ӳɼ� 2�������У����߶�ƣ���Ŀ�������ӳɼ�
	char cAccount[MAXBATCHSELECTACKCOUNT][31]; //ͨ���˺���ӳɼ�  ������ݿ�û������˺ţ��͵ô����µ��˺š����Բ���ͨ��userid��userid���Զ����ӵ�
	unsigned char bSubjectId[MAXBATCHSELECTACKCOUNT][MAXSUBJECTSCOUNT]; //��Ŀid
	unsigned char bScore[MAXBATCHSELECTACKCOUNT][MAXSUBJECTSCOUNT]; //��Ŀ����
	unsigned char bSubjectCount; //��Ŀ��
	unsigned char bRecordCount; //�ɼ����ݼ�¼������ҪС�ڵ���MAXBATCHSELECTACKCOUNT
	unsigned char bRecordNO; //�������Σ���1��ʼ
	unsigned char bEnd; //0û�з������ 1���
	CS_MSG_ADD_BATCH_SCORE_REQ()
	{
		memset(this, 0, sizeof(CS_MSG_ADD_BATCH_SCORE_REQ));
	}
};

//������ӳɼ��ظ�  assist[10021]
struct CS_MSG_ADD_BATCH_SCORE_ACK
{
	bool bSucceed; //�ͻ��˽��յ����سɹ����ż�����ʣ�µ����ݸ������
	short sType; //1�����������ӳɼ� 2�������п�Ŀ�������ӳɼ�
	CS_MSG_ADD_BATCH_SCORE_ACK()
	{
		memset(this, 0, sizeof(CS_MSG_ADD_BATCH_SCORE_ACK));
	}
};

#endif