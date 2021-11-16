#include "pch.h"
#include "DragAndDrop.h"
#include"Editor/Windows/AssetBrowser/AssetBrowser.h"
#include "ECS/ComponentManager/Components/ScriptComponent.h"

namespace Eclipse
{
	void DragAndDrop::StringPayloadSource(const char* id, const std::string& source,
		PayloadSourceType type)
	{
		if (ECGui::BeginDragDropSource())
		{
			ECGui::SetDragDropPayload(id, source.c_str(), source.size() + 1);

			switch (type)
			{
			case PayloadSourceType::PST_TEXT:
				ECGui::TextUnformatted(source.c_str());
				break;
			case PayloadSourceType::PST_IMAGE:
				// For rendering 2D Image -> Need ask Graphics side
				break;
			default:
				break;
			}

			ECGui::EndDragDropSource();
		}
	}

	void DragAndDrop::IndexPayloadSource(const char* id, const int& source, PayloadSourceType type, Entity ID)
	{
		if (ECGui::BeginDragDropSource())
		{
			std::cout << "Being dragged here" << std::endl;
			ECGui::SetDragDropPayload(id, &source, sizeof(source));
			switch (type)
			{
			case PayloadSourceType::PST_TEXT:
				ECGui::TextUnformatted(lexical_cast<std::string>(source).c_str());
				break;
			case PayloadSourceType::PST_IMAGE:
				// For rendering 2D Image -> Need ask Graphics side
				break;
			case PayloadSourceType::PST_ENTITY:
				if (engine->world.CheckComponent<EntityComponent>(ID))
				{
					auto& entCom = engine->world.GetComponent<EntityComponent>(static_cast<Entity>(ID));

					if (entCom.Name != "Untagged Entity")
						ImGui::TextUnformatted(entCom.Name.c_str());
					else
						ImGui::TextUnformatted(lexical_cast_toStr<EntityType>(entCom.Tag).c_str());
				}
				else
				{
					ImGui::TextUnformatted(lexical_cast<std::string>(ID).c_str());
				}
				PrefabID = ID;
				break;
			}

			ECGui::EndDragDropSource();
		}
	}

	void DragAndDrop::StringPayloadTarget(const char* id, std::string& destination,
		const char* cMsg, PayloadTargetType type, Entity ID, size_t arrayIndex)
	{
		if (ECGui::BeginDragDropTarget())
		{
			const ImGuiPayload* payload = ECGui::AcceptDragDropPayload(id);
			std::string path;
			if (payload)
			{
				switch (type)
				{
				case PayloadTargetType::PTT_WIDGET:
					if (std::strcmp(id, "cs") == 0)
					{
						// Put it here Nico, ur script instance thing
						std::filesystem::path temp = ((const char*)payload->Data);
						destination = AssetBrowserWindow::GetFileName(temp.filename().string().c_str());
						auto& scriptCom = engine->world.GetComponent<ScriptComponent>(ID);
						scriptCom.scriptList[arrayIndex] = engine->mono.GetScriptPointerByName(destination);
						//scriptCom.scriptList[arrayIndex]->scriptName = destination;
					}
					else
					{
						destination = (const char*)payload->Data;
					}
					break;
					// Create Meshes and instances
				case PayloadTargetType::PTT_WINDOW:
					// Load Stuff here in the future
					path = ((const char*)payload->Data);
					engine->pfManager.CreatePrefabInstance(path.c_str());
					// FOR JIAN HERNG for creating prefab on scene buffer, take the path
					break;
				case PayloadTargetType::PTT_ASSETS:

					if (!strcmp(id, "dds"))
					{
						std::filesystem::path temp = ((const char*)payload->Data);
						std::string folder = temp.parent_path().filename().string();
						destination = AssetBrowserWindow::GetFileName(temp.filename().string().c_str());

						engine->gPBRManager->GenerateMaterialTexture(folder, destination);
					}
					else if (!strcmp(id, "mat"))
					{
						std::filesystem::path temp = ((const char*)payload->Data);
						std::string folder = temp.parent_path().filename().string();
						destination = AssetBrowserWindow::GetFileName(temp.filename().string().c_str());

						auto& Material = engine->world.GetComponent<MaterialComponent>(ID);
						Material.MaterialInstanceName = destination;
					}
					else
					{
						// FOR JIAN HERNG entID for entity number and destination for path
						// to generate prefab file in asset browser
						engine->pfManager.GeneratePrefab(PrefabID, destination.c_str());
					}
					break;
				case PayloadTargetType::PTT_SCRIPT_LIGHT:
					if (engine->world.CheckComponent<LightComponent>(engine->editorManager->GetEntityID(*((int*)payload->Data))))
					{
						destination = std::to_string(engine->editorManager->GetEntityID(*((int*)payload->Data)));
					}
					else
					{
						cMsg = "Wrong type of entity dragged , entity needs to have LIGHT COMPONENT";
					}
					break;
				case PayloadTargetType::PTT_SCRIPT_AUDIO:
					if (engine->world.CheckComponent<AudioComponent>(engine->editorManager->GetEntityID(*((int*)payload->Data))))
						
					{
						destination = std::to_string(engine->editorManager->GetEntityID(*((int*)payload->Data)));
					}
					else
					{
						cMsg = "Wrong type of entity dragged , entity needs to have AUDIO COMPONENT";
					}
					break;
				case PayloadTargetType::PTT_SCRIPT_GAMEOBJECT:
					if (engine->world.CheckComponent<EntityComponent>(engine->editorManager->GetEntityID(*((int*)payload->Data))))

					{
						destination = std::to_string(engine->editorManager->GetEntityID(*((int*)payload->Data)));
					}
					else
					{
						cMsg = "Wrong type of entity dragged , entity needs to have ENTITY COMPONENT";
					}
					break;
				}

				EDITOR_LOG_INFO(cMsg);
			}

			ECGui::EndDragDropTarget();
		}
	}

