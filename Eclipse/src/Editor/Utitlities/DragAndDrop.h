#pragma once
#include <filesystem>
namespace Eclipse
{
	class DragAndDrop
	{
	public:

		void AssetBrowerFilesAndFoldersSource(const char* type, std::filesystem::path relativePath );
		void AssetBrowerFilesAndFoldersTarget(const char* type, const wchar_t* paths, std::filesystem::path AssetPath, std::filesystem::directory_entry dirEntry, bool refreshBrowerser);
	};
}
