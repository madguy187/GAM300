#include "pch.h"
#include "OpenFileDialog.h"


std::string FileDialog::FileBrowser()
{
	OPENFILENAMEA openFileName;
	char fileName[MAX_PATH] = "";
	wchar_t wFilename[MAX_PATH] = L"";
	mbstowcs(wFilename, fileName, strlen(fileName) + 1);//Plus null
	ZeroMemory(&openFileName, sizeof(openFileName));
	openFileName.lStructSize = sizeof(OPENFILENAME);
	openFileName.lpstrFile = fileName;
	openFileName.lpstrFile[0] = '\0';
	openFileName.nMaxFile = MAX_PATH;
	openFileName.lpstrFilter = "All Files\0*.*\0C++ Files\0*.cpp\0Header Files\0*.h\0";
	openFileName.nFilterIndex = 1;
	openFileName.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
	std::filesystem::path _originPath{ std::filesystem::current_path() };
	if (GetOpenFileNameA(&openFileName))
	{
		std::filesystem::current_path(_originPath);
		//open file (de-serialize here)
		std::filesystem::path tempPath = fileName;
		
		EDITOR_LOG_INFO(tempPath.string().c_str());
		return tempPath.string();
	}
	return std::string();
}
std::string FileDialog::SaveFile()
{
	OPENFILENAMEA openFileName;
	char fileName[MAX_PATH] = "";
	wchar_t wFilename[MAX_PATH] = L"";
	mbstowcs(wFilename, fileName, strlen(fileName) + 1);//Plus null
	ZeroMemory(&openFileName, sizeof(openFileName));
	openFileName.lStructSize = sizeof(OPENFILENAME);
	openFileName.lpstrFile = fileName;
	openFileName.lpstrFile[0] = '\0';
	openFileName.nMaxFile = MAX_PATH;
	openFileName.lpstrFilter = "All Files\0*.*\0C++ Files\0*.cpp\0Header Files\0*.h\0";
	openFileName.nFilterIndex = 1;
	openFileName.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
	
	std::filesystem::path _originPath{ std::filesystem::current_path() };

	if (GetSaveFileNameA(&openFileName))
	{
		std::filesystem::path _path{ fileName };

		std::filesystem::current_path(_originPath);
		
		//save file  (serialize here)
		EDITOR_LOG_INFO(_path.string().c_str());
		return  _path.string();
	}
	return std::string();
}
