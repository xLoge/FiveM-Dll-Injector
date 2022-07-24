#pragma once

#include <Windows.h>
#include "../header/xorstr.h"

const std::string SelectFile()
{
	char filePath[MAX_PATH];
	OPENFILENAME ofn;
	ZeroMemory(&filePath, sizeof(filePath));
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = 0;
	ofn.lpstrFilter = xorstr_("Dynamic Link Library (*.dll)\0 *.dll\0 All Files (*.*)\0 *.*\0");
	ofn.lpstrFile = filePath;
	ofn.nMaxFile = MAX_PATH;
	ofn.Flags = OFN_DONTADDTORECENT | OFN_FILEMUSTEXIST;
	GetOpenFileNameA(&ofn);
	
	if (filePath)
	{
		return (std::string)(filePath);
	}
	else
	{
		return xorstr_("");
	}
}