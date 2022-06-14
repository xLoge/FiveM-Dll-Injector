#pragma once

void SelectFile(const char*& fileVar)
{
	char filePath[MAX_PATH];
	OPENFILENAME ofn;
	ZeroMemory(&filePath, sizeof(filePath));
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;
	ofn.lpstrFilter = "Dynamic Link Library (*.dll)\0 *.dll\0 All Files (*.*)\0 *.*\0";
	ofn.lpstrFile = filePath;
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrTitle = "Select File";
	ofn.Flags = OFN_DONTADDTORECENT | OFN_FILEMUSTEXIST;
	if (!GetOpenFileNameA(&ofn))
		switch (CommDlgExtendedError())
		{
			case NULL:
			default:
				fileVar = NULL;
				return;
		}
	fileVar = filePath;
	return;
}
