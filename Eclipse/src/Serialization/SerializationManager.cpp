#include "pch.h"
#include "SerializationManager.h"
#include "Editor/Windows/Debug/Debug.h"
#include "ECS/SystemManager/Systems/System/ParentChildSystem/ParentSystem/ParentSystem.h"

namespace Eclipse
{
	Serializer SerializationManager::sz;
	Deserializer SerializationManager::dsz;

	SerializationManager::SerializationManager() 
	{
		std::filesystem::create_directories("Config");
	}

	void SerializationManager::Backup::SaveBackup(Serializer& targetSz)
	{
		targetSz.SaveBackup(_backup, backUpPath);
	}

	void SerializationManager::Backup::LoadBackup(Deserializer& targetDsz)
	{
		targetDsz.LoadBackup(_backup, backUpPath);
		_backup.Clear();
	}

	const char* SerializationManager::Backup::GetBackUpPath()
	{
		return backUpPath;
	}

	void SerializationManager::PostUpdate::RegisterForPostUpdate(World&, const Entity& oldEnt, const Entity& newEnt)
	{
		oldToNewMap[oldEnt] = newEnt;
	}

	void SerializationManager::PostUpdate::RunPostUpdate()
	{
		auto& w = engine->world;

		for (auto& pair : oldToNewMap)
		{
			if (w.CheckComponent<ParentComponent>(pair.second))
			{
				auto& parentComp = w.GetComponent<ParentComponent>(pair.second);
				for (auto& ent : parentComp.child)
				{
					ent = oldToNewMap[ent];
				}
			}

			if (w.CheckComponent<ChildComponent>(pair.second))
			{
				auto& childComp = w.GetComponent<ChildComponent>(pair.second);
				childComp.parentIndex = oldToNewMap[childComp.parentIndex];
			}

			if (w.CheckComponent<AIComponent>(pair.second))
			{
				auto& aiComp = w.GetComponent<AIComponent>(pair.second);
				for (auto& ent : aiComp.waypoints)
				{
					ent = oldToNewMap[ent];
				}
			}

			if (w.CheckComponent<ScriptComponent>(pair.second))
			{
				auto& scriptComp = w.GetComponent<ScriptComponent>(pair.second);

				for (auto script : scriptComp.scriptList) 
				{
					for (auto& var : script->vars)
					{
						if (!var.varValue.empty() && (var.type == m_Type::MONO_LIGHT || var.type == m_Type::MONO_AUDIO))
						{
							Entity old = lexical_cast<Entity>(var.varValue);
							var.varValue = lexical_cast<std::string>(oldToNewMap[old]);
						}
					}
				}
			}
		}
		Clear();
	}

	void SerializationManager::SerializeEntity(World& w, const Entity& ent, const size_t& counter)
	{
		sz.StartElement("Entity_", true, counter);
		sz.AddAttributeToElement("ID", ent);
		SerializeAllComponents(w, ent);
		sz.CloseElement();
	}

	Entity SerializationManager::DeserializeEntity(World& w, const size_t& counter, bool PrefabUse)
	{
		Entity ent = MAX_ENTITY;
		if (dsz.StartElement("Entity_", true, counter))
		{
			Entity oldEnt;
			dsz.ReadAttributeFromElement("ID", oldEnt);
			ent = w.CreateEntity();
			if (DeserializeAllComponents(w, ent, PrefabUse))
			{
				if (!PrefabUse)
				{
					pUpdate.RegisterForPostUpdate(w, oldEnt, ent);
					engine->editorManager->RegisterExistingEntity(ent);
				}
			}
			dsz.CloseElement();
		}
		return ent;
	}

	void SerializationManager::SerializeAllComponents(World& w, const Entity& ent)
	{
		SerializeListedComponent(w, ent, all_component_list);
	}

