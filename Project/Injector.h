#pragma once

void Injector(const char* filepath)
{
	DWORD procId = NULL;

	bool IsWaiting = false;

	while (true)
	{
		CheckProc("FiveM_GameProcess.exe", procId);
		CheckProc("FiveM_b2060_GameProcess.exe", procId);
		CheckProc("FiveM_b2189_GameProcess.exe", procId);
		CheckProc("FiveM_b2372_GameProcess.exe", procId);
		CheckProc("FiveM_b2545_GameProcess.exe", procId);

		CheckProc("FiveM_GTAProcess.exe", procId);
		CheckProc("FiveM_b2060_GTAProcess.exe", procId);
		CheckProc("FiveM_b2189_GTAProcess.exe", procId);
		CheckProc("FiveM_b2372_GTAProcess.exe", procId);
		CheckProc("FiveM_b2545_GTAProcess.exe", procId);

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