	bool DragAndDrop::NodeEditorTextureTarget(const char* id, std::string& destination, const char* cMsg, PayloadTargetType type, Entity ID, size_t arrayIndex)
	{
		(void)arrayIndex;
		(void)ID;
		if (ECGui::BeginDragDropTarget())
		{
			const ImGuiPayload* payload = ECGui::AcceptDragDropPayload(id);
			std::string path;
			if (payload)
			{
				switch (type)
				{
				case PayloadTargetType::PTT_WIDGET:
					destination = (const char*)payload->Data;
					return true;
					break;
				}

				EDITOR_LOG_INFO(cMsg);
			}

			ECGui::EndDragDropTarget();
		}
		return false;
	}

	void DragAndDrop::IndexPayloadTarget(const char* id, const int& destination, bool& IsSelected, PayloadTargetType type)
	{
		if (ECGui::BeginDragDropTarget())
		{
			const ImGuiPayload* payload = ECGui::AcceptDragDropPayload(id);

			if (payload)
			{
				SourceIndex_ = *((int*)payload->Data);
				DestinationIndex_ = destination;
				IsIndexJobSelected = true;
			}

			ECGui::EndDragDropTarget();
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

								TransformComponent* childTransComp = nullptr;
								TransformComponent* parentTransComp = nullptr;

								ChildComponent* childComp = nullptr;

								switch (i)
								{
									// Move index
								case 0:
									engine->editorManager->InsertExistingEntity(static_cast<size_t>(DestinationIndex_),
										engine->editorManager->GetEntityID(SourceIndex_));
									IsIndexJobSelected = false;
									IsSelected = false;
									EDITOR_LOG_INFO("Entity moved.");
									break;
									// Swap index
								case 1:
									engine->editorManager->SwapEntities(static_cast<size_t>(SourceIndex_), static_cast<size_t>(DestinationIndex_));
									IsIndexJobSelected = false;
									IsSelected = false;
									EDITOR_LOG_INFO("Entity positions swapped.");
									break;
									// Parent Child
								case 2:
									DestinationEntCom = &engine->world.GetComponent<EntityComponent>(engine->editorManager->GetEntityID(DestinationIndex_));

									SourceEntCom = &engine->world.GetComponent<EntityComponent>(engine->editorManager->GetEntityID(SourceIndex_));

									if (!engine->world.CheckComponent<ParentComponent>(engine->editorManager->GetEntityID(DestinationIndex_)))
									{
										engine->world.AddComponent(engine->editorManager->GetEntityID(DestinationIndex_), ParentComponent{});
										engine->world.GetComponent<ParentComponent>(engine->editorManager->GetEntityID(DestinationIndex_)).child.push_back(engine->editorManager->GetEntityID(SourceIndex_));
									}
									else
									{
										engine->world.GetComponent<ParentComponent>(engine->editorManager->GetEntityID(DestinationIndex_)).child.push_back(engine->editorManager->GetEntityID(SourceIndex_));
									}

									if (!engine->world.CheckComponent<ChildComponent>(engine->editorManager->GetEntityID(SourceIndex_)))
									{
										engine->world.AddComponent(engine->editorManager->GetEntityID(SourceIndex_), ChildComponent{});
										engine->world.GetComponent<ChildComponent>(engine->editorManager->GetEntityID(SourceIndex_)).parentIndex = engine->editorManager->GetEntityID(DestinationIndex_);
										engine->world.GetComponent<ChildComponent>(engine->editorManager->GetEntityID(SourceIndex_)).hasParent = true;
										engine->world.GetComponent<ChildComponent>(engine->editorManager->GetEntityID(SourceIndex_)).IsAChild = true;
									}
									else
									{
										engine->world.GetComponent<ChildComponent>(engine->editorManager->GetEntityID(SourceIndex_)).parentIndex = engine->editorManager->GetEntityID(DestinationIndex_);
										engine->world.GetComponent<ChildComponent>(engine->editorManager->GetEntityID(SourceIndex_)).hasParent = true;
										engine->world.GetComponent<ChildComponent>(engine->editorManager->GetEntityID(SourceIndex_)).IsAChild = true;
									}

									childComp = &engine->world.GetComponent<ChildComponent>(engine->editorManager->GetEntityID(SourceIndex_));
									childTransComp = &engine->world.GetComponent<TransformComponent>(engine->editorManager->GetEntityID(SourceIndex_));
									parentTransComp = &engine->world.GetComponent<TransformComponent>(engine->editorManager->GetEntityID(DestinationIndex_));
									engine->world.GetComponent<ChildComponent>(engine->editorManager->GetEntityID(SourceIndex_)).PosOffset = childTransComp->position - parentTransComp->position;
									engine->world.GetComponent<ChildComponent>(engine->editorManager->GetEntityID(SourceIndex_)).ScaleOffset.setX(childTransComp->scale.getX() / parentTransComp->scale.getX());
									engine->world.GetComponent<ChildComponent>(engine->editorManager->GetEntityID(SourceIndex_)).ScaleOffset.setY(childTransComp->scale.getY() / parentTransComp->scale.getY());
									engine->world.GetComponent<ChildComponent>(engine->editorManager->GetEntityID(SourceIndex_)).ScaleOffset.setZ(childTransComp->scale.getZ() / parentTransComp->scale.getZ());

									IsSelected = false;
									IsIndexJobSelected = false;
									break;
									// Cancel
								default:
									IsSelected = false;
									IsIndexJobSelected = false;
									break;
								}
							}

						
					}

