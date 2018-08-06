#ifndef __BASEPROC_H__
#define __BASEPROC_H__


#include <iostream>
#include <map>
#include "ProcDef.h"

using namespace std;


class BaseProc 
{
public:
	BaseProc(ProcDef nProcDef);
	~BaseProc();

	//�����๫���ӿ�
public:
	void OnStart(bool bChooseAgain = false); //�������̽ӿ�

	virtual void StartProc();
	virtual void EndProc();

	virtual void StartRecv(void* vpData, unsigned int DataLen, /*int iMianId,*/ int iAssistId);
	virtual void EndRecv();

	void SetIEndFlag(int iEndFlag);
	int GetIEndFlag();

	virtual bool initMapChoose() = 0;

	virtual void SwitchToOper(OperPermission CurOper); //�����ڲ������л����������в����Ľ��棬������ָ��ɾ��ĵ���Щ���� ��������ǰ���̣��л�������������ǰ�����̡�����ʱ��һ��Ҫ����EndRecv������ݣ�

	OperPermission GetCurOper();
	void SetCurOper(OperPermission CurOper);
	
	ProcDef GetNextProc(); //���ݵ�ǰ�����ȷ�����һ�����̵ı�ʶ
	ProcDef GetProcDefByOper(OperPermission Oper);
	ProcDef GetProcDefByRealChoose(int iRealChoose);
	OperPermission GetOperPerByRealChoose(int iRealChoose);

	bool IsShow();
	bool IsRunning();
	ProcDef GetMyProcDef();

	//m_mChoose��keyֵ�������Сֵ
	int& GetMinRealChoose();
	int& GetMaxRealChoose();

	int GetMyChoose();

	int GetRealChooseByUserChoose(int iChoose);//ͨ���û������Ȩ��Ӱ�����choose���ҵ�m_mChoose��Ӧ������keyֵ
	int GetUserMaxChoose(); //��ȡ�û���ѡ��������ѡ��ֵ

	void ExitSys(); //���������˳�ϵͳ

	void OperInputErrorHandle(bool bFlag=true, int uMaxInputErrorCount=OPERINPUTERRORMAXLIMIT); //���������������� Ҫ����ֻ������ѡ�����͵��˷��ؽ����ѡ�����bFlag������ڿ����Ƿ���Ҫʹ�øú������л�����

	//����˽�нӿ�
protected:
	//���ؽ������
	bool NotifyUserExitSysRecvHandle(void* vpData, unsigned int DataLen); //֪ͨ�û��˳�ϵͳ

protected:
	//key����realChoose
	map<int, ChooseData> m_mChoose;

	//���û�����Ȩ�����ƣ�������Ӧm_mChoose�е�keyֵ������Ҫת��
	int m_iMyChoose;

	//�����������������ƣ���ת�ص�ԭ����
	int m_iOperInputErrorLimit;
private:
	bool m_IsShow;
	bool m_IsRunning; //������Ϣʱ����⵽���еĽ��̣��Ͱ������ϢͶ�ݵ���Ӧ���̵�StartRecv����StartRecv�ٸ���Э��id����ȡ����Ϣ  --��ʵ�о������Ӳ��Ǻܺá�һ�㶼���ȸ���Э��id��������������
	OperPermission m_CurOper; //���еĲ���
	int m_iEndFlag; //0û���� 1��������  -1����������  Ϊ�˷�ֹ����һ������һ�ν��ܽ��̾���ɶ���Ƶģ�������Ӧ�������

	//m_mChoose��keyֵ�������Сֵ
	int m_iMinRealChoose;
	int m_iMaxRealChoose;

	bool m_IsChooseAgain;

	const ProcDef m_MyProcDef;

};

#endif