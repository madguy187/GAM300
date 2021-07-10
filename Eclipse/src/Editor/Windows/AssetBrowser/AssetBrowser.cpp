#include "pch.h"
#include "AssetBrowser.h"

namespace Eclipse
{
	AssetBrowserWindow::AssetBrowserWindow()
		:CurrentDir(AllDir),AllDir(AssetPath)
	{
		Type = EditorWindowType::ASSETBROWSER;
		WindowName = "AssetBrowser";
		
		sprite.textureRef = Graphics::textures.find("PlayPauseStop");

		FolderIcon.textureRef = Graphics::textures.find("FolderIcon");

		memset(searchBuffer, 0, 128);
	}
	
	void AssetBrowserWindow::Update()
	{
		if (IsVisible)
			ECGui::DrawMainWindow<void()>(WindowName, std::bind(&AssetBrowserWindow::DrawImpl, this));
	}
	
	void AssetBrowserWindow::DrawImpl()
	{
		//temp render components test phase
		
		ImGui::Columns(2);
		
		ImGui::SetColumnOffset(1, 150);
		
		if(refresh == true)
		{
			ScanAll();

			refresh = false;
		}
		//left side
		ECGui::DrawChildWindow<void()>({ "##folders_common" },std::bind(&AssetBrowserWindow::LeftFolderHierarchy,this));
		
		ImGui::NextColumn();

		//right side
		ECGui::DrawChildWindow<void()>({ "##directory_structure", ImVec2(0, ImGui::GetWindowHeight() - 65) }, std::bind(&AssetBrowserWindow::RightFoldersAndItems, this));
	}

