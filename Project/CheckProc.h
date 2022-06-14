#pragma once

void CheckProc(const char* ProcName, DWORD& procId)
{
	if (GetProcId(ProcName))
	{
		procId = GetProcId(ProcName);
		return;
	}
	Sleep(50);
	return;
}