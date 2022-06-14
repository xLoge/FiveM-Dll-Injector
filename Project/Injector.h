#pragma once

void Injector(const char* filePath)
{
	DWORD procId = 0;
	static int ms = 1;
	bool once = 1;

	while (!procId)
	{		
		CheckProc("FiveM_GameProcess.exe", procId, ms);
		CheckProc("FiveM_b2060_GameProcess.exe", procId, ms);
		CheckProc("FiveM_b2189_GameProcess.exe", procId, ms);
		CheckProc("FiveM_b2372_GameProcess.exe", procId, ms);
		CheckProc("FiveM_b2545_GameProcess.exe", procId, ms);
		CheckProc("FiveM_GTAProcess.exe", procId, ms);
		CheckProc("FiveM_b2060_GTAProcess.exe", procId, ms);
		CheckProc("FiveM_b2189_GTAProcess.exe", procId, ms);
		CheckProc("FiveM_b2372_GTAProcess.exe", procId, ms);
		CheckProc("FiveM_b2545_GTAProcess.exe", procId, ms);

		if (once)
		{
			if (procId)
				break;
			else
			{
				std::cout << "File: \"" << filePath << "\"\n\n";
				std::cout << "Waiting for FiveM ...";
				ms = 30;
				once = false;
			}
		}
	}

	if (!once)
		std::cout << "\nFound FiveM! (PID: " << procId << ") Waiting 8 Seconds to prevent errors."; Sleep(8000);

	HANDLE hProc = OpenProcess(PROCESS_ALL_ACCESS, false, procId);

	if (hProc && hProc != INVALID_HANDLE_VALUE)
	{
		void* loc = VirtualAllocEx(hProc, 0, MAX_PATH, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

		if (loc)
			WriteProcessMemory(hProc, loc, filePath, strlen(filePath) + 1, 0);

		HANDLE hThread = CreateRemoteThread(hProc, 0, 0, (LPTHREAD_START_ROUTINE)LoadLibraryA, loc, 0, 0);

		if (hThread)
			CloseHandle(hThread);
	}
	if (hProc)
		CloseHandle(hProc);
	
	return;
}
