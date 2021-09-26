#include "pch.h"
#include "DragAndDrop.h"
#include"Editor/Windows/AssetBrowser/AssetBrowser.h"

namespace Eclipse
{
	void DragAndDrop::StringPayloadSource(const char* id, const std::string& source,
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

	void DragAndDrop::IndexPayloadSource(const char* id, const int& source, PayloadSourceType type)
	{
		if (ImGui::BeginDragDropSource())
		{
			ImGui::SetDragDropPayload(id, &source, sizeof(source));
			switch (type)
			{
			case PayloadSourceType::PST_TEXT:
				ImGui::TextUnformatted(lexical_cast<std::string>(source).c_str());
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

	void DragAndDrop::StringPayloadTarget(const char* id, std::string& destination,
		const char* cMsg, PayloadTargetType type, Entity ID)
	{
		(void)ID;
		if (ImGui::BeginDragDropTarget())
		{
			const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(id);

			if (payload)
			{
				switch (type)
				{
				case PayloadTargetType::PTT_WIDGET:
					if (id == "cs")
					{
						// Put it here Nico, ur script instance thing
						std::filesystem::path temp = ((const char*)payload->Data);
						destination = AssetBrowserWindow::GetFileName(temp.filename().string().c_str());
					}
					else
					{
						destination = (const char*)payload->Data;
					}
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

	void DragAndDrop::IndexPayloadTarget(const char* id, const int& destination, bool IsSelected, PayloadTargetType type)
	{
		if (ImGui::BeginDragDropTarget())
		{
			const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(id);

			if (payload)
			{
				SourceIndex_ = *((int*)payload->Data);
				DestinationIndex_ = destination;
				IsIndexJobSelected = true;
			}

			ImGui::EndDragDropTarget();
		}
		else if (IsSelected && IsIndexJobSelected)
		{
			switch (type)
			{
			case PayloadTargetType::PTT_INDEXEDIT:
			{
				ImGui::OpenPopup("IndexJobList");

				if (ImGui::BeginPopup("IndexJobList"))
				{
					for (int i = 0; i < IM_ARRAYSIZE(IndexJobNames); ++i)
					{
						bool selected = false;
						if (ECGui::CreateSelectableButton(IndexJobNames[i], &selected))
						{
							EntityComponent* DestinationEntCom = nullptr;
							EntityComponent* SourceEntCom = nullptr;

							switch (i)
							{
							// Move index
							case 0:
								engine->editorManager->InsertExistingEntity(static_cast<size_t>(DestinationIndex_),
									engine->editorManager->GetEntityID(SourceIndex_));
								IsIndexJobSelected = false;
								EDITOR_LOG_INFO("Entity moved.");
								break;
							// Swap index
							case 1:
								engine->editorManager->SwapEntities(static_cast<size_t>(SourceIndex_), static_cast<size_t>(DestinationIndex_));
								IsIndexJobSelected = false;
								EDITOR_LOG_INFO("Entity positions swapped.");
								break;
							// Parent Child
							case 2:
								DestinationEntCom = &engine->world.GetComponent<EntityComponent>(engine->editorManager->GetEntityID(DestinationIndex_));
								SourceEntCom = &engine->world.GetComponent<EntityComponent>(engine->editorManager->GetEntityID(SourceIndex_));
								DestinationEntCom->Child.push_back(engine->editorManager->GetEntityID(SourceIndex_));
								SourceEntCom->IsAChild = true;
								SourceEntCom->Parent.push_back(engine->editorManager->GetEntityID(DestinationIndex_));
								IsIndexJobSelected = false;
								break;
							// Cancel
							default:
								IsIndexJobSelected = false;
								break;
							}
						}
					}
					ImGui::EndPopup();
				}
				break;
			}
			default:
				break;
			}
		}
	}
	
	void DragAndDrop::AssetBrowerFilesAndFoldersTarget(const char* type, const char* paths, 
		std::string AssetPath, std::filesystem::directory_entry dirEntry, bool& refreshBrowser, 
		std::map<std::filesystem::path, std::vector<std::filesystem::path>> pathMap, bool& CopyMode)
	{
		static std::string folderName;

		static std::string parentPath;

		std::filesystem::path itemPaths = AssetPath.c_str();

		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(type))
			{
				try
				{
					paths = (const char*)payload->Data;

					if(std::filesystem::is_directory(itemPaths.string() + "\\" + paths))
					{
						parentPath = std::filesystem::path(itemPaths / paths).string();
						bool baseFile = false;
						size_t pos = 0;
						int fileSize = 0;
						parentPath = parentPath.substr(0, parentPath.find_last_of("/\\"));
						if(parentPath ==  AssetPath)
						{
							parentPath = std::filesystem::path(itemPaths / paths).string();
							baseFile = true;
						}
						for (const auto& file : std::filesystem::recursive_directory_iterator(std::filesystem::path(itemPaths / paths)))
						{
							fileSize++;
						}
						for (auto const& pair : pathMap)
						{
							if (pair.first == std::filesystem::path(parentPath.c_str()))
							{
								for (auto const& pair2 : pair.second)
								{
									if (std::filesystem::is_directory(pair2))
									{
										size_t found = pair2.string().find(paths);
										if (found != std::string::npos)
										{
											fileSize--;
											deletefiles.insert(std::pair<std::string, std::string>(pair2.string(), pair2.string()));
											folderName = pair2.string();
											pos = folderName.find(parentPath);
											if (pos != std::string::npos)
											{
												// If found then erase it from string
												folderName.erase(pos, parentPath.length());
											}
											if (files.find((parentPath + folderName).c_str()) != files.end())
											{
												folderName = files.at((parentPath + folderName).c_str());
											}
											else
											{
												if(baseFile)
												{
													files.insert(std::pair<std::string, std::string>(dirEntry.path().string()+ "\\"  + std::filesystem::path(parentPath).filename().string() + folderName, dirEntry.path().string() + "\\" + std::filesystem::path(parentPath).filename().string() + folderName));
												}
												else
												{
													files.insert(std::pair<std::string, std::string>(dirEntry.path().string() + folderName, dirEntry.path().string() + folderName));
												}
											}
											if (baseFile)
											{
												std::filesystem::create_directories(dirEntry.path().string() + "\\" + std::filesystem::path(parentPath).filename().string());
												std::filesystem::create_directories(dirEntry.path().string() + "\\" + std::filesystem::path(parentPath).filename().string() + folderName);
											}
											else
											{
												std::filesystem::create_directories(dirEntry.path().string() + folderName);
											}
										}
									}
									else
									{
										size_t found = pair2.string().find(paths);
										if (found != std::string::npos)
										{
											fileSize--;
											deletefiles.insert(std::pair<std::string, std::string>(pair2.string(), pair2.string()));
											folderName = pair2.string();
											pos = folderName.find(parentPath);
											if (pos != std::string::npos)
											{
												// If found then erase it from string
												folderName.erase(pos, parentPath.length());
											}
											
											if (baseFile)
											{
												deletefiles.insert(std::pair<std::string, std::string>(pair2.string(), pair2.string()));
												if (files.find((parentPath + folderName).c_str()) != files.end())
												{
													folderName = files.at((parentPath + folderName).c_str());
												}
												else
												{
													files.insert(std::pair<std::string, std::string>(dirEntry.path().string() + "\\" + std::filesystem::path(parentPath).filename().string() + folderName, dirEntry.path().string() + "\\" + std::filesystem::path(parentPath).filename().string() + folderName));
												}

												std::filesystem::create_directories(dirEntry.path().string() + "\\" + std::filesystem::path(parentPath).filename().string());
												std::filesystem::copy(pair2, dirEntry.path().string() + "\\" + std::filesystem::path(parentPath).filename().string() + folderName);
								
											}
											else
											{
												
												if (files.find((parentPath + folderName).c_str()) != files.end())
												{
													folderName = files.at((parentPath + folderName).c_str());
												}
												else
												{
													files.insert(std::pair<std::string, std::string>(dirEntry.path().string() + folderName, dirEntry.path().string() + folderName));
												}
												std::filesystem::copy(pair2, dirEntry.path().string() + folderName);
											}
										}
									}
									if(fileSize == -1 )
									{
										break;
									}
								}
								
								if (!CopyMode)
								{
									for (auto const& it : deletefiles)
									{
										std::filesystem::remove_all(std::filesystem::path(it.first.c_str()));
									}
								}
							
								if (baseFile)
								{
									baseFile = false;
									if (!CopyMode)
									{
										std::filesystem::remove(std::filesystem::path(parentPath));
									}
								}
							}
						}
						
						folderName = std::filesystem::path(paths).filename().string();

						bool once = true;

						for (int i = 1; i <= fileSize; ++i)
						{
							const auto copyOptions = std::filesystem::copy_options::update_existing
								| std::filesystem::copy_options::recursive;

							if(std::filesystem::is_directory(std::filesystem::path(itemPaths / paths)) && once)
							{
								std::filesystem::create_directories(dirEntry.path().string() + "\\" + std::filesystem::path(itemPaths / paths).filename().string());
								once = false;
							}
							std::filesystem::copy(std::filesystem::path(itemPaths / paths), dirEntry.path().string() + "//" + folderName, copyOptions);
						}
						
						if (!CopyMode)
						{
							std::filesystem::remove_all(std::filesystem::path(itemPaths / paths));
						}
						
						//resetting for the next moving of files
						ClearAllFiles(folderName, parentPath, refreshBrowser);
					}
					else
					{
						std::filesystem::copy(std::filesystem::path(itemPaths / paths), dirEntry.path());

						if(!CopyMode)
						{
							std::filesystem::remove(std::filesystem::path(itemPaths / paths));
						}
					}
					refreshBrowser = true;
				}
				catch (std::filesystem::filesystem_error& e)
				{
					EDITOR_LOG_WARN(e.what());

					if (!CopyMode)
					{
						std::filesystem::remove_all(std::filesystem::path(itemPaths / paths));
					}

					ClearAllFiles(folderName,parentPath, refreshBrowser);
				}
			}
			ImGui::EndDragDropTarget();
		}
	}
	void DragAndDrop::ClearAllFiles(std::string& folderName, std::string& parentPath, bool& refreshBrowser)
	{
		deletefiles.clear();
		files.clear();
		folderName.clear();
		parentPath.clear();
		refreshBrowser = true;
	}
}
