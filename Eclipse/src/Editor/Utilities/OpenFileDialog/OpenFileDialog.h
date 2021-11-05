#pragma once
#include <Windows.h>
class FileDialog
{
public:
	static std::string FileBrowser();
	static std::string SaveAsFile();
	static std::string SaveFile();
};