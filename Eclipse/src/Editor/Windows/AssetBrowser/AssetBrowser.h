#pragma once
#include <filesystem>

#include "imgui.h"

namespace Eclipse
{
	class AssetBrowserWindow final : public ECGuiWindow
	{
		//Path and Dir
		////////////////////////////////
		const std::filesystem::path AssetPath = "src\\Assets";
		std::filesystem::path AllDir;
		std::filesystem::path CurrentDir;
		std::filesystem::path NextDir;
		////////////////////////////////

		
		//button settings
		////////////////////////////////
		char searchItemBuffer[128] = "";
		char searchFolderBuffer[128] = "";
		ImVec2 buttonSize;
		float padding = 0.0f;
		float thumbnailSize = 0.0f;
		float cellSize = 0.0f;
		float panelWidth = 0.0f;
		int columnCount = 0;
		const char* paths = nullptr;
		////////////////////////////////

		//Searching
		////////////////////////////////
		// key: main dir
		// value is a vector of sub dir / items
		//std::map<std::filesystem::path, std::vector<std::filesystem::path>> OgpathMap;(testing)
		std::map<std::filesystem::path, std::vector<std::filesystem::path>> pathMap;
		std::map<std::filesystem::path, std::vector<std::filesystem::path>> FolderMap;
		//stores all data for path map
		std::vector<std::string> subDirItemsPath;
		std::string searchItemsLowerCase;
		std::string searchFoldersLowerCase;
		std::string folderString;
		std::vector<std::string> allExtensions;
		bool found = false;
		bool searchItemMode = false;
		bool searchFolderMode = false;
		bool refresh = true;
		bool jumpDir = false;
		bool ResetTreeNodeOpen = false;
		bool CopyFilesAndFolder = false;
		////////////////////////////////
	public:
		//main update function
		////////////////////////////////
		void Update() override;
		void Init() override;
		void Unload() override;
		AssetBrowserWindow();
		void DrawImpl();
		////////////////////////////////

		//asset browser main
		////////////////////////////////
		template <typename T>
		void CreateTreeNode(std::string name,std::function<T> function);
		void LeftFolderHierarchy();
		void LeftFolders();
		void LeftSearchedFolders();
		void RightFoldersAndItems();
		void FoldersAndItems();
		void PathAndSearches();
		void Path();
		void ItemsAndFolders();
		void BackToParentPath(std::filesystem::path &CurrentPath);
		void NextPath(std::filesystem::path& CurrentPath,std::filesystem::path pathsofItem);
		////////////////////////////////

		
		// search functions 
		////////////////////////////////
		void AddToPathMap(std::filesystem::path dirEntry, std::vector<std::filesystem::path> subDirItems);
		void AddToFolderMap(std::filesystem::path dirEntry, std::vector<std::filesystem::path> Folder);
		bool ExistInVector(std::string Path, std::vector<std::string> container);
		void GetIndex(std::vector<std::string> container, std::string key, int& pos);
		void ScanAll();
		void SearchItems();
		void SearchFolders();
		void SearchInBaseFolder();
		void SearchInFolders();
		void MainSearchLogic(std::map<std::filesystem::path, std::vector<std::filesystem::path>>::value_type Key);
		void MainSearchLogic(std::vector<std::string> subDirItemsPaths);
		bool BuffIsEmpty(const char* buffer);
		void findRootPath(std::string inputPath, std::string& outputPath);
		//TODO  for Darren material editor 
		//bool test2 = false;
		//std::string tempcontainer;
		//void temptest(std::string pathName);
		std::map<std::filesystem::path, std::vector<std::filesystem::path>> getFolderMap();
		std::map<std::filesystem::path, std::vector<std::filesystem::path>> getPathMap();
		////////////////////////////////

		void DragAndDropWrapper(std::string filename,std::string relativePath_, std::filesystem::path dirEntry);
		// converting to small case letters
		////////////////////////////////
		std::string LowerCase(const char* buffer);

		static std::string GetFileName(const char* buffer);
		////////////////////////////////
	};
}