	bool SerializationManager::DeserializeAllComponents(World& w, const Entity& ent, bool PrefabUse)
	{
		bool isSuccess = false;

		if (DeserializeListedComponent(w, ent, all_component_list))
		{
			if (!PrefabUse)
			{
				if (w.CheckComponent<CameraComponent>(ent))
				{
					auto& camera = w.GetComponent<CameraComponent>(ent);
					engine->gCamera.ReInitCameraList(camera.camType, ent);
				}

				if (w.CheckComponent<AABBComponent>(ent))
				{
					auto& aabb = w.GetComponent<AABBComponent>(ent);
					engine->gCullingManager->Insert(aabb, ent);
					engine->gDynamicAABBTree.InsertData(ent);
					engine->gPicker.UpdateAabb(ent);
				}
			}
			isSuccess = true;
		}
		else
		{
			if (w.CheckComponent<EntityComponent>(ent))
			{
				auto& entity = w.GetComponent<EntityComponent>(ent);
				std::string msg = "Entity \"";
				msg += entity.Name + "\" cannot be deserialized due to corrupted component.";
				EDITOR_LOG_WARN(msg.c_str())
			}
			else
			{
				std::string msg = "Unknown entity cannot be deserialized due to corrupted component.";
				EDITOR_LOG_WARN(msg.c_str())
			}
			w.DestroyEntity(ent);
		}
		return isSuccess;
	}

	void SerializationManager::SerializeAllEntity()
	{
		const std::vector<Entity>& entities = engine->editorManager->GetEntityListByConstRef();
		World& w = engine->world;

		sz.StartElement("Entities");
		sz.AddAttributeToElement("Size", entities.size());
		size_t counter = 0;
		for (auto const& ent : entities)
		{
			SerializeEntity(w, ent, counter++);
		}
		sz.CloseElement();
	}

	void SerializationManager::DeserializeAllEntity()
	{
		World& w = engine->world;

		if (dsz.StartElement("Entities"))
		{
			size_t size = 0;
			dsz.ReadAttributeFromElement("Size", size);

			for (size_t i = 0; i < size; ++i)
			{
				DeserializeEntity(w, i);
			}
		}
		dsz.CloseElement();
	}

	void SerializationManager::SaveFile(const char* fullpath)
	{
		sz.SaveXML(fullpath);
	}

	bool SerializationManager::LoadFile(const char* fullpath)
	{
		return dsz.LoadXML(fullpath);
	}

	SerializationManager::~SerializationManager()
	{
	}

	void SerializationManager::SaveEngineConfig(const std::string& windowName, const int& windowWidth,
		const int& windowHeight, const bool& editorState, const bool& isFullscreen)
	{
		sz.StartElement("EngineConfig");
		{
			sz.StartElement("Window");
			{
				sz.AddAttributeToElement("Name", windowName);
				sz.AddAttributeToElement("Width", windowWidth);
				sz.AddAttributeToElement("Height", windowHeight);
				sz.CloseElement();
			}

			sz.StartElement("EditorState");
			{
				sz.AddAttributeToElement("Bool", editorState);
				sz.CloseElement();
			}

			sz.StartElement("VSYNC");
			{
				sz.AddAttributeToElement("Bool", false);
				sz.CloseElement();
			}

			sz.StartElement("Fullscreen");
			{
				sz.AddAttributeToElement("Bool", isFullscreen);
				sz.CloseElement();
			}
			
			sz.CloseElement();
		}
		SaveFile("Config\\Engine.cfg");
	}

	void SerializationManager::SaveDefaultEngineConfig()
	{
		SaveEngineConfig();
	}

	void SerializationManager::LoadEngineConfig(std::string& windowName, int& windowWidth, int& windowHeight, bool& editorState, bool& isFullscreen)
	{
		if (LoadFile("Config\\Engine.cfg"))
		{
			if(dsz.StartElement("EngineConfig"))
			{
				if (dsz.StartElement("Window"))
				{
					dsz.ReadAttributeFromElement("Name", windowName);
					dsz.ReadAttributeFromElement("Width", windowWidth);
					dsz.ReadAttributeFromElement("Height", windowHeight);
					dsz.CloseElement();
				}

				if (dsz.StartElement("EditorState"))
				{
					dsz.ReadAttributeFromElement("Bool", editorState);
					dsz.CloseElement();
				}

				if (dsz.StartElement("Fullscreen"))
				{
					dsz.ReadAttributeFromElement("Bool", isFullscreen);
					dsz.CloseElement();
				}

				dsz.CloseElement();
			}
		}
		else
		{
			SaveDefaultEngineConfig();
		}
	}

	void SerializationManager::SaveSceneFile(const char* fullpath)
	{
		engine->pfManager.EndUpdate();
		SerializeAllEntity();
		SaveFile(fullpath);
	}

