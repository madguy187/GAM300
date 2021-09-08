#include "pch.h"
#include "AssetBrowser.h"
namespace Eclipse
{
	AssetBrowserWindow::AssetBrowserWindow()
		:CurrentDir(AllDir), AllDir(AssetPath), padding(16.0f), thumbnailSize(120.0f),sprite(),FolderIcon()
	{
		Type = EditorWindowType::EWT_ASSETBROWSER;

		WindowName = "AssetBrowser";
	
		memset(searchItemBuffer, 0, 128);

		memset(searchFolderBuffer, 0, 128);

		buttonSize = { thumbnailSize,thumbnailSize };
	}

	void AssetBrowserWindow::Update()
	{
		if (IsVisible)
			ECGui::DrawMainWindow<void()>(WindowName, std::bind(&AssetBrowserWindow::DrawImpl, this));
	}

	void AssetBrowserWindow::Unload()
	{
	}

	void AssetBrowserWindow::DrawImpl()
	{
		//temp render components test phase

		ImGui::Columns(2);

		ImGui::SetColumnOffset(1, 150);

		//left side
		ECGui::DrawChildWindow<void()>({ "##folders_common" }, std::bind(&AssetBrowserWindow::LeftFolderHierarchy, this));

		ImGui::NextColumn();

		sprite.textureRef = Graphics::textures.find("PlayPauseStop")->first;

		FolderIcon.textureRef = Graphics::textures.find("FolderIcon")->first;
		
		//right side
		ECGui::DrawChildWindow<void()>({ "##directory_structure", ImVec2(0, ImGui::GetWindowHeight() - 65) }, std::bind(&AssetBrowserWindow::RightFoldersAndItems, this));

		if (refresh == true)
		{
			ScanAll();
			EDITOR_LOG_INFO("Asset Browser Refreshed!");
			refresh = false;
		}
	}
	template <typename T>
	void AssetBrowserWindow::CreateTreeNode(std::string name, std::function<T> function)
	{
		if (ECGui::BeginTreeNode(name.c_str()))
		{
			function();
			ECGui::EndTreeNode();
		}
	}

	void AssetBrowserWindow::LeftFolderHierarchy()
	{

		SearchFolders();

		if (ECGui::CreateCollapsingHeader("Content"))
		{
			if (!searchFolderMode)
			{
				LeftFolders();

				ResetTreeNodeOpen = false;
			}
			else
			{
				ResetTreeNodeOpen = true;

				LeftSearchedFolders();
			}

		}

	}

	void AssetBrowserWindow::LeftFolders()
	{
		for (auto& dirEntry : std::filesystem::directory_iterator(AllDir))
		{
			if (ResetTreeNodeOpen)
			{
				ImGui::SetNextItemOpen(false);
			}

			const auto& path = dirEntry.path();

			std::string fileNameString = path.filename().string();

			if (dirEntry.is_directory())
			{

				if (ECGui::BeginTreeNode(fileNameString.c_str()))
				{
					//setting the subDir to Current Dir
					NextDir = AllDir;

					//setting its nxt path so that the tree node
					//shows all paths the current path hold afterwards

					NextPath(NextDir, path);

					//checking if the dir does not exist 
					if (!exists(NextDir))
					{
						NextDir = AllDir;
					}

					engine->editorManager->DragAndDropInst_.AssetBrowerFilesAndFoldersTarget("ITEM", paths, AssetPath.string(), dirEntry, refresh, pathMap, CopyFilesAndFolder);

					for (auto& secondEntry : std::filesystem::recursive_directory_iterator(NextDir))
					{
						if (secondEntry.is_directory())
						{
							const auto& path2 = secondEntry.path();

							std::string fileNameString2 = path2.filename().string();

							if (ECGui::BeginTreeNode(fileNameString2.c_str()))
							{
								if (/*ImGui::IsMouseDoubleClicked(0) &&*/ImGui::IsItemClicked(0))
								{
									NextDir = path2;

									CurrentDir = NextDir;

									if (!exists(CurrentDir))
									{
										CurrentDir = AllDir;
									}

									jumpDir = true;
								}

								ECGui::EndTreeNode();
							}
							engine->editorManager->DragAndDropInst_.AssetBrowerFilesAndFoldersTarget("ITEM", paths, AssetPath.string(), secondEntry, refresh, pathMap, CopyFilesAndFolder);
						}
					}
					ECGui::EndTreeNode();
				}

				engine->editorManager->DragAndDropInst_.AssetBrowerFilesAndFoldersTarget("ITEM", paths, AssetPath.string(), dirEntry, refresh, pathMap, CopyFilesAndFolder);

				if (!jumpDir && /*ImGui::IsMouseDoubleClicked(0) &&*/ ImGui::IsItemClicked(0))
				{
					NextPath(CurrentDir, path);
				}

				if (jumpDir)
				{
					jumpDir = false;
				}
			}

		}
	}

