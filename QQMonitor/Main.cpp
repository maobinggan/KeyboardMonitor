#include "Main.h"

//隐藏控制台黑窗口!
#ifndef _DEBUG
#pragma comment( linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" )
#endif // _DEBUG

int main()
{
#ifdef _DEBUG
	PTCHAR QQPath = (PTCHAR)TEXT("C:\\Program Files (x86)\\Tencent\\QQ\\Bin\\QQ.exe");
#else
	PTCHAR QQPath = (PTCHAR)TEXT("QQ.exe");
#endif // _DEBUG

	//启动
	Monitor monitor = Monitor(QQPath);
	monitor.Start();

}
