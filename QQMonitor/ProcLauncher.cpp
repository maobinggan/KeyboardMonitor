#include "ProcLauncher.h"

//构造、析构
ProcLauncher::ProcLauncher() {}
ProcLauncher::ProcLauncher(TCHAR* QQPath)
{
	ProcLauncher::QQPath = QQPath;
}
ProcLauncher::~ProcLauncher() {}

//启动QQ
BOOL ProcLauncher::LaunchProc()
{
	STARTUPINFO si = { 0 }; PROCESS_INFORMATION pi; si.cb = sizeof(STARTUPINFO);
	BOOL res = CreateProcess(QQPath, NULL, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);
	if (res == 0x0) { return FALSE; }
	return TRUE;
}



