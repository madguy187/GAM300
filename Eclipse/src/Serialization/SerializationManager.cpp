#include "pch.h"
#include "SerializationManager.h"
#include "ECS/SystemManager/Systems/System/ParentChildSystem/ParentSystem/ParentSystem.h"
#include "ECS/ComponentManager/Components/ParentComponent.h"
#include "ECS/ComponentManager/Components/ChildComponent.h"
namespace Eclipse
{
	Serializer SerializationManager::sz;
	Deserializer SerializationManager::dsz;

	SerializationManager::SerializationManager() {}

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

	void SerializationManager::ParentChildPostUpdate::RegisterForPostUpdate(World& w, const Entity& oldEnt, const Entity& newEnt)
	{
		if (w.CheckComponent<ParentComponent>(newEnt) || w.CheckComponent<ChildComponent>(newEnt))
		{
			oldToNewMap[oldEnt] = newEnt;
		}
	}

	void SerializationManager::ParentChildPostUpdate::PostUpdate()
	{
		auto& w = engine->world;
		const auto& entSet = w.GetSystem<ParentSystem>()->mEntities;

		for (auto& ent : entSet)
		{
			auto& parent = w.GetComponent<ParentComponent>(ent);
			auto& parentEntComp = w.GetComponent<EntityComponent>(ent);

			for (auto& parentEnt : parentEntComp.Child)
			{
				parentEnt = oldToNewMap[parentEnt];
			}

			for (auto& child : parent.child)
			{
				child = oldToNewMap[child];
				
				auto& childComp = w.GetComponent<ChildComponent>(child);
				auto& childEntComp = w.GetComponent<EntityComponent>(child);
				childComp.parentIndex = ent;

				for (auto& childEnt : childEntComp.Parent)
				{
					childEnt = oldToNewMap[childEnt];
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
					PCPostUpdate.RegisterForPostUpdate(w, oldEnt, ent);
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
			PCPostUpdate.PostUpdate();
		}
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
		PCPostUpdate.PostUpdate();
		std::filesystem::remove(std::filesystem::path(backup.GetBackUpPath()));
	}

	const char* SerializationManager::GetBackUpPath()
	{
		return backup.GetBackUpPath();
	}

	void SerializationManager::SavePrefab(const EUUID& prefabID, std::vector<Entity>& prefabContents)
	{
		World& prefabW = engine->prefabWorld;
		size_t counter = 0;

		sz.StartElement("Prefab");
		sz.AddAttributeToElement("PrefabID", prefabID);
		sz.AddAttributeToElement("Size", prefabContents.size());
		for (auto ent : prefabContents)
		{
			SerializeEntity(prefabW, ent, counter++);
		}
		sz.CloseElement();
	}

	EUUID SerializationManager::LoadPrefab(Entity& dszEnt, bool IsFromMainWorld)
	{
		World& prefabW = IsFromMainWorld ? engine->world : engine->prefabWorld;
		EUUID PrefabID = 0;

		if(dsz.StartElement("Prefab"))
		{
			size_t size = 0;
			dsz.ReadAttributeFromElement("Size", size);
			dsz.ReadAttributeFromElement("PrefabID", PrefabID);
			for (size_t i = 0; i < size; ++i)
			{
				Entity ent = DeserializeEntity(prefabW, i, true);

				if (i == 0)
				{
					dszEnt = ent;
				}
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

	void SerializationManager::SavePrefabFile(const EUUID& prefabID, std::vector<Entity>& prefabContents, const char* path)
	{
		SavePrefab(prefabID, prefabContents);
		SaveFile(path);
	}

	EUUID SerializationManager::LoadPrefabFile(Entity& dszEnt, const char* fullpath, bool IsFromMainWorld)
	{
		EUUID PrefabID = 0;
		if (LoadFile(fullpath))
		{
			PrefabID =  LoadPrefab(dszEnt, IsFromMainWorld);
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

	Entity SerializationManager::ParentChildPostUpdate::GetNew(const Entity& oldEnt)
	{
		if (oldToNewMap.find(oldEnt) != oldToNewMap.end())
			return oldToNewMap[oldEnt];
		else
			return MAX_ENTITY;
	}
	void SerializationManager::ParentChildPostUpdate::Clear()
	{
		oldToNewMap.clear();
	}
}