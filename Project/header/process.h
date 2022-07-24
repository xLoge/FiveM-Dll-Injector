#pragma once

#include <Windows.h>
#include <TlHelp32.h>

namespace Process
{
	std::string lastOpenedName;

	DWORD FindProcess(const char* Name)
	{
		HANDLE snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

		PROCESSENTRY32 pe;
		pe.dwSize = sizeof(PROCESSENTRY32);

		if (Process32First(snap, &pe))
		{
			do
			{
				if (std::string(pe.szExeFile).find(Name) != std::string::npos)
				{
					CloseHandle(snap);
					lastOpenedName = Name;
					return pe.th32ProcessID;
				}
			} while (Process32Next(snap, &pe));
		}

		CloseHandle(snap);
		return 0;
	}
}
