#include "pch.h"
#include "DragAndDrop.h"

namespace Eclipse
{
	void DragAndDrop::GenericPayloadSource(const char* id, const std::string& source,
		PayloadSourceType type)
	{
		if (ImGui::BeginDragDropSource())
		{
			ImGui::SetDragDropPayload(id, source.c_str(), source.size() + 1);

			switch (type)
			{
				case PayloadSourceType::PST_TEXT:
					ImGui::TextUnformatted(source.c_str());
					break;
				case PayloadSourceType::PST_IMAGE:
					// For rendering 2D Image -> Need ask Graphics side
					break;
				default:
					break;
			}

			ImGui::EndDragDropSource();
		}
	}

	void DragAndDrop::GenericPayloadTarget(const char* id, std::string& destination,
		const char* cMsg, PayloadTargetType type)
	{
		if (ImGui::BeginDragDropTarget())
		{
			const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(id);

			if (payload)
			{
				switch (type)
				{
				case PayloadTargetType::PTT_WIDGET:
					destination = (const char*)payload->Data;
					break;
				// Create Meshes and instances
				case PayloadTargetType::PTT_WINDOW:
					// Load Stuff here in the future
					break;
				default:
					break;
				}

				EDITOR_LOG_INFO(cMsg);
			}

			ImGui::EndDragDropTarget();
		}
	}
	
	void DragAndDrop::AssetBrowerFilesAndFoldersTarget(const char* type, const char* paths, 
		std::string AssetPath, std::filesystem::directory_entry dirEntry, bool refreshBrowser, std::map<std::filesystem::path, std::vector<std::filesystem::path>> pathMap)
	{
		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(type))
			{
				try
				{
					paths = (const char*)payload->Data;
					std::filesystem::path itemPaths = AssetPath.c_str();
					if(std::filesystem::is_directory(itemPaths.string() + "\\" + paths))
					{
						std::string parentPath = std::filesystem::path(itemPaths / paths).string();
						static std::string folderName;
						size_t pos = 0;
						parentPath = parentPath.substr(0, parentPath.find_last_of("/\\"));
						std::string EndingPath = dirEntry.path().filename().string();
						EndingPath = EndingPath.substr(0, EndingPath.find_last_of("/\\"));
						std::string token = EndingPath;
						for (auto const& pair : pathMap)
						{
							if (pair.first == std::filesystem::path(parentPath.c_str()))
							{
								for (auto const& pair2 : pair.second)
								{
									size_t found = pair2.string().find(paths);
									if (found != std::string::npos)
									{
										deletefiles.insert(std::pair<std::string, std::string>(pair2.string(), pair2.string()));
										folderName = pair2.string();
										pos = folderName.find(parentPath);
										if (pos != std::string::npos)
										{
											// If found then erase it from string
											folderName.erase(pos, parentPath.length());
										}

										if (std::filesystem::is_directory(pair2))
										{
											if (files.find((parentPath + folderName).c_str()) != files.end())
											{
												folderName = files.at((parentPath + folderName).c_str());
											}
											else
											{
												files.insert(std::pair<std::string, std::string>(dirEntry.path().string() + folderName, dirEntry.path().string() + "\\" + folderName));
											}
											std::filesystem::create_directories(dirEntry.path().string() + folderName);
										}
										else
										{
											std::filesystem::copy(pair2, dirEntry.path().string() + folderName);
										}
									}
								}
							}

							for (auto const& it : deletefiles)
							{
								std::filesystem::remove_all(std::filesystem::path(it.first.c_str()));
							}
							deletefiles.clear();
							files.clear();
						}
					}
					else
					{
						std::filesystem::copy(std::filesystem::path(itemPaths / paths), dirEntry.path());
						std::filesystem::remove(std::filesystem::path(itemPaths / paths));
					}
					refreshBrowser = true;
				}
				catch (std::filesystem::filesystem_error& e)
				{
					std::cout << e.what() << '\n';
				}
			}
			ImGui::EndDragDropTarget();
		}
	}
}
