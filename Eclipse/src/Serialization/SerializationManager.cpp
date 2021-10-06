#include "pch.h"
#include "SerializationManager.h"

namespace Eclipse
{
	Serializer SerializationManager::sz;
	Deserializer SerializationManager::dsz;

	SerializationManager::SerializationManager() {}

	void SerializationManager::Backup::SaveBackup(Serializer& targetSz)
	{
		backUpPath = TEMP_PATH;
		backUpPath += SceneManager::GetCurrentSceneName();
		backUpPath += "RunningTemp";
		backUpPath += SCENE_EXTENSION;
		targetSz.SaveBackup(_backup, backUpPath);
	}

	void SerializationManager::Backup::LoadBackup(Deserializer& targetDsz)
	{
		targetDsz.LoadBackup(_backup, backUpPath);
		backUpPath.clear();
		_backup.Clear();
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
			ent = w.CreateEntity();
			if (DeserializeAllComponents(w, ent, PrefabUse))
			{
				if (!PrefabUse)
				{
					engine->editorManager->RegisterExistingEntity(ent);
				}
			}
			dsz.CloseElement();
		}
		return ent;
	}

	void SerializationManager::SerializeAllComponents(World& w, const Entity& ent)
	{
		SerializeComponent<AABBComponent>(w, ent);

		SerializeComponent<AIComponent>(w, ent);
		
		SerializeComponent<AudioComponent>(w, ent);

		SerializeComponent<CameraComponent>(w, ent);

		SerializeComponent<CollisionComponent>(w, ent);

		SerializeComponent<DirectionalLightComponent>(w, ent);

		SerializeComponent<EntityComponent>(w, ent);

		SerializeComponent<LightComponent>(w, ent);

		SerializeComponent<MaterialComponent>(w, ent);

		SerializeComponent<MeshComponent>(w, ent);

		SerializeComponent<ModelComponent>(w, ent);

		SerializeComponent<PointLightComponent>(w, ent);

		SerializeComponent<PrefabComponent>(w, ent);
		
		SerializeComponent<RigidBodyComponent>(w, ent);
		
		SerializeComponent<SpotLightComponent>(w, ent);

		SerializeComponent<TransformComponent>(w, ent);
	}

	bool SerializationManager::DeserializeAllComponents(World& w, const Entity& ent, bool PrefabUse)
	{
		bool isSuccess = false;

		if (DeserializeComponent<AABBComponent>(w, ent) &&
			DeserializeComponent<AIComponent>(w, ent) &&
			DeserializeComponent<AudioComponent>(w, ent) &&
			DeserializeComponent<CameraComponent>(w, ent) &&
			DeserializeComponent<CollisionComponent>(w, ent) &&
			DeserializeComponent<DirectionalLightComponent>(w, ent) &&
			DeserializeComponent<EntityComponent>(w, ent) &&
			DeserializeComponent<LightComponent>(w, ent) &&
			DeserializeComponent<MaterialComponent>(w, ent) &&
			DeserializeComponent<MeshComponent>(w, ent) &&
			DeserializeComponent<ModelComponent>(w, ent) &&
			DeserializeComponent<PointLightComponent>(w, ent) &&
			DeserializeComponent<PrefabComponent>(w, ent) &&
			DeserializeComponent<RigidBodyComponent>(w, ent) &&
			DeserializeComponent<SpotLightComponent>(w, ent) &&
			DeserializeComponent<TransformComponent>(w, ent))
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
		SerializeAllEntity();
		SaveFile(fullpath);
	}

	void SerializationManager::LoadSceneFile(const char* fullpath)
	{
		if (LoadFile(fullpath))
		{
			DeserializeAllEntity();
		}
	}

	void SerializationManager::SaveBackupFile()
	{
		SerializeAllEntity();
		backup.SaveBackup(sz);
	}

	void SerializationManager::LoadBackupFile()
	{
		SceneManager::Clear();
		backup.LoadBackup(dsz);
		DeserializeAllEntity();
		std::filesystem::remove_all(TEMP_PATH);
	}

	void SerializationManager::SavePrefab(long long unsigned int prefabID, std::vector<Entity>& prefabContents)
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

	long long unsigned int SerializationManager::LoadPrefab(Entity& dszEnt)
	{
		World& prefabW = engine->prefabWorld;
		long long unsigned int PrefabID = 0;

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

	void SerializationManager::SavePrefabFile(unsigned long long int prefabID, std::vector<Entity>& prefabContents, const char* path)
	{
		SavePrefab(prefabID, prefabContents);
		SaveFile(path);
	}

	long long unsigned int SerializationManager::LoadPrefabFile(Entity& dszEnt, const char* fullpath)
	{
		long long unsigned int PrefabID = 0;
		if (LoadFile(fullpath))
		{
			PrefabID =  LoadPrefab(dszEnt);
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
}