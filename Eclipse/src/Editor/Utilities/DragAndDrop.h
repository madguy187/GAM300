#pragma once
#include <filesystem>
namespace Eclipse
{
	class DragAndDrop
	{
	public:

		void AssetBrowerFilesAndFoldersSource(const char* type,std::string relativePath );
		void AssetBrowerFilesAndFoldersTarget(const char* type, const wchar_t* paths, std::string AssetPath, std::filesystem::directory_entry dirEntry, bool refreshBrowser);
	};
}
