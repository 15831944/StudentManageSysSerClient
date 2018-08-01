
#ifndef __PROTODEF_H__
#define __PROTODEF_H__

//����Ŀ��
#define MAXSUBJECTSCOUNT 20
//���������ջ��߷��ͼ�¼��
#define MAXBATCHREQACKCOUNT 10
//������Ȩ����Ŀ
#define MAXAUTHORITBYCOUNT 30

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
	ASSIST_ID_EXIT_SYS_REQ							 =   10005, //�˳�ϵͳ  -- �ò������Э��ID
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
	ASSIST_ID_GET_AUTHORITY_REQ						 =   10022, //��ȡ�����ɾ��������ӵ�Ȩ������
	ASSIST_ID_GET_AUTHORITY_ACK						 =   10023, //��ȡ�����ɾ��������ӵ�Ȩ�޻ظ�
	ASSIST_ID_EDIT_AUTHORITY_REQ					 =   10024, //ɾ�������Ӷ���Ȩ������
	ASSIST_ID_EDIT_AUTHORITY_ACK					 =   10025, //ɾ�������Ӷ���Ȩ�޻ظ�
	ASSIST_ID_ADD_SINGLE_USERINFO_REQ				 =   10026, //���������û���Ϣ����
	ASSIST_ID_ADD_SINGLE_USERINFO_ACK				 =   10027, //���������û���Ϣ�ظ�
	ASSIST_ID_ADD_BATCH_USERINFO_REQ				 =   10028, //���������û���Ϣ����
	ASSIST_ID_ADD_BATCH_USERINFO_ACK				 =   10029, //���������û���Ϣ�ظ�


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

//�˳�ϵͳ   assist[10005]   -- �ò������Э��
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
	unsigned int uUserid[MAXBATCHREQACKCOUNT]; //�û�id
	char cName[MAXBATCHREQACKCOUNT][31]; //����
	char cAccount[MAXBATCHREQACKCOUNT][31]; //�˺� 
	char cGrade[MAXBATCHREQACKCOUNT][31]; //�༶ 
	short sRank[MAXBATCHREQACKCOUNT]; //����
	unsigned short bSum[MAXBATCHREQACKCOUNT]; //�ܷ�
	unsigned char bAverage[MAXBATCHREQACKCOUNT]; //ƽ����
	unsigned char bSubjectId[MAXBATCHREQACKCOUNT][MAXSUBJECTSCOUNT]; //ÿ����ĿID
	unsigned char bScore[MAXBATCHREQACKCOUNT][MAXSUBJECTSCOUNT]; //ÿ�Ʒ���
	unsigned char bSubjectCount; //��Ŀ��
	unsigned char bDataRecord[3]; //[0]��ѯ�����¼����<= MAXBATCHREQACKCOUNT�� [1]������ţ���1��ʼ�� [2]������ϱ�־ 0û����� 1���
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
	char cAccount[MAXBATCHREQACKCOUNT][31]; //ͨ���˺���ӳɼ�  ������ݿ�û������˺ţ��͵ô����µ��˺š����Բ���ͨ��userid��userid���Զ����ӵ�
	unsigned char bSubjectId[MAXBATCHREQACKCOUNT][MAXSUBJECTSCOUNT]; //��Ŀid
	unsigned char bScore[MAXBATCHREQACKCOUNT][MAXSUBJECTSCOUNT]; //��Ŀ����
	unsigned char bSubjectCount; //��Ŀ��
	unsigned char bRecordCount; //�ɼ����ݼ�¼������ҪС�ڵ���MAXBATCHREQACKCOUNT
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
	bool bSucceed;//�ͻ��˽��յ����سɹ����ż�����ʣ�µ����ݸ������
	short sType; //1�����������ӳɼ� 2�������п�Ŀ�������ӳɼ�
	CS_MSG_ADD_BATCH_SCORE_ACK()
	{
		memset(this, 0, sizeof(CS_MSG_ADD_BATCH_SCORE_ACK));
	}
};

//�����˺Ż�ȡ�����ɾ�����߿����ӵ�Ȩ������ assist[10022]
struct CS_MSG_GET_AUTHORITY_REQ
{
	unsigned char sType; //1��ɾ���� 2�����ӵ�
	char cAccount[31]; //���������˺�
	CS_MSG_GET_AUTHORITY_REQ()
	{
		memset(this, 0, sizeof(CS_MSG_GET_AUTHORITY_REQ));
	}
};

