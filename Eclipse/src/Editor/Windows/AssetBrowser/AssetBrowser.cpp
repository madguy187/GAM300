#include "pch.h"
#include "AssetBrowser.h"

namespace Eclipse
{
	AssetBrowserWindow::AssetBrowserWindow()
		:CurrentDir(AllDir),AllDir(AssetPath)
	{
		Type = EditorWindowType::ASSETBROWSER;
		WindowName = "AssetBrowser";
	}
	
	void AssetBrowserWindow::Update()
	{
		if (IsVisible)
			ECGui::DrawMainWindow<void()>(WindowName, std::bind(&AssetBrowserWindow::DrawImpl, this));
	}
	
	void AssetBrowserWindow::DrawImpl()
	{
		//temp render components test phase
		
		sprite.textureRef = Graphics::textures.find("PlayPauseStop");
		
		FolderIcon.textureRef = Graphics::textures.find("FolderIcon");
		
		ImGui::Columns(2);
		
		ImGui::SetColumnOffset(1, 240);
		
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
			//if (CurrentDir != std::filesystem::path(AssetPath))
			//{
			//	if (ImGui::Button("..."))
			//	{
			//		BackToParentPath(CurrentDir);
			//	}
			//}

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
						for (auto& secondEntry : std::filesystem::directory_iterator(NextDir))
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
		ECGui::DrawChildWindow<void()>({ "##top_bar", ImVec2(0, 20) }, std::bind(&AssetBrowserWindow::PathTracker, this));
		
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

		//	ImGui::Columns(s_ColumnCount, nullptr, false);
		
		float window_visible_x2 = ImGui::GetWindowPos().x + ECGui::GetWindowSize().x;

		static ImVec2 button_sz(120, 60);
		
		//ImGui::DragFloat2("size", (float*)&button_sz, 0.5f, 1.0f, 200.0f, "%.0f");
		
		ImGuiStyle& style = ImGui::GetStyle();
		
		ImDrawList* draw_list = ImGui::GetWindowDrawList();
		
		static int counter = 0;
		
		for (auto& dirEntry : std::filesystem::directory_iterator(CurrentDir))
		{
			const auto& path = dirEntry.path();
			
			auto relativePath = std::filesystem::relative(path, AssetPath);
			
			std::string fileNameString = relativePath.filename().string();

			if (dirEntry.is_directory())
			{
				ImGui::PushID(counter);
				
				ImGui::BeginGroup();
				
				if (ImGui::ImageButton((void*)FolderIcon.textureRef->second.GetHandle(),
					button_sz,
					ImVec2(folderoffsetX, folderoffsetY),
					ImVec2(1 / FolderIcon.textureRef->second.GetCols() + folderoffsetX, 1 / FolderIcon.textureRef->second.GetRows() + folderoffsetY)))
				{
					//do stuff with the files
				}
				
				ImGui::TextWrapped(fileNameString.c_str());

				ImGui::EndGroup();
				
				//draw_list->AddRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax(), IM_COL32(255, 255, 0, 255),0,0,3.f);
				if (ECGui::IsItemHovered())
				{
					ECGui::SetToolTip(fileNameString.c_str());
				}
			}
			else
			{
				ImGui::PushID(counter);
				
				ImGui::BeginGroup();
				
				if (ImGui::ImageButton((void*)sprite.textureRef->second.GetHandle(),
					button_sz,
					ImVec2(offsetX, offsetY),
					ImVec2(1 / sprite.textureRef->second.GetCols() + offsetX, 1 / sprite.textureRef->second.GetRows() + offsetY)))
				{
					//do stuff with the files
				}
				
				ImGui::TextWrapped(fileNameString.c_str());
				
				ImGui::EndGroup();
				
				//draw_list->AddRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax(), IM_COL32(255, 255, 0, 255),0,0,3.f);
				if (ECGui::IsItemHovered())
				{
					ECGui::SetToolTip(fileNameString.c_str());
				}
			}
			float last_button_x2 = ImGui::GetItemRectMax().x;
			
			float next_button_x2 = last_button_x2 + style.ItemSpacing.x + button_sz.x; // Expected position if next button was on same line

			if (counter + 1 < 5 && next_button_x2 < window_visible_x2)
			{
				ECGui::InsertSameLine();
				counter = 0;
			}
			else
			{
				ImGui::NextColumn();
			}
			
			ImGui::PopID();
			
			++counter;

			if (dirEntry.is_directory())
			{
				if (ImGui::IsMouseDoubleClicked(0) && ImGui::IsItemClicked(0))
				{
					NextPath(CurrentDir, path);
				}
			}
		}
		counter = 0;

		ImGui::PopStyleColor(2);

	}
	
	void AssetBrowserWindow::PathTracker()
	{
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

}