	void SerializationManager::LoadSceneFile(const char* fullpath)
	{
		if (LoadFile(fullpath))
		{
			DeserializeAllEntity();
			pUpdate.RunPostUpdate();
		}
	}

	void SerializationManager::SaveLayersConfigFile(const std::unordered_map<int, std::string>& LayerList,
		const size_t& UserLayerSize, const char* fullpath)
	{
		sz.StartElement("LayersConfig");
		{
			sz.StartElement("LayerSize");
			{
				sz.AddAttributeToElement("Overall", LayerList.size());
				sz.AddAttributeToElement("UserDefined", UserLayerSize);

				sz.StartElement("LayerNames");
				{
					for (const auto& [ID, str] : LayerList)
					{
						sz.StartElement("Layer_", true, ID);
						{
							sz.AddAttributeToElement("Index", ID);
							sz.AddAttributeToElement("Name", str.c_str());
						}
						sz.CloseElement();
					}
				}
				sz.CloseElement();
			}
			sz.CloseElement();
		}
		sz.CloseElement();
		
		SaveFile(fullpath);
	}

	bool SerializationManager::LoadLayersConfigFile(std::unordered_map<int, std::string>& LayerList,
		size_t& UserLayerSize, const char* fullpath)
	{
		if (LoadFile(fullpath))
		{
			if (dsz.StartElement("LayersConfig"))
			{
				if (dsz.StartElement("LayerSize"))
				{
					size_t size = 0;
					dsz.ReadAttributeFromElement("Overall", size);
					dsz.ReadAttributeFromElement("UserDefined", UserLayerSize);

					if (dsz.StartElement("LayerNames"))
					{
						for (size_t i = 0; i < size; ++i)
						{
							if (dsz.StartElement("Layer_", true, i))
							{
								int index;
								std::string name;
								name.reserve(256);
								dsz.ReadAttributeFromElement("Index", index);
								dsz.ReadAttributeFromElement("Name", name);
								LayerList[index] = name;
							}
							dsz.CloseElement();
						}
					}
					dsz.CloseElement();
				}
				dsz.CloseElement();
			}
			dsz.CloseElement();

			return true;
		}

		return false;
	}

	void SerializationManager::SaveBackupFile()
	{
		engine->pfManager.EndUpdate();
		SerializeAllEntity();
		backup.SaveBackup(sz);
	}

	void SerializationManager::LoadBackupFile()
	{
		SceneManager::Clear();
		backup.LoadBackup(dsz);
		DeserializeAllEntity();
		pUpdate.RunPostUpdate();
		std::filesystem::remove(std::filesystem::path(backup.GetBackUpPath()));
	}

	const char* SerializationManager::GetBackUpPath()
	{
		return backup.GetBackUpPath();
	}

	void SerializationManager::SavePrefab(const EUUID& prefabID, const Entity& ent, bool IsFromMainWorld)
	{
		World& prefabW = IsFromMainWorld ? engine->world : engine->prefabWorld;
		size_t counter = 0;
		size_t tempSize = 0;
		sz.AddAttributeToElement("PrefabID", prefabID);

		SerializeEntity(prefabW, ent, counter);

		sz.StartElement("Children");
		if (prefabW.CheckComponent<ParentComponent>(ent))
		{
			auto& parentComp = prefabW.GetComponent<ParentComponent>(ent);

			for (auto child : parentComp.child)
			{
				sz.StartElement("Child", true, counter++);
				auto& prefabComp = prefabW.GetComponent<PrefabComponent>(child);
				SavePrefab(prefabComp.PrefabID, child, IsFromMainWorld);
				sz.CloseElement();
			}
			tempSize = parentComp.child.size();
		}
		
		sz.AddAttributeToElement("Size", tempSize);
		sz.CloseElement();
	}

	void SerializationManager::LoadPrefabCleanUp(World& world, const Entity& ent)
	{
		if (world.CheckComponent<ParentComponent>(ent))
		{
			auto& comp = world.GetComponent<ParentComponent>(ent);
			comp.child.clear();
		}
	}