	void AssetBrowserWindow::LeftSearchedFolders()
	{
		for (auto& pair : FolderMap)
		{
			ImGui::SetNextItemOpen(true);

			if (ECGui::BeginTreeNode((pair.first.filename().string().c_str())))
			{
				for (auto& pair2 : pair.second)
				{
					std::string temp = LowerCase(pair2.filename().string().c_str()).c_str();
					if (!BuffIsEmpty(searchFolderBuffer) && temp.find(searchFolderBuffer) != std::string::npos)
					{
						//ImGui::SetNextItemOpen(true, ImGuiCond_Once);
						if (ImGui::TreeNode(temp.c_str()))
						{
							if (ImGui::IsMouseDoubleClicked(0) && ImGui::IsItemClicked(0))
							{
								NextDir = pair2;

								CurrentDir = NextDir;

								if (!exists(CurrentDir))
								{
									CurrentDir = AllDir;
								}
							}
							ECGui::EndTreeNode();
						}
					}
				}
				ECGui::EndTreeNode();
			}

		}
	}

	void AssetBrowserWindow::RightFoldersAndItems()
	{
		//top path 
		ECGui::DrawChildWindow<void()>({ "##top_bar", ImVec2(0, 70) }, std::bind(&AssetBrowserWindow::PathAndSearches, this));

		ImGui::Separator();

		//folders and items

		ECGui::DrawChildWindow<void()>({ "Scrolling" }, std::bind(&AssetBrowserWindow::FoldersAndItems, this));
	}

