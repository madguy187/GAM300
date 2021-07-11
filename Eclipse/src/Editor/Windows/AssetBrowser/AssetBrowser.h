#pragma once
#include <filesystem>
#include "../Interface/ECGuiWindow.h"
namespace Eclipse
{
	class AssetBrowserWindow final : public ECGuiWindow
	{
		//Path and Dir
		////////////////////////////////
		const std::filesystem::path AssetPath = "src//Assets";
		std::filesystem::path AllDir;
		std::filesystem::path CurrentDir;
		std::filesystem::path NextDir;
		////////////////////////////////
		

		//Buton Icon rendering
		////////////////////////////////
		RenderComponent sprite;
		RenderComponent FolderIcon;
		////////////////////////////////

		
		//button settings
		////////////////////////////////
		char searchBuffer[128];
		ImVec2 buttonSize;
		float padding;
		float thumbnailSize;
		float cellSize;
		float panelWidth;
		int columnCount;
		////////////////////////////////

		//Searching
		////////////////////////////////
		// key: main dir
		// value is a vector of sub dir / items
		//std::map<std::filesystem::path, std::vector<std::filesystem::path>> OgpathMap;(testing)
		std::map<std::filesystem::path, std::vector<std::filesystem::path>> pathMap;
		std::map<std::filesystem::path, std::vector<std::filesystem::path>> FolderMap;
		std::vector<std::filesystem::path> subDirItems;
		std::vector<std::filesystem::path> subDirFolders;
		std::string searchLowerCase;
		std::string folderString;
		bool found = false;
		bool searchMode = false;
		bool refresh = true;
		////////////////////////////////
		
	public:
		
		//main update function
		////////////////////////////////
		void Update() override;
		AssetBrowserWindow();
		void DrawImpl();
		////////////////////////////////

		//asset browser main
		////////////////////////////////
		void LeftFolderHierarchy();
		void RightFoldersAndItems();
		void FoldersAndItems();
		void PathAndSearches();
		void Path();
		void ItemsAndFolders();
		void BackToParentPath(std::filesystem::path &CurrentPath);
		void NextPath(std::filesystem::path& CurrentPath,std::filesystem::path paths);
		void MapNextPath(std::map<std::filesystem::path, std::vector<std::filesystem::path>> &pathMap, std::filesystem::path Key);
		////////////////////////////////

		
		// search functions 
		////////////////////////////////
		void AddToPathMap(std::filesystem::path dirEntry, std::vector<std::filesystem::path> subDirItems);
		void AddToFolderMap(std::filesystem::path dirEntry, std::vector<std::filesystem::path> Folder);
		void ScanAll();
		void Search();
		void SearchInBaseFolder();
		void SearchInFolders();
		void MainSearchLogic(std::map<std::filesystem::path, std::vector<std::filesystem::path>>::value_type Key);
		bool BuffIsEmpty(const char* buffer);
		////////////////////////////////

		
		// converting to small case letters
		////////////////////////////////
		std::string LowerCase(const char* buffer);
		////////////////////////////////
	};
}