					ImGui::EndPopup();
				}
				

				ImGuiHoveredFlags flag = ImGuiHoveredFlags_AllowWhenBlockedByPopup | ImGuiHoveredFlags_AnyWindow;

				if (ImGui::IsWindowHovered(flag) && ImGui::IsMouseClicked(0))
				{
					IsIndexJobSelected = false;
				}
				break;
			}
			default:
				break;
			}
		}
	}

	void DragAndDrop::AssetBrowerFilesAndFoldersTarget(const char* type, const char* paths,
		std::string AssetPath, std::filesystem::path dirEntry, bool& refreshBrowser,
		const std::map<std::filesystem::path, std::vector<std::filesystem::path>> & pathMap, bool& CopyMode)
	{

		if (ECGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ECGui::AcceptDragDropPayload(type))
			{
				static std::string folderName;

				static std::string parentPath;

				std::filesystem::path itemPaths = AssetPath.c_str();

				try
				{
					paths = (const char*)payload->Data;

					if (std::filesystem::is_directory(itemPaths.string() + "\\" + paths))
					{
						parentPath = std::filesystem::path(itemPaths / paths).string();
						bool baseFile = false;
						size_t pos = 0;
						int fileSize = 0;
						parentPath = parentPath.substr(0, parentPath.find_last_of("/\\"));
						if (parentPath == AssetPath)
						{
							parentPath = std::filesystem::path(itemPaths / paths).string();
							baseFile = true;
						}
						for (const auto& file : std::filesystem::recursive_directory_iterator(std::filesystem::path(itemPaths / paths)))
						{
							(void)file;
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
												if (baseFile)
												{
													files.insert(std::pair<std::string, std::string>(dirEntry.string() + "\\" + std::filesystem::path(parentPath).filename().string() + folderName, dirEntry.string() + "\\" + std::filesystem::path(parentPath).filename().string() + folderName));
												}
												else
												{
													files.insert(std::pair<std::string, std::string>(dirEntry.string() + folderName, dirEntry.string() + folderName));
												}
											}
											if (baseFile)
											{
												std::filesystem::create_directories(dirEntry.string() + "\\" + std::filesystem::path(parentPath).filename().string());
												std::filesystem::create_directories(dirEntry.string() + "\\" + std::filesystem::path(parentPath).filename().string() + folderName);
											}
											else
											{
												std::filesystem::create_directories(dirEntry.string() + folderName);
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
													files.insert(std::pair<std::string, std::string>(dirEntry.string() + "\\" + std::filesystem::path(parentPath).filename().string() + folderName, dirEntry.string() + "\\" + std::filesystem::path(parentPath).filename().string() + folderName));
												}

												std::filesystem::create_directories(dirEntry.string() + "\\" + std::filesystem::path(parentPath).filename().string());
												std::filesystem::copy(pair2, dirEntry.string() + "\\" + std::filesystem::path(parentPath).filename().string() + folderName);

											}
											else
											{

												if (files.find((parentPath + folderName).c_str()) != files.end())
												{
													folderName = files.at((parentPath + folderName).c_str());
												}
												else
												{
													files.insert(std::pair<std::string, std::string>(dirEntry.string() + folderName, dirEntry.string() + folderName));
												}
												std::filesystem::copy(pair2, dirEntry.string() + folderName);
											}
										}
									}
									if (fileSize == -1)
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

							if (std::filesystem::is_directory(std::filesystem::path(itemPaths / paths)) && once)
							{
								std::filesystem::create_directories(dirEntry.string() + "\\" + std::filesystem::path(itemPaths / paths).filename().string());
								once = false;
							}
							std::filesystem::copy(std::filesystem::path(itemPaths / paths), dirEntry.string() + "//" + folderName, copyOptions);
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
						if (std::filesystem::is_directory(dirEntry))
						{
							std::filesystem::copy(std::filesystem::path(itemPaths / paths), dirEntry);

							if (!CopyMode)
							{
								std::filesystem::remove(std::filesystem::path(itemPaths / paths));
							}
						}
						else
						{
							EDITOR_LOG_WARN("You are copying / moving a file to another file!");
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

					ClearAllFiles(folderName, parentPath, refreshBrowser);
				}
			}
			ECGui::EndDragDropTarget();
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
	void DragAndDrop::CreateEmptyFolder(std::string folderName, std::string folderPath)
	{
		std::string fileLocation = folderPath + folderName;
		std::string outputLog;
		if (std::filesystem::exists(fileLocation))
		{
			outputLog = "FOLDER ALREADY EXIST AT: ";
			outputLog += fileLocation;
			EDITOR_LOG_WARN(outputLog.c_str());
		}
		else
		{
			outputLog = "FOLDER CREATED AT: ";
			outputLog += fileLocation;
			std::filesystem::create_directories(fileLocation);
			EDITOR_LOG_INFO(outputLog.c_str());
		}
	}

}
