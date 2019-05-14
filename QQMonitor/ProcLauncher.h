#pragma once
#include "Main.h"

class ProcLauncher
{
private:
	TCHAR* QQPath;
public:
	ProcLauncher(TCHAR* QQPath);
	ProcLauncher();
	~ProcLauncher();
	BOOL LaunchProc();
};

