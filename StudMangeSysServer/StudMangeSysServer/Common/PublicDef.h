#ifndef __PUBLICDEF_H__
#define __PUBLICDEF_H__

#include <string>
using namespace std;

//����Ȩ�޺� �����˶�Ӧ  ��¼��ע�����Ȩ�ޣ�ÿ����Ҷ�ӵ��
enum OperPermission
{
	OPER_PER_INVALID        =    0,
	OPER_PER_START	        =    0, //��Чֵ��ʼֵ
	OPER_PER_LOGIN          =    1, 
	OPER_PER_REGISTER       =    2,

	OPER_PER_END                    //��Чֵ��ֵֹ
};

enum IdentType
{
	IDENT_TYPE_INVALID      =    0,
	IDENT_TYPE_START	    =    0, //��Чֵ��ʼֵ
	IDENT_TYPE_STUDENT      =    1, 
	IDENT_TYPE_TEACHER      =    2,
	IDENT_TYPE_ADMIN        =    3,

	IDENT_TYPE_END                    //��Чֵ��ֵֹ
};

struct UserInfo
{
	//�û�����¼
	unsigned int iUserId;
	string strAccount;
	string strIP;
	unsigned short sPort;
	string strAuthority;


	//�û���̬��¼
	short sRegNeedCount;  //ע����ʱ����3�����ݿ������ ����ͻ��˴�������Ϣ  ����userid����ݱ�ʶ  �����������Ȩ���ٲ������ݿ�

	UserInfo()
	{
		iUserId = 0;
		strAccount = "";
		strIP = "";
		sPort = 0;
		strAuthority = "";


		sRegNeedCount = 0;
	}
};


#endif