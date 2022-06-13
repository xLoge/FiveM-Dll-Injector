#pragma once
#include <Windows.h>
#include <iostream>
#include <TlHelp32.h>

DWORD procId = 0;

DWORD GetProcId(const char* procName)
{
	DWORD procId = 0;
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
					procId = procEntry.th32ProcessID;
					break;
				}
			} while (Process32Next(hSnap, &procEntry));
		}
	}
	CloseHandle(hSnap);
	return procId;
}

void CheckProc(const char* ProcName)
{
	procId = GetProcId(ProcName);
	if (procId) return;
	procId = 0;
	Sleep(25);
	return;
}

void Inject(const char* filename)
{
	bool IsWaiting = false;

	while (!procId)
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

		if (procId) break; // Without the code will allways wait 5 Seconds

		if (!IsWaiting)
		{
			std::cout << "Waiting for FiveM ...\n";
			IsWaiting = true;
		}
	}

	std::cout << "Found FiveM! (" << procId << ")";

		if (IsWaiting)
		{
			std::cout << " Waiting 5 Seconds to prevent errors\n";
			Sleep(5000);
		}
		else
			std::cout << "\n";
	
	HANDLE hProc = OpenProcess(PROCESS_ALL_ACCESS, 0, procId);

	if (hProc && hProc != INVALID_HANDLE_VALUE)
	{
		void* loc = VirtualAllocEx(hProc, 0, MAX_PATH, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

		if (loc)
		{
			WriteProcessMemory(hProc, loc, filename, strlen(filename) + 1, 0);
		}

		HANDLE hThread = CreateRemoteThread(hProc, 0, 0, (LPTHREAD_START_ROUTINE)LoadLibraryA, loc, 0, 0);

		if (hThread)
		{
			CloseHandle(hThread);
		}
	}

	if (hProc)
	{
		CloseHandle(hProc);
	}

	return;
}

void Start()
{
	// File Select

	char file[MAX_PATH] = "";
	OPENFILENAME ofn;
	ZeroMemory(&file, sizeof(file));
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;
	ofn.lpstrFilter = "Dynamic Link Library (*.dll)\0 * .dll\0";
	ofn.lpstrFile = file;
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrTitle = "Select DLL";
	ofn.Flags = OFN_DONTADDTORECENT | OFN_FILEMUSTEXIST;
	if (!GetOpenFileNameA(&ofn))
	{
		switch (CommDlgExtendedError())
		{
		default: return;
		}
	}

	// Inject

	Inject(file);
	
	return;
}