	void AssetBrowserWindow::LeftFolderHierarchy()
	{
		static bool jumpDir = false;
		
		if (ECGui::CreateCollapsingHeader("Content"))
		{

			for (auto& dirEntry : std::filesystem::directory_iterator(AllDir))
			{
				const auto& path = dirEntry.path();
				
				auto relativePath = relative(path, AssetPath);
				
				std::string fileNameString = relativePath.filename().string();
				
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
						for (auto& secondEntry : std::filesystem::recursive_directory_iterator(NextDir))
						{
							if (secondEntry.is_directory())
							{
								const auto& path2 = secondEntry.path();
								auto relativePath2 = relative(path2, AssetPath);
								std::string fileNameString2 = relativePath2.filename().string();
								if (ECGui::BeginTreeNode(fileNameString2.c_str()))
								{
									if (ImGui::IsMouseDoubleClicked(0) && ImGui::IsItemClicked(0))
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

							}
						}
						ECGui::EndTreeNode();
					}
					
					if (!jumpDir && ImGui::IsMouseDoubleClicked(0) && ImGui::IsItemClicked(0))
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

	}

	void AssetBrowserWindow::RightFoldersAndItems()
	{
		//top path 
		ECGui::DrawChildWindow<void()>({ "##top_bar", ImVec2(0, 50) }, std::bind(&AssetBrowserWindow::PathTracker, this));
		
		ImGui::Separator();

		//folders and items
		
		ECGui::DrawChildWindow<void()>({ "Scrolling"}, std::bind(&AssetBrowserWindow::FoldersAndItems, this));
	}

	void AssetBrowserWindow::FoldersAndItems()
	{
		//rightside with told path
		float offsetX = static_cast<float>((1.0 / sprite.textureRef->second.GetCols()));
		float offsetY = static_cast<float>((1.0 / sprite.textureRef->second.GetRows()));
		float folderoffsetX = static_cast<float>((1.0 / FolderIcon.textureRef->second.GetCols()));
		float folderoffsetY = static_cast<float>((1.0 / FolderIcon.textureRef->second.GetRows()));
		
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.3f, 0.3f, 0.3f, 0.35f));

		if (ImGui::BeginPopupContextWindow(0, 1, false))
		{
			//for pop ups
			ImGui::EndPopup();
		}
		
		ImGui::Columns(5,0,false);


		static ImVec2 button_sz(120, 60);
	
		//ImGui::DragFloat2("size", (float*)&button_sz, 0.5f, 1.0f, 200.0f, "%.0f");
		
		ImGuiStyle& style = ImGui::GetStyle();
		
		static int counter = 0;

		if(!searchMode)
		{
			for (auto& dirEntry : std::filesystem::directory_iterator(CurrentDir))
			{

				const auto& path = dirEntry.path();

				auto relativePath = std::filesystem::relative(path, AssetPath);

				std::string fileNameString = relativePath.filename().string();

				RenderComponent icon = dirEntry.is_directory() ? FolderIcon : sprite;
				ImGui::ImageButton((void*)icon.textureRef->second.GetHandle(),
					button_sz,
					ImVec2(folderoffsetX, folderoffsetY),
					ImVec2(1 / FolderIcon.textureRef->second.GetCols() + folderoffsetX, 1 / FolderIcon.textureRef->second.GetRows() + folderoffsetY));
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
					ECGui::SetToolTip(fileNameString.c_str());
				}
				if (found && searchLowerCase == LowerCase(relativePath.filename().string().c_str()))
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
			}
		}
		else
		{
			// for searching only
			
			for (auto const& pair : pathMap)
			{
				for (auto const& pair2 : pair.second)
				{
					std::string nameString = LowerCase(pair2.filename().string().c_str());
					
					const auto& path = pair2;
					
					auto relativePath = std::filesystem::relative(path, AssetPath);
					
					std::string fileNameString = relativePath.filename().string();
					
					if (found && nameString.find(searchLowerCase) != std::string::npos)
					{
						RenderComponent icon = std::filesystem::is_directory(pair2) ? FolderIcon : sprite;
						ImGui::ImageButton((void*)icon.textureRef->second.GetHandle(),
							button_sz,
							ImVec2(folderoffsetX, folderoffsetY),
							ImVec2(1 / FolderIcon.textureRef->second.GetCols() + folderoffsetX, 1 / FolderIcon.textureRef->second.GetRows() + folderoffsetY));
						if (ImGui::IsMouseDoubleClicked(0) && ImGui::IsItemClicked(0) && ImGui::IsItemHovered())
						{
							if (std::filesystem::is_directory(pair2))
							{
								std::cout << " am in";
								searchMode = false;
								CurrentDir = pair2;
							}
							else
							{
								//files do something ?
							}
						}
						if (ECGui::IsItemHovered())
						{
							ECGui::SetToolTip(fileNameString.c_str());
						}
						ImDrawList* draw_list = ImGui::GetWindowDrawList();
						ImGui::TextWrapped(fileNameString.c_str());
						draw_list->AddRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax(), IM_COL32(255, 255, 0, 255));
					}
				}
				ImGui::NextColumn();
			}
		}
		ImGui::Columns(1);
		ImGui::PopStyleColor(2);
		

	}
	
	void AssetBrowserWindow::PathTracker()
	{
		//ImGui::SetColumnOffset(2, 300);
		std::string source = {"src"};
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
			if(it.string() != source)
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

		std::cout << searchMode;
		if (ImGui::InputTextWithHint("", "Search...", searchBuffer, 128))
		{
			if(BuffIsEmpty(searchBuffer))
			{
				searchMode = false;
			}
			else
			{
				searchMode = true;
			}
			
			//when theres nothing in the buffer

			// this is the string i am searching in the directories
			searchLowerCase = LowerCase(searchBuffer);
			
			if(CurrentDir == "src//Assets")
			{
				for (auto const& pair : pathMap)
				{
					for (auto const& pair2 : pair.second)
					{
						folderString = LowerCase(pair2.filename().string().c_str());

						if (searchLowerCase == folderString)
						{
							//found
							found = true;
							//CurrentDir = pair.first;

							if (BuffIsEmpty(searchBuffer))
							{
								found = false;
							}
						}
						else
						{
							if (folderString.find(searchLowerCase) != std::string::npos)
							{
								//CurrentDir = pair.first;
								found = true;
							}
						}
					}
				}
			}
			else
			{
				for (auto const& pair : pathMap)
				{
					if (pair.first == CurrentDir)
					{
						for (auto const& pair2 : pair.second)
						{
							folderString = LowerCase(pair2.filename().string().c_str());

							if (searchLowerCase == folderString)
							{
								//found
								found = true;
								//CurrentDir = pair.first;

								if (BuffIsEmpty(searchBuffer))
								{
									found = false;
								}
							}
							else
							{
								if(folderString.find(searchLowerCase) != std::string::npos)
								{
									//found = true;
									CurrentDir = pair.first;
								}
							}
						}
					}
				}
			}
		}

		ImGui::SameLine();
		if(ImGui::Button("Refresh",{40,40}) && ImGui::IsItemClicked())
		{
			refresh = true;
		}
		else
		{
			refresh = false;
		}
		
		//left side search for all files & folders in that current dir
		//right side search for all folders
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

	void AssetBrowserWindow::MapNextPath(std::map<std::filesystem::path, std::vector<std::filesystem::path>>& pathMap,
		std::filesystem::path Key)
	{
		
	}

	void AssetBrowserWindow::AddToPathMap(std::filesystem::path dirEntry, std::vector<std::filesystem::path> subDirItems)
	{
		if(pathMap.find(dirEntry) != pathMap.end())
		{
			pathMap.insert_or_assign(dirEntry, subDirItems);
		}
		else
		{
			pathMap.emplace(dirEntry, subDirItems);
		}

		//for (auto const& pair : pathMap) {
		//	std::cout << "{" << pair.first.filename();
		//	for(auto const&pair2 : pair.second)
		//	{
		//		std::cout << pair2.filename() << std::endl;
		//	}
		//	std::cout << "}" << std::endl;
		//}
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

		for (auto const& pair : FolderMap) 
		{
			std::cout << "{" << pair.first.filename();
			for (auto const& pair2 : pair.second)
			{
				std::cout << pair2.filename() << std::endl;
			}
			std::cout << "}" << std::endl;
		}
	}

	void AssetBrowserWindow::ScanAll()
	{
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
					if(secondEntry.is_directory())
					{
						subDirFolders.push_back(secondEntry);
					}
					subDirItems.push_back(secondEntry);
				}
				
			}
			AddToFolderMap(dirEntry, subDirFolders);
			AddToPathMap(dirEntry, subDirItems);
			subDirFolders.clear();
			subDirItems.clear();
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
		if (strlen(searchBuffer) == 0)
		{
			return true;
		}
		
		return false;
	}


}
