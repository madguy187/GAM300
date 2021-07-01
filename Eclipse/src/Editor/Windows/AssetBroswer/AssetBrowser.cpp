#include "pch.h"

Eclipse::AssetBrowser::AssetBrowser()
	:CurrentDir(AssetPath), NextDir()
{
	Type = EditorWindowType::ASSETBROWSER;
	WindowName = "AssetBrowser";
}

void Eclipse::AssetBrowser::Update()
{
	if (IsVisible)
		ECGui::DrawMainWindow<void()>(WindowName, std::bind(&AssetBrowser::DrawImpl, this));
}

void Eclipse::AssetBrowser::DrawImpl()
{
	RenderComponent sprite;
	sprite.textureRef = Graphics::textures.find("PlayPauseStop");
	ImGui::Columns(2);
	ImGui::SetColumnOffset(1, 240);
	//left side
	ImGui::BeginChild("##folders_common");
	{
		if (ImGui::CollapsingHeader("Content", nullptr, ImGuiTreeNodeFlags_DefaultOpen))
		{
			if (CurrentDir != std::filesystem::path(AssetPath))
			{
				if (ImGui::Button("..."))
				{
					BackToParentPath(CurrentDir);
				}
			}
			for (auto& dirEntry : std::filesystem::directory_iterator(CurrentDir))
			{
				const auto& path = dirEntry.path();
				auto relativePath = relative(path, AssetPath);
				std::string fileNameString = relativePath.filename().string();
				static bool jumpDir = false;
				if (dirEntry.is_directory())
				{
					if (ImGui::TreeNode(fileNameString.c_str()))
					{
						//setting the subDir to Current Dir
						NextDir = CurrentDir;
						
						//setting its nxt path so that the tree node
						//shows all paths the current path hold afterwards
						
						NextPath(NextDir, path);
						//checking if the dir does not exist 
						if(!exists(NextDir))
						{
							NextDir = CurrentDir;
						}
						for (auto& secondEntry : std::filesystem::directory_iterator(NextDir))
						{
							if(secondEntry.is_directory())
							{
								const auto& path2 = secondEntry.path();
								auto relativePath2 = relative(path2, AssetPath);
								std::string fileNameString2 = relativePath2.filename().string();
								if (ImGui::TreeNode(fileNameString2.c_str()))
								{
									if (ImGui::IsMouseDoubleClicked(0) && ImGui::IsItemClicked(0))
									{
										NextDir = path2;
										CurrentDir = NextDir;
										jumpDir = true;
									}
									ImGui::TreePop();
								}
								
							}
						}
						
						ImGui::TreePop();
					}
					if (!jumpDir && ImGui::IsMouseDoubleClicked(0) && ImGui::IsItemClicked(0))
					{
						NextPath(CurrentDir, path);
					}
					if(jumpDir)
					{
						jumpDir = false;
					}
				}
				
			}
		}
		
	}
	ImGui::EndChild();
	ImGui::NextColumn();
	//rightside with told path
	float offsetX = static_cast<float>((1.0 / sprite.textureRef->second.GetCols()));
	float offsetY = static_cast<float>((1.0 / sprite.textureRef->second.GetRows()));
	ImGui::BeginChild("##directory_structure", ImVec2(0, ImGui::GetWindowHeight() - 65));
	{
		ImGui::BeginChild("##top_bar", ImVec2(0, 20));
		{
			PathTracker(CurrentDir);
		}
		ImGui::EndChild();
		ImGui::Separator();
		ImGui::BeginChild("Scrolling");
		{
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.3f, 0.3f, 0.3f, 0.35f));
			
			if (ImGui::BeginPopupContextWindow(0, 1, false))
			{
				//for pop ups
				ImGui::EndPopup();
			}
			
		//	ImGui::Columns(s_ColumnCount, nullptr, false);
			float window_visible_x2 = ImGui::GetWindowPos().x + ImGui::GetWindowContentRegionMax().x;
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
				if (!dirEntry.is_directory())
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
					if (ImGui::IsItemHovered())
					{
						ImGui::SetTooltip("%s", fileNameString.c_str());
					}
					float last_button_x2 = ImGui::GetItemRectMax().x;
					float next_button_x2 = last_button_x2 + style.ItemSpacing.x + button_sz.x; // Expected position if next button was on same line
					if (counter + 1 < 5 && next_button_x2 < window_visible_x2)
					{
						ImGui::SameLine();
						//ImGui::NextColumn();
						counter = 0;
					}
					else
					{
						ImGui::NextColumn();
					}
					//ImGui::EndGroup();
					ImGui::PopID();
					++counter;
				}
			}
			counter = 0;
			
			ImGui::PopStyleColor(2);
			
		}
		ImGui::EndChild();
	}
	ImGui::EndChild();
}

void Eclipse::AssetBrowser::PathTracker(std::filesystem::path& CurrentPath)
{
	std::string source = {"src"};
	int id = 0, newPathId = -1;
	for (auto& it : CurrentPath)
	{
		ImGui::PushID(id);
		if (id > 0)
		{
			ImGui::SameLine();
			ImGui::Text("/");
			ImGui::SameLine();
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
		for (auto& sec : CurrentPath)
		{
			if (i++ > newPathId)
			{
				break;
			}
			newPath /= sec;
		}
		CurrentPath = newPath;
	}
}

void Eclipse::AssetBrowser::BackToParentPath(std::filesystem::path& CurrentPath)
{
	CurrentPath = CurrentPath.parent_path();
}

void Eclipse::AssetBrowser::NextPath(std::filesystem::path& CurrentPath, std::filesystem::path paths)
{
	CurrentPath /= paths.filename();
}
