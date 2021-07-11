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
		void Update() override;
		AssetBrowserWindow();
		void DrawImpl();
		void LeftFolderHierarchy();
		void RightFoldersAndItems();
		void FoldersAndItems();
		void PathAndSearches();
		void Path();
		void ItemsAndFolders();
		void BackToParentPath(std::filesystem::path &CurrentPath);
		void NextPath(std::filesystem::path& CurrentPath,std::filesystem::path paths);
		void MapNextPath(std::map<std::filesystem::path, std::vector<std::filesystem::path>> &pathMap, std::filesystem::path Key);

		// search functions
		void AddToPathMap(std::filesystem::path dirEntry, std::vector<std::filesystem::path> subDirItems);
		void AddToFolderMap(std::filesystem::path dirEntry, std::vector<std::filesystem::path> Folder);
		void ScanAll();
		void Search();
		void SearchInBaseFolder();
		void SearchInFolders();
		void MainSearchLogic(std::map<std::filesystem::path, std::vector<std::filesystem::path>>::value_type Key);
		std::string LowerCase(const char* buffer);
		bool BuffIsEmpty(const char* buffer);
	};
}
//for (auto const& pair : pathMap)
//{
//	for (auto const& pair2 : pair.second)
//	{
//		std::string nameString = LowerCase(pair2.filename().string().c_str());
//
//		const auto& path = pair2;
//
//		auto relativePath = std::filesystem::relative(path, AssetPath);
//
//		std::string fileNameString = relativePath.filename().string();
//
//		if (!std::filesystem::is_directory(pair2))
//		{
//			if (found && nameString.find(searchLowerCase) != std::string::npos)
//			{
//				RenderComponent icon = std::filesystem::is_directory(pair2) ? FolderIcon : sprite;
//
//				ImGui::ImageButton((void*)icon.textureRef->second.GetHandle(),
//					button_sz,
//					{ 1,0 },
//					{ 2,1 });
//
//				if (ImGui::IsMouseDoubleClicked(0) && ImGui::IsItemClicked(0) && ImGui::IsItemHovered())
//				{
//					//files do something ?
//				}
//
//				if (ECGui::IsItemHovered())
//				{
//					//ImGui::BeginChild("test", { 200,200 }, true);
//					//{
//					//	ImGui::Text("File Path: %s", relativePath.string().c_str());
//					//	
//					//	ImGui::Text("File Name: %s", fileNameString.c_str());
//					//}
//					//
//					//ImGui::EndChild();
//
//					//ECGui::SetToolTip(fileNameString.c_str());
//
//					ECGui::SetToolTip(relativePath.string().c_str());
//				}
//
//				ImDrawList* draw_list = ImGui::GetWindowDrawList();
//
//				ImGui::TextWrapped(fileNameString.c_str());
//
//				draw_list->AddRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax(), IM_COL32(255, 255, 0, 255));
//
//				ImGui::NextColumn();
//			}
//		}
//	}
//	ImGui::NextColumn();

//}