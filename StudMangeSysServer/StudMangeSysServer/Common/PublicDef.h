#ifndef __PUBLICDEF_H__
#define __PUBLICDEF_H__

//#define WIN32_LEAN_AND_MEAN
//#include <WinSock2.h>  


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



#endif