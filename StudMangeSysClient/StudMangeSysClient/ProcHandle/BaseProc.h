#ifndef __BASEPROC_H__
#define __BASEPROC_H__

#include <iostream>
#include <map>
#include "ProcDef.h"

using namespace std;

class ProcMgr;

class BaseProc
{
public:
	BaseProc(ProcDef nProcDef);
	~BaseProc();

	void OnStart(bool bChooseAgain = false); //�������̽ӿ�

	virtual void StartProc();
	virtual void EndProc();

	virtual void StartRecv(void* vpData, unsigned int DataLen, /*int iMianId,*/ int iAssistId) = 0;
	virtual void EndRecv();

	virtual bool initMapChoose() = 0;

	virtual void SwitchToOper(OperPermission CurOper) = 0; //�����ڲ������л����������в����Ľ��棬������ָ��ɾ��ĵ���Щ����

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

	int GetRealChooseByUserChoose(int iChoose);//ͨ���û������Ȩ��Ӱ�����choose���ҵ�m_mChoose��Ӧ������keyֵ
	int GetUserMaxChoose(); //��ȡ�û���ѡ��������ѡ��ֵ

	void ExitSys();

protected:
	//key����realChoose
	map<int, ChooseData> m_mChoose;

	//���û�����Ȩ�����ƣ�������Ӧm_mChoose�е�keyֵ������Ҫת��
	int m_iMyChoose;

private:
	bool m_IsShow;
	bool m_IsRunning; //������Ϣʱ����⵽���еĽ��̣��Ͱ������ϢͶ�ݵ���Ӧ���̵�StartRecv����StartRecv�ٸ���Э��id����ȡ����Ϣ  --��ʵ�о������Ӳ��Ǻܺá�һ�㶼���ȸ���Э��id��������������
	OperPermission m_CurOper; //���еĲ���

	//m_mChoose��keyֵ�������Сֵ
	int m_iMinRealChoose;
	int m_iMaxRealChoose;

	bool m_IsChooseAgain;

	const ProcDef m_MyProcDef;
};

#endif