#include "APPRun.h"


APPRun::APPRun()
{

}

APPRun::~APPRun()
{

}

void APPRun::Run()
{
	//tcp����
	if (!TCPHandle::GetInstance()->TCPConn())
	{
		cout<< " TCP connect fail, exit system" <<endl;
  		system("pause");
		exit(0);
	}

	//�����й��ܽ������ѹ������
	ProcMgr::GetInstance()->initProc();

	//����������Ϣ�̺߳ͷ����������߳� --�������̺߳������ϣ�������������
	HANDLE recvThread = CreateThread(NULL, 0, RecvThread, NULL, 0, NULL);

	//������¼ע����̣�֮���ٻ�ȡ����Ȩ�ޣ�����Ȩ�޲���ҪŪ��һ���࣬����ֱ���ڵ�¼ע��������洦��
	ProcMgr::GetInstance()->ProcSwitch(PROC_DEF_ENTERSYSPROC);

	/*
	* ��ع��ܽ������ - - ��������������ʱ����ʹ�ý��̹�������ʵ��������ͨ��ָ�루���ã���ȡ���̹�������Ľ��̹�������Ա����������м�أ�
	*/
	ProcMgr::GetInstance()->Notify();
}