#pragma once
#include <filesystem>
#include "../Interface/ECGuiWindow.h"
namespace Eclipse
{
	class AssetBrowserWindow final : public ECGuiWindow
	{
		const std::filesystem::path AssetPath = "src//Assets";
		std::filesystem::path AllDir;
		std::filesystem::path CurrentDir;
		std::filesystem::path NextDir;
		RenderComponent sprite;
		RenderComponent FolderIcon;
		char searchBuffer[128];
		// key: main dir
		// value is a vector of sub dir / items
		std::map<std::filesystem::path, std::vector<std::filesystem::path>> OgpathMap;
		std::map<std::filesystem::path, std::vector<std::filesystem::path>> pathMap;
		// key: main dir
		// value is a vector of sub folders
		std::map<std::filesystem::path, std::vector<std::filesystem::path>> FolderMap;
		std::vector<std::filesystem::path> subDirItems;
		std::vector<std::filesystem::path> subDirFolders;
		std::string searchLowerCase;
		std::string folderString;
		bool found = false;
		bool searchMode = false;
		bool refresh = true;
	public:
		void Update() override;
		AssetBrowserWindow();
		void DrawImpl();
		void LeftFolderHierarchy();
		void RightFoldersAndItems();
		void FoldersAndItems();
		void PathTracker();
		void BackToParentPath(std::filesystem::path &CurrentPath);
		void NextPath(std::filesystem::path& CurrentPath,std::filesystem::path paths);
		void MapNextPath(std::map<std::filesystem::path, std::vector<std::filesystem::path>> &pathMap, std::filesystem::path Key);

		// search functions
		void AddToPathMap(std::filesystem::path dirEntry, std::vector<std::filesystem::path> subDirItems);
		void AddToFolderMap(std::filesystem::path dirEntry, std::vector<std::filesystem::path> Folder);
		void ScanAll();
		std::string LowerCase(const char* buffer);
		bool BuffIsEmpty(const char* buffer);
	};
}