//�����˺Ż�ȡ�����ɾ�����߿����ӵ�Ȩ�޻ظ� assist[10023]
struct CS_MSG_GET_AUTHORITY_ACK
{
	bool bSucceed;
	unsigned char sType; //1��ɾ���� 2�����ӵ�
	char cAccount[31]; //���������˺�
	unsigned char cAuthority[MAXAUTHORITBYCOUNT]; //����Ȩ��
	unsigned char cAuthorityCount; //Ȩ������
	CS_MSG_GET_AUTHORITY_ACK()
	{
		memset(this, 0, sizeof(CS_MSG_GET_AUTHORITY_ACK));
	}
};

//�����˺�����ɾ����Ȩ������ assist[10024]
struct CS_MSG_EDIT_AUTHORITY_REQ
{
	unsigned char sType; //1ɾ��Ȩ�� 2����Ȩ��
	char cAccount[31]; //���������˺�
	unsigned char cAuthority[MAXAUTHORITBYCOUNT]; //���ӻ�ɾ������Ĳ���Ȩ��
	unsigned char cAuthorityCount; //Ȩ������
	CS_MSG_EDIT_AUTHORITY_REQ()
	{
		memset(this, 0, sizeof(CS_MSG_EDIT_AUTHORITY_REQ));
	}
};

//�����˺�����ɾ����Ȩ�޻ظ� assist[10025]
struct CS_MSG_EDIT_AUTHORITY_ACK
{
	bool bSucceed;
	unsigned char sType; //1ɾ��Ȩ�� 2����Ȩ��
	char cAccount[31]; //���������˺�
	CS_MSG_EDIT_AUTHORITY_ACK()
	{
		memset(this, 0, sizeof(CS_MSG_EDIT_AUTHORITY_ACK));
	}
};

//���������û���Ϣ����   assist[10026]
struct CS_MSG_ADD_SINGLE_USERINFO_REQ
{
	char cAccount[31];
	char cPWD[31];
	char cName[31];
	unsigned char sSex;
	unsigned char sIdent;
	char cMajor[41];
	char cGrade[31];
	CS_MSG_ADD_SINGLE_USERINFO_REQ()
	{
		memset(this, 0, sizeof(CS_MSG_ADD_SINGLE_USERINFO_REQ));
	}
};

//���������û���Ϣ�ظ�   assist[10027]
struct CS_MSG_ADD_SINGLE_USERINFO_ACK
{
	bool bSucceed;
	char cAccount[31];
	CS_MSG_ADD_SINGLE_USERINFO_ACK()
	{
		memset(this, 0, sizeof(CS_MSG_ADD_SINGLE_USERINFO_ACK));
	}
};

//���������û���Ϣ���� -- ÿ��10������   assist[10028]
struct CS_MSG_ADD_BATCH_USERINFO_REQ
{
	char cAccount[MAXBATCHREQACKCOUNT][31];
	char cPWD[MAXBATCHREQACKCOUNT][31];
	char cName[MAXBATCHREQACKCOUNT][31];
	unsigned char sSex[MAXBATCHREQACKCOUNT];
	unsigned char sIdent[MAXBATCHREQACKCOUNT];
	char cMajor[MAXBATCHREQACKCOUNT][41];
	char cGrade[MAXBATCHREQACKCOUNT][31];
	unsigned char bRecordCount; //�������ݼ�¼������ҪС�ڵ���MAXBATCHREQACKCOUNT
	unsigned char bRecordNO; //�������Σ���1��ʼ
	unsigned char bEnd; //0û�з������ 1���
	CS_MSG_ADD_BATCH_USERINFO_REQ()
	{
		memset(this, 0, sizeof(CS_MSG_ADD_BATCH_USERINFO_REQ));
	}
};

//���������û���Ϣ�ظ� -- ÿ��10������   assist[10029]
struct CS_MSG_ADD_BATCH_USERINFO_ACK
{
	bool bSucceed; //�ͻ��˽��յ����سɹ����ż�����ʣ�µ����ݸ������ --�������ȫ�����ʧ�ܣ��ͷ���ʧ�ܣ������ǳɹ�
	unsigned char bSucceedRecordCount; //���γɹ����ӵ����ݿ��������ҪС�ڵ���MAXBATCHREQACKCOUNT
	CS_MSG_ADD_BATCH_USERINFO_ACK()
	{
		memset(this, 0, sizeof(CS_MSG_ADD_BATCH_USERINFO_ACK));
	}
};

#endif