	void SerializationManager::UpdateParentChild(World& world, const Entity& parentEnt, const Entity& childEnt, bool IsFromMainWorld)
	{
		if (world.CheckComponent<ParentComponent>(parentEnt))
		{
			auto& parentComp = world.GetComponent<ParentComponent>(parentEnt);
			parentComp.child.push_back(childEnt);
		}
		else
		{
			ParentComponent parentComp;
			parentComp.child.push_back(childEnt);
			world.AddComponent<ParentComponent>(parentEnt, parentComp);
		}

		if (world.CheckComponent<ChildComponent>(childEnt))
		{
			auto& childComp = world.GetComponent<ChildComponent>(childEnt);
			childComp.parentIndex = parentEnt;

			if (IsFromMainWorld)
			{
				TransformComponent& childTransComp = engine->world.GetComponent<TransformComponent>(childEnt);
				TransformComponent& parentTransComp = engine->world.GetComponent<TransformComponent>(parentEnt);

				engine->world.GetSystem<ParentSystem>()->UpdateChildPosition(parentEnt, childEnt);
				childComp.RotOffset = childTransComp.rotation - parentTransComp.rotation;
			}
		}
		else
		{
			ChildComponent childComp;
			childComp.parentIndex = parentEnt;
			world.AddComponent<ChildComponent>(childEnt, childComp);
		}
	}

	EUUID SerializationManager::LoadPrefab(Entity& dszEnt, bool IsFromMainWorld)
	{
		World& prefabW = IsFromMainWorld ? engine->world : engine->prefabWorld;
		EUUID PrefabID = 0;

		dsz.ReadAttributeFromElement("PrefabID", PrefabID);

		dszEnt = DeserializeEntity(prefabW, 0, true);

		LoadPrefabCleanUp(prefabW, dszEnt);

		if (dsz.StartElement("Children"))
		{
			size_t size = 0;
			dsz.ReadAttributeFromElement("Size", size);
			for (size_t i = 0; i < size; ++i)
			{
				Entity child = MAX_ENTITY;
				dsz.StartElement("Child", true, i);
				LoadPrefab(child, IsFromMainWorld);
				UpdateParentChild(prefabW, dszEnt, child, IsFromMainWorld);
				dsz.CloseElement();
			}

			dsz.CloseElement();
		}

		return PrefabID;
	}

	bool SerializationManager::CheckBackUpPathExistence()
	{
		if (std::filesystem::exists(backup.GetBackUpPath()))
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	void SerializationManager::SavePrefabFile(const EUUID& prefabID, const Entity& ent, const char* path, bool IsFromMainWorld)
	{
		sz.StartElement("Prefab");
		SavePrefab(prefabID, ent, IsFromMainWorld);
		sz.CloseElement();
		SaveFile(path);
	}

	EUUID SerializationManager::LoadPrefabFile(Entity& dszEnt, const char* fullpath, bool IsFromMainWorld)
	{
		EUUID PrefabID = 0;
		if (LoadFile(fullpath))
		{
			if (dsz.StartElement("Prefab"))
			{
				PrefabID = LoadPrefab(dszEnt, IsFromMainWorld);
				dsz.CloseElement();
			}
		}
		
		return PrefabID;
	}

	//Debug purpose
	void SerializationManager::SavePrefabWorld(const std::set<Entity>& entities)
	{
		World& prefabW = engine->prefabWorld;

		sz.StartElement("Entities");
		sz.AddAttributeToElement("Size", entities.size());
		size_t counter = 0;
		for (auto const& ent : entities)
		{
			if(prefabW.CheckComponent<PrefabComponent>(ent))
			{
				auto& prefabComp = prefabW.GetComponent<PrefabComponent>(ent);

				if (prefabComp.IsInstance)
				{
					continue;
				}

				SerializeEntity(prefabW, ent, counter++);
			}
		}
		sz.CloseElement();
	}

	void SerializationManager::SavePrefabWorldFile(const std::set<Entity>& entities)
	{
		SavePrefabWorld(entities);
		std::string path = PrefabManager::PrefabPath;
		path += "List.xml";
		SaveFile(path.c_str());
	}

	Entity SerializationManager::PostUpdate::GetNew(const Entity& oldEnt)
	{
		if (oldToNewMap.find(oldEnt) != oldToNewMap.end())
			return oldToNewMap[oldEnt];
		else
			return MAX_ENTITY;
	}
	void SerializationManager::PostUpdate::Clear()
	{
		oldToNewMap.clear();
	}
}