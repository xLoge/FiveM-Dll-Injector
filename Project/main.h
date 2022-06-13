#pragma once
#include <Windows.h>
#include <iostream>
#include <TlHelp32.h>

DWORD procId;

DWORD GetProcId(const char* procName)
{
	DWORD lProcId = 0;
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	if (hSnap != INVALID_HANDLE_VALUE)
	{
		PROCESSENTRY32 procEntry;
		procEntry.dwSize = sizeof(procEntry);

		if (Process32First(hSnap, &procEntry))
		{
			do
			{
				if (!_stricmp(procEntry.szExeFile, procName))
				{
					lProcId = procEntry.th32ProcessID;
					break;
				}
			}
			while (Process32Next(hSnap, &procEntry));
		}
	}
	CloseHandle(hSnap);
	return lProcId;
}

void CheckProc(const char* ProcName)
{
	if (GetProcId(ProcName)) 
	{
		procId = GetProcId(ProcName);
		return;
	}
	Sleep(25);
	return;
}

void Inject(const char* filepath)
{
	bool IsWaiting = false;

	while (true)
	{
		CheckProc("FiveM_GameProcess.exe");
		CheckProc("FiveM_b2060_GameProcess.exe");
		CheckProc("FiveM_b2189_GameProcess.exe");
		CheckProc("FiveM_b2372_GameProcess.exe");
		CheckProc("FiveM_b2545_GameProcess.exe");

		CheckProc("FiveM_GTAProcess.exe");
		CheckProc("FiveM_b2060_GTAProcess.exe");
		CheckProc("FiveM_b2189_GTAProcess.exe");
		CheckProc("FiveM_b2372_GTAProcess.exe");
		CheckProc("FiveM_b2545_GTAProcess.exe");

		if (procId) break;

		if (!IsWaiting)
		{
			std::cout << "Waiting for FiveM ...";
			IsWaiting = true;
		}
	}

	if (IsWaiting)
	{
		std::cout << "\nFound FiveM! (PID: " << procId << ") Waiting 5 Seconds to prevent errors";
		Sleep(5000);
	}
	
	HANDLE hProc = OpenProcess(PROCESS_ALL_ACCESS, false, procId);

	if (hProc != INVALID_HANDLE_VALUE)
	{
		void* loc = VirtualAllocEx(hProc, 0, MAX_PATH, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

		if (loc)
			WriteProcessMemory(hProc, loc, filepath, strlen(filepath) + 1, 0);

		HANDLE hThread = CreateRemoteThread(hProc, 0, 0, (LPTHREAD_START_ROUTINE)LoadLibraryA, loc, 0, 0);

		if (hThread)
			CloseHandle(hThread);
	}

	if (hProc)
		CloseHandle(hProc);

	return;
}

const char* SelectFile()
{
	char filePath[MAX_PATH];
	OPENFILENAME ofn;
	ZeroMemory(&filePath, sizeof(filePath));
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;
	ofn.lpstrFilter = "Dynamic Link Library (*.dll)\0 * .dll\0";
	ofn.lpstrFile = filePath;
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrTitle = "Select File";
	ofn.Flags = OFN_DONTADDTORECENT | OFN_FILEMUSTEXIST;
	if (!GetOpenFileNameA(&ofn))
		switch (CommDlgExtendedError())
		{
			case NULL:
			default: return NULL;
		}

	std::cout << "File: \"" << filePath << "\"\n\n";
	return filePath;
}
