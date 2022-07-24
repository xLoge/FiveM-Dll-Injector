#include "../header/include.h"

bool Inject(const std::string& filePath)
{
	DWORD pidF(0), pidFC(0);
	bool once(true);

	while (true)
	{
		DWORD proc(Process::FindProcess(xorstr_("GTAProcess")));

		if (proc)
		{
			pidF = proc;
			break;
		}
		else
		{
			DWORD proc(Process::FindProcess(xorstr_("GameProcess")));
			if (proc)
			{
				pidF = proc;
				break;
			}
		}

		Sleep(100);

		if (once)
		{
			if (pidF)
			{
				break;
			}
			else
			{
				std::cout << xorstr_("File: \"") << filePath << xorstr_("\"\n\n") << xorstr_("Waiting for FiveM ...\n");
				once = false;
			}
		}
	}

	if (!once)
	{
		std::cout << xorstr_("Found FiveM! (PID: ") << pidF << xorstr_(") Waiting for FiveM_ChromeBrowser to prevent errors.\n");

		while (true)
		{
			DWORD procIdChrome(Process::FindProcess(xorstr_("ChromeBrowser")));
			if (procIdChrome)
			{
				pidFC = procIdChrome;
				break;
			}
			Sleep(100);
		}

		std::cout << xorstr_("Found FiveM_ChromeBrowser! (PID: ") << pidFC << xorstr_(") Waiting Eight Seconds to prevent crashing.\n");

		Sleep(8000);
	}

	// ---------------------
	// Rename File:
	// ---------------------

	std::string oldFileName(filePath), filePathStr(filePath), newFileName(gen::string() + xorstr_(".dll"));

	// Get Old Filename
	const size_t last(oldFileName.find_last_of(xorstr_("\\/")));
	if (last != std::string::npos)
	{
		oldFileName.erase(0, last + 1);
	}

	// Rename to new Filename
	if (rename(filePathStr.c_str(), newFileName.c_str()))
	{
		return true;
	}

	// Get Filepath
	filePathStr = filePathStr.substr(0, filePathStr.find_last_of(xorstr_("\\/"))) + xorstr_("\\");

	// Set new Filepath
	const char* newFilePath = (filePathStr + newFileName).c_str();

	// ---------------------
	// Inject File:
	// ---------------------

	HANDLE hFiveM(OpenProcess(PROCESS_ALL_ACCESS, false, pidF));
	
	void* Alloc(VirtualAllocEx(hFiveM, 0, MAX_PATH, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE));

	if (!hFiveM or !Alloc)
	{
		return true;
	}

	if (!WriteProcessMemory(hFiveM, Alloc, newFilePath, strlen(newFilePath) + 1, 0))
	{
		VirtualFreeEx(hFiveM, Alloc, 0, MEM_RELEASE);
		return true;
	}

	HANDLE hThread(CreateRemoteThread(hFiveM, 0, 0, (LPTHREAD_START_ROUTINE)LoadLibraryA, Alloc, 0, 0));
	
	if (hThread)
	{
		WaitForSingleObject(hThread, INFINITE);
		CloseHandle(hThread);
	}

	VirtualFreeEx(hFiveM, Alloc, 0, MEM_RELEASE);

	CloseHandle(hFiveM);

	if (rename(newFilePath, oldFileName.c_str()))
	{
		return true;
	}

	system(xorstr_(">nul 2>nul @taskkill /f /im SearchFilterHost.exe >nul 2>nul & @taskkill /f /im SearchProtocolHost.exe >nul 2>nul & @taskkill /f /im SearchIndexer.exe >nul 2>nul & @taskkill /f /im GameBar.exe >nul 2>nul & @taskkill /f /im gamingservices.exe >nul 2>nul"));

	return false;
}

int main()
{
	SetConsoleTitleA(xorstr_(""));

	// This bottom line is your Dll path (You can change it)
	//const std::string file = "C:\\UGay.dll";

	const std::string file = SelectFile();

	if (file != xorstr_(""))
	{
		if (Inject(file))
		{
			MessageBoxA(nullptr, xorstr_("Injection failed!"), xorstr_("Error"), MB_OK | MB_ICONERROR | MB_APPLMODAL);
			return 1;
		}
		return 0;
	}
	else
	{
		return 1;
	}
}