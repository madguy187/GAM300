#include "pch.h"

namespace Eclipse
{
	//to be changed
	static const std::filesystem::path s_AssetPath = "Assets";
}
Eclipse::AssetBrowser::AssetBrowser()
	:m_CurrentDir(s_AssetPath), m_SecondDir()
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
	ImGui::Columns(2);
	ImGui::SetColumnOffset(1, 240);
	//left side
	ImGui::BeginChild("##folders_common");
	{
		if (ImGui::CollapsingHeader("Content", nullptr, ImGuiTreeNodeFlags_DefaultOpen))
		{
			if (m_CurrentDir != std::filesystem::path(s_AssetPath))
			{
				if (ImGui::Button("..."))
				{
					m_CurrentDir = m_CurrentDir.parent_path();
				}
			}
			for (auto& dirEntry : std::filesystem::directory_iterator(m_CurrentDir))
			{
				const auto& path = dirEntry.path();
				auto relativePath = std::filesystem::relative(path, s_AssetPath);
				std::string relativePathStr = relativePath.string();
				if (dirEntry.is_directory())
				{

					if (ImGui::TreeNode(relativePathStr.c_str()))
					{
						
						m_SecondDir = m_CurrentDir;
						m_SecondDir /= path.filename();
						if(!std::filesystem::exists(m_SecondDir))
						{
							m_SecondDir = m_CurrentDir.parent_path();
						}
						for (auto& secondEntry : std::filesystem::directory_iterator(m_SecondDir))
						{
							if(secondEntry.is_directory())
							{
								const auto& path2 = secondEntry.path();
								auto relativePath2 = std::filesystem::relative(path2, s_AssetPath);
								std::string relativePathStr2 = relativePath2.string();
								if (ImGui::TreeNode(relativePathStr2.c_str()))
								{
									ImGui::TreePop();
								}
								
							}
						}
						
						ImGui::TreePop();
					}
					
					if (ImGui::IsMouseDoubleClicked(0) && ImGui::IsItemClicked(0))
					{
						m_CurrentDir /= path.filename();
					}

					//if (ImGui::Button(relativePathStr.c_str()))
					//{
					//	m_CurrentDir /= path.filename();
					//}
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
			int secIdx = 0, newPwdLastSecIdx = -1;
			int secIdxside = 0, newPwdLastSecIdxside = -1;
			for (auto& sec : s_AssetPath)
			{
				ImGui::PushID(secIdx);
				if (secIdx > 0)
					ImGui::SameLine();
				if (ImGui::SmallButton((sec.u8string()).c_str()))
					newPwdLastSecIdx = secIdx;
				ImGui::PopID();
				++secIdx;
			}

			if (newPwdLastSecIdx >= 0)
			{
				int i = 0;
				std::filesystem::path newPwd;
				for (auto& sec : s_AssetPath)
				{
					if (i++ > newPwdLastSecIdx)
						break;
					newPwd /= sec;
				}
				m_CurrentDir = newPwd;
				m_SecondDir = newPwd;
			}
		}
		ImGui::EndChild();
		ImGui::Separator();

		ImGui::BeginChild("Scrolling");
		{
			for (auto& dirEntry : std::filesystem::directory_iterator(m_CurrentDir))
			{
				const auto& path = dirEntry.path();
				auto relativePath = std::filesystem::relative(path, s_AssetPath);
				std::string fileNameString = relativePath.filename().string();
				if (!dirEntry.is_directory())
				{
					if (ImGui::Button(fileNameString.c_str()))
					{
						
					}
				} 
			}
		}
		ImGui::EndChild();
	}
	ImGui::EndChild();
}