	void AssetBrowserWindow::FoldersAndItems()
	{
		//rightside with told path

		cellSize = thumbnailSize + padding;

		panelWidth = ImGui::GetContentRegionAvail().x;

		columnCount = (int)(panelWidth / cellSize);

		if (columnCount < 1)
		{
			columnCount = 1;
		}

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.3f, 0.3f, 0.3f, 0.35f));

		if (ImGui::BeginPopupContextWindow(0, 1, false))
		{
			//for pop ups
			ImGui::EndPopup();
		}

		ImGui::Columns(columnCount, 0, false);


		static ImVec2 button_sz(thumbnailSize, thumbnailSize);

		//ImGui::DragFloat2("size", (float*)&button_sz, 0.5f, 1.0f, 200.0f, "%.0f");

		ImGuiStyle& style = ImGui::GetStyle();

		if (!searchItemMode)
		{
			ItemsAndFolders();
		}
		else
		{
			if (CurrentDir == "src//Assets")
			{
				SearchInBaseFolder();
			}
			else
			{
				SearchInFolders();
			}
		}
		ImGui::Columns(1);

		ImGui::PopStyleColor(2);


	}

	void AssetBrowserWindow::PathAndSearches()
	{
		Path();

		SearchItems();
	}

	void AssetBrowserWindow::Path()
	{
		std::string source = { "src" };

		int id = 0, newPathId = -1;

		for (auto& it : this->CurrentDir)
		{
			ImGui::PushID(id);
			if (id > 0)
			{
				ECGui::InsertSameLine();

				ImGui::Text("/");

				ECGui::InsertSameLine();
			}
			if (it.string() != source)
			{
				if (ImGui::SmallButton((it.u8string()).c_str()))
				{
					newPathId = id;
				}
			}

			ImGui::PopID();

			++id;
		}

		if (newPathId >= 0)
		{
			int i = 0;

			std::filesystem::path newPath;

			for (auto& sec : this->CurrentDir)
			{
				if (i++ > newPathId)
				{
					break;
				}

				newPath /= sec;
			}

			this->CurrentDir = newPath;
		}
	}

	void AssetBrowserWindow::ItemsAndFolders()
	{
		for (auto& dirEntry : std::filesystem::directory_iterator(CurrentDir))
		{

			const auto& path = dirEntry.path();

			auto relativePath = std::filesystem::relative(path, AssetPath);

			std::string fileNameString = relativePath.filename().string();

			ImGui::PushID(fileNameString.c_str());

			RenderComponent icon = dirEntry.is_directory() ? FolderIcon : sprite;

			ImGui::ImageButton((void*)Graphics::textures[icon.textureRef].GetHandle(),
				buttonSize,
				{ 1,0 },
				{ 2,1 });
			//drag drop
			engine->editorManager->DragAndDropInst_.StringPayloadSource("ITEM", relativePath.string());

			engine->editorManager->DragAndDropInst_.AssetBrowerFilesAndFoldersTarget("ITEM", paths, AssetPath.string(), dirEntry, refresh, pathMap, CopyFilesAndFolder);

			if (ImGui::IsMouseDoubleClicked(0) && ImGui::IsItemClicked(0) && ImGui::IsItemHovered())
			{
				if (dirEntry.is_directory())
				{
					NextPath(CurrentDir, path);
				}
				else
				{
					//do stuff
				}
			}

			if (ECGui::IsItemHovered())
			{
				ECGui::SetToolTip(relativePath.string().c_str());
			}

			if (found && searchItemsLowerCase == LowerCase(relativePath.filename().string().c_str()))
			{
				ImDrawList* draw_list = ImGui::GetWindowDrawList();

				ImGui::TextWrapped(fileNameString.c_str());

				draw_list->AddRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax(), IM_COL32(255, 255, 0, 255));
			}
			else
			{
				ImGui::TextWrapped(fileNameString.c_str());
			}

			ImGui::NextColumn();
			ImGui::PopID();
		}
	}

	void AssetBrowserWindow::BackToParentPath(std::filesystem::path& CurrentPath)
	{
		CurrentPath = CurrentPath.parent_path();
	}

	void AssetBrowserWindow::NextPath(std::filesystem::path& CurrentPath, std::filesystem::path paths)
	{
		CurrentPath /= paths.filename();

		if (!exists(CurrentPath))
		{
			CurrentPath = paths;
		}
	}

	void AssetBrowserWindow::AddToPathMap(std::filesystem::path dirEntry, std::vector<std::filesystem::path> subDirItems)
	{
		if (pathMap.find(dirEntry) != pathMap.end())
		{
			pathMap.insert_or_assign(dirEntry, subDirItems);
		}
		else
		{
			pathMap.emplace(dirEntry, subDirItems);
		}

	}

	void AssetBrowserWindow::AddToFolderMap(std::filesystem::path dirEntry, std::vector<std::filesystem::path> Folder)
	{
		if (FolderMap.find(dirEntry) != FolderMap.end())
		{
			FolderMap.insert_or_assign(dirEntry, Folder);
		}
		else
		{
			FolderMap.emplace(dirEntry, Folder);
		}

	}

	bool AssetBrowserWindow::ExistInVector(std::string Path, std::vector<std::string> container)
	{
		if (std::find(container.begin(), container.end(), Path) != container.end())
		{
			return true;
		}
		return false;
	}

	void AssetBrowserWindow::GetIndex(std::vector<std::string> container, std::string key, int& pos)
	{
		auto it = std::find(container.begin(), container.end(), key);

		// If element was found
		if (it != container.end())
		{
			pos = it - container.begin();
		}
	}

	void AssetBrowserWindow::ScanAll()
	{
		std::vector<std::filesystem::path> tempSubDirFolders;

		std::vector<std::filesystem::path> tempSubDirItems;

		for (auto& dirEntry : std::filesystem::directory_iterator(AllDir))
		{
			const auto& path = dirEntry.path();

			auto relativePath = relative(path, AssetPath);

			std::string fileNameString = relativePath.filename().string();

			if (dirEntry.is_directory())
			{
				NextDir = AllDir;

				NextPath(NextDir, path);

				if (!exists(NextDir))
				{
					NextDir = AllDir;
				}

				for (auto& secondEntry : std::filesystem::recursive_directory_iterator(NextDir))
				{
					if (secondEntry.is_directory())
					{
						tempSubDirFolders.push_back(secondEntry);
					}

					if (!ExistInVector(secondEntry.path().string(), subDirItemsPath))
					{
						subDirItemsPath.push_back(secondEntry.path().string());
					}

					tempSubDirItems.push_back(secondEntry);
				}

			}


			AddToFolderMap(dirEntry, tempSubDirFolders);

			AddToPathMap(dirEntry, tempSubDirItems);

			tempSubDirFolders.clear();

			tempSubDirItems.clear();
		}

	}

	void AssetBrowserWindow::SearchItems()
	{
		if (ImGui::InputTextWithHint("", "Search...", searchItemBuffer, 128))
		{
			if (BuffIsEmpty(searchItemBuffer))
			{
				searchItemMode = false;
			}
			else
			{
				searchItemMode = true;
			}

			//when theres nothing in the buffer

			// this is the string i am searching in the directories
			searchItemsLowerCase = LowerCase(searchItemBuffer);

		}
		if (ImGui::Button("Refresh", { 70,20 }))
		{
			refresh = true;
		}
		ImGui::SameLine();
		if (ImGui::Button("Clear", { 70,20 }))
		{
			if (BuffIsEmpty(searchItemBuffer))
			{
				searchItemMode = false;
				EDITOR_LOG_WARN("Cleared Failed: Buffer Empty");
			}
			else
			{
				memset(searchItemBuffer, 0, 128);
				searchItemMode = false;
				EDITOR_LOG_INFO("Item Search Buffer Cleared");
			}
		}
		ImGui::SameLine();

		ImGui::Checkbox("Copy Mode", &CopyFilesAndFolder);

		ImGui::SameLine();

		ImGuiAPI::HelpMarker("When ticked |Copy of files and folder| Else |Moving of files and folder | to destination  ");

		//left side search for all files & folders in that current dir
		//right side search for all folders
	}

	void AssetBrowserWindow::SearchFolders()
	{
		if (ImGui::InputTextWithHint("", "Search...", searchFolderBuffer, 128))
		{
			if (BuffIsEmpty(searchFolderBuffer))
			{
				searchFolderMode = false;
			}
			else
			{
				searchFolderMode = true;
			}

			//when theres nothing in the buffer

			// this is the string i am searching in the directories
			searchFoldersLowerCase = LowerCase(searchFolderBuffer);

		}
	}

	void AssetBrowserWindow::SearchInBaseFolder()
	{
		MainSearchLogic(subDirItemsPath);
	}

	void AssetBrowserWindow::SearchInFolders()
	{
		for (auto const& pair : pathMap)
		{
			if (pair.first == CurrentDir)
			{
				MainSearchLogic(pair);
			}
		}
	}

	void AssetBrowserWindow::MainSearchLogic(std::map<std::filesystem::path, std::vector<std::filesystem::path>>::value_type Key)
	{
		for (auto const& pair2 : Key.second)
		{
			folderString = LowerCase(pair2.filename().string().c_str());

			if (searchItemsLowerCase == folderString)
			{
				//found
				found = true;
				//CurrentDir = pair.first;

				if (BuffIsEmpty(searchItemBuffer))
				{
					found = false;
				}
			}
			else
			{
				if (folderString.find(searchItemsLowerCase) != std::string::npos)
				{
					//CurrentDir = pair.first;
					found = true;
				}
			}

			std::string nameString = LowerCase(pair2.filename().string().c_str());

			const auto& path = pair2;

			auto relativePath = std::filesystem::relative(path, AssetPath);

			std::string fileNameString = relativePath.filename().string();

			if (!std::filesystem::is_directory(pair2))
			{
				if (found && nameString.find(searchItemsLowerCase) != std::string::npos)
				{
					RenderComponent icon = std::filesystem::is_directory(pair2) ? FolderIcon : sprite;

					ImGui::ImageButton((void*)Graphics::textures[icon.textureRef].GetHandle(),
						buttonSize,
						{ 1,0 },
						{ 2,1 });

					if (ImGui::IsMouseDoubleClicked(0) && ImGui::IsItemClicked(0) && ImGui::IsItemHovered())
					{
						//files do something ?
					}

					if (ECGui::IsItemHovered())
					{
						ECGui::SetToolTip(relativePath.string().c_str());
					}

					ImDrawList* draw_list = ImGui::GetWindowDrawList();

					ImGui::TextWrapped(fileNameString.c_str());

					draw_list->AddRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax(), IM_COL32(255, 255, 0, 255));

					ImGui::NextColumn();
				}
			}
		}
	}

	void AssetBrowserWindow::MainSearchLogic(std::vector<std::string> subDirItemsPath)
	{

		//path that can be used in the future to get the item
		std::filesystem::path tempPath;

		for (auto const& pair2 : subDirItemsPath)
		{
			tempPath = pair2;

			if (!std::filesystem::is_directory(tempPath))
			{

				if (!BuffIsEmpty(searchItemBuffer) && tempPath.string().find(searchItemsLowerCase) != std::string::npos)
				{
					RenderComponent icon = std::filesystem::is_directory(tempPath) ? FolderIcon : sprite;

					ImGui::ImageButton((void*)Graphics::textures[icon.textureRef].GetHandle(),
						buttonSize,
						{ 1,0 },
						{ 2,1 });

					if (ImGui::IsMouseDoubleClicked(0) && ImGui::IsItemClicked(0) && ImGui::IsItemHovered())
					{
						//files do something ?
					}

					if (ECGui::IsItemHovered())
					{
						ECGui::SetToolTip(pair2.c_str());
					}

					ImDrawList* draw_list = ImGui::GetWindowDrawList();

					ImGui::TextWrapped(tempPath.filename().string().c_str());

					draw_list->AddRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax(), IM_COL32(255, 255, 0, 255));

					ImGui::NextColumn();
				}
			}

		}
	}

	std::string AssetBrowserWindow::LowerCase(const char* buffer)
	{
		std::string lowerCaseString;

		std::string bufferString(buffer);

		for (const auto& character : bufferString)
		{
			lowerCaseString += std::tolower(character);
		}

		size_t lastdot = lowerCaseString.find_last_of(".");

		if (lastdot == std::string::npos)
		{
			return lowerCaseString;
		}

		return lowerCaseString.substr(0, lastdot);
	}

	bool AssetBrowserWindow::BuffIsEmpty(const char* buffer)
	{
		if (strlen(buffer) == 0)
		{
			return true;
		}

		return false;
	}

	std::map<std::filesystem::path, std::vector<std::filesystem::path>> AssetBrowserWindow::getFolderMap()
	{
		return FolderMap;
	}

	std::map<std::filesystem::path, std::vector<std::filesystem::path>> AssetBrowserWindow::getPathMap()
	{
		return pathMap;
	}
}
