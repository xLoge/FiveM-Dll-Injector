#pragma once

int64_t GetBase(DWORD procID, const char* name)
{
	MODULEENTRY32 ModuleEntry = { 0 };
	HANDLE SnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, procID);

	if (!SnapShot) return NULL;

	ModuleEntry.dwSize = sizeof(ModuleEntry);

	if (!Module32First(SnapShot, &ModuleEntry)) return NULL;

	do
	{
		if (strcmp(ModuleEntry.szModule, name) == 0)
		{
			CloseHandle(SnapShot);
			return reinterpret_cast<uint64_t>(ModuleEntry.modBaseAddr);
		}
	} while (Module32Next(SnapShot, &ModuleEntry));

	CloseHandle(SnapShot);
	return NULL;
}

void Injector(char* filePath)
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
	{
		std::cout << "\nFound FiveM! (PID: " << procId << ") Waiting 8 Seconds to prevent errors.";
		Sleep(8000);
	}

	std::string oldFileName = filePath;
	std::string filePathStr = filePath;

	// Get old Filename
	const size_t last_slash_idx = oldFileName.find_last_of("\\/");
	if (std::string::npos != last_slash_idx)
	{
		oldFileName.erase(0, last_slash_idx + 1);
	}

	// Gen new Filename
	std::string newFileName = randStr() + ".dll";

	// Rename to new Filename
	rename(filePath, newFileName.c_str());

	// Get Filepath
	filePathStr = filePathStr.substr(0, filePathStr.find_last_of("\\/")) + "\\";
	
	std::string temp = filePathStr + newFileName;

	// Set new Filepath
	const char* newFilePath = (filePathStr + newFileName).c_str();
	
	HANDLE hProc = OpenProcess(PROCESS_ALL_ACCESS, false, procId);

	if (hProc)
	{
		void* loc = VirtualAllocEx(hProc, 0, MAX_PATH, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

		if (loc)
			WriteProcessMemory(hProc, loc, newFilePath, strlen(newFilePath) + 1, 0);

		HANDLE hThread = CreateRemoteThread(hProc, 0, 0, (LPTHREAD_START_ROUTINE)LoadLibraryA, loc, 0, 0);

		if (hThread)
			CloseHandle(hThread);
	}
	
	if (hProc)
		CloseHandle(hProc);

	// Bypass

	HANDLE hBypass = OpenProcess(PROCESS_ALL_ACCESS, FALSE, procId);

	if (hBypass)
	{
		const bool ShAllowed = true;
		static uint64_t bypass = GetBase(procId, ("gta-core-five.dll"));
		void* AllowScriptHook = (BYTE*)(bypass + 0xDCAB8);
		WriteProcessMemory(hBypass, AllowScriptHook, &ShAllowed, sizeof(ShAllowed), NULL);
	}
	
	rename(newFilePath, oldFileName.c_str());

	return;
}
