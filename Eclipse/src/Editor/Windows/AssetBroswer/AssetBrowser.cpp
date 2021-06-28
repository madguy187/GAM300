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
	ImGui::Columns(1);
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
	ImGui::BeginChild("##directory_structure", ImVec2(0, ImGui::GetWindowHeight() - 65));
	{
		ImGui::BeginChild("##top_bar", ImVec2(0, 30));
		{
			PathTracker(CurrentDir);
		}
		ImGui::EndChild();
		ImGui::Separator();

		ImGui::BeginChild("Contents");
		{
			ImGui::Columns(11, nullptr, false);
			
			for (auto& dirEntry : std::filesystem::directory_iterator(CurrentDir))
			{
				const auto& path = dirEntry.path();
				auto relativePath = std::filesystem::relative(path, AssetPath);
				std::string fileNameString = relativePath.filename().string();
				if (!dirEntry.is_directory())
				{
					if (ImGui::Button(fileNameString.c_str()))
					{
						//do stuff with the files
					}
				} 
			}
		}
		ImGui::EndChild();
	}
	ImGui::EndChild();
}

void Eclipse::AssetBrowser::PathTracker(std::filesystem::path& CurrentPath)
{
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
		if (ImGui::SmallButton((it.u8string()).c_str()))
		{
			newPathId = id;
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
