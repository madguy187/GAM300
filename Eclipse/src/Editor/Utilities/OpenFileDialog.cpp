#include "pch.h"
#include "OpenFileDialog.h"

char* FileDialog::FileBrowser()
{
	OPENFILENAMEA openFileName;
	char fileName[MAX_PATH] = "";
	wchar_t wFilename[MAX_PATH] = L"";
	mbstowcs(wFilename, fileName, strlen(fileName) + 1);//Plus null
	ZeroMemory(&openFileName, sizeof(openFileName));
	openFileName.lStructSize = sizeof(OPENFILENAME);
	openFileName.lpstrFilter = "All Files\0*.*\0C++ Files\0*.cpp\0Header Files\0*.h\0";
	openFileName.lpstrFile = fileName;
	openFileName.nMaxFile = MAX_PATH;
	openFileName.nFilterIndex = 1;
	std::filesystem::path _originPath{ std::filesystem::current_path() };
	if (GetOpenFileNameA(&openFileName))
	{
		std::filesystem::current_path(_originPath);
		//open file (de-serialize here)
	}
	return nullptr;
}

char* FileDialog::SaveFile()
{
	OPENFILENAMEA openFileName;
	char fileName[MAX_PATH] = "";
	wchar_t wFilename[MAX_PATH] = L"";
	mbstowcs(wFilename, fileName, strlen(fileName) + 1);//Plus null
	ZeroMemory(&openFileName, sizeof(openFileName));
	openFileName.lStructSize = sizeof(OPENFILENAME);
	openFileName.lpstrFilter = "All Files\0*.*\0C++ Files\0*.cpp\0Header Files\0*.h\0";
	openFileName.lpstrFile = fileName;
	openFileName.nMaxFile = MAX_PATH;
	openFileName.nFilterIndex = 1;
	std::filesystem::path _originPath{ std::filesystem::current_path() };

	if (GetSaveFileNameA(&openFileName))
	{
		std::filesystem::path _path{ fileName };
		std::string _fileDirName = _path.replace_extension("").filename().string();

		std::filesystem::current_path(_originPath);
		std::filesystem::path _dataDir{ "Data" };

		if (!std::filesystem::exists(_dataDir))
		{
			std::filesystem::create_directory(_dataDir);
		}

		std::filesystem::path _fileDir{ ((_dataDir.string() + "\\") + _fileDirName) + "\\" };

		if (!std::filesystem::exists(_fileDir))
		{
			std::filesystem::create_directory(_fileDir);
		}
		//save file  (serialize here)
	}
	return nullptr;
}
