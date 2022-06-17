#pragma once

void CheckProc(const char* procName, DWORD &procId, int ms)
{
	if (GetProcId(procName))
	{
		procId = GetProcId(procName);
		return;
	}
	else
	{
		Sleep(ms);
		return;
	}
}
