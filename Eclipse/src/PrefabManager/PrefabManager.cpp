#include "pch.h"
#include "PrefabManager.h"
#include "../ECS/ComponentManager/Components/PrefabComponent.h"
#include "../ECS/ComponentManager/Components/ParentComponent.h"
#include "../ECS/ComponentManager/Components/ChildComponent.h"
#include "../ECS/ComponentManager/Components/ScriptComponent.h"
#include "../ECS/ComponentManager/Components/AIComponent.h"
#include "ECS/SystemManager/Systems/System/PrefabSystem/PrefabSystem.h"

namespace Eclipse
{
	const std::string PrefabManager::PrefabPath = "src\\Assets\\Prefabs\\";
	long long unsigned int PrefabManager::CountID = 0;

	PrefabManager::PrefabManager()
	{
		std::filesystem::create_directories(PrefabPath);
	}

	void PrefabManager::LoadAllPrefab()
	{
		const char* AssetsPath = "src\\Assets";
		for (auto& entry : std::filesystem::recursive_directory_iterator(AssetsPath))
		{
			auto extension = entry.path().extension().string();
			if (!extension.compare(".prefab"))
			{
				LoadPrefab(entry.path().string().c_str());
			}
		}
	}

	void PrefabManager::LoadPrefab(const char* path)
	{
		Entity ent = MAX_ENTITY;
		long long unsigned int PrefabID = engine->szManager.LoadPrefabFile(ent, path);

		if (PrefabID != 0)
		{
			if (PrefabIDSet.find(PrefabID) == PrefabIDSet.end())
			{
				mapPathToID[path] = PrefabID;
				PrefabIDSet.insert(PrefabID);
				mapPIDToEID[PrefabID] = ent;
			}
			else
			{
				ENGINE_CORE_ERROR(false, "Prefab crashes, same ID.")
			}
		}
		else
		{
			std::string msg = path;
			msg += " is unable to be loaded as prefab.";
			EDITOR_LOG_WARN(msg.c_str());
		}
	}

	std::string PrefabManager::GenerateFileName(EntityComponent& entComp, const char* path)
	{
		std::string& name = entComp.Name;
		std::string prefabName = "\\" + name + ".prefab";
		std::string checkPath = path + prefabName;
		std::string tempName;
		size_t count = 0;
		while (std::filesystem::exists(checkPath))
		{
			tempName = name + lexical_cast<std::string>(++count);
			prefabName = "\\" + tempName + ".prefab";
			checkPath = path + prefabName;
		}

		if (count)
		{
			name = tempName;
		}

		return checkPath;
	}

	void PrefabManager::GeneratePrefab(const Entity& ent, const char* path)
	{
		//Declaration
		World& prefabW = engine->prefabWorld;
		World& w = engine->world;
		std::queue<Entity> copyQueue;
		std::vector<Entity> contents;
		PrefabComponent comp;
		EUUID generatedID = UUIDGenerator::GenerateUUID();
		comp.PrefabID = generatedID;
		if (!w.CheckComponent<PrefabComponent>(ent))
		{
			comp.PrefabID = generatedID;
			w.AddComponent(ent, comp);
		}
		else
		{
			auto& existing = w.GetComponent<PrefabComponent>(ent);
			existing = comp;
		}

		//Push the first entity
		copyQueue.push(ent);

		//Putting child into queue eventually for prefab saving.
		while (!copyQueue.empty())
		{
			Entity entity = copyQueue.front();
			copyQueue.pop();
			/*
			//Uncomment when parent child is up and implement
			
			for (auto& child : children)
			{
				copyQueue.push(child);
			}
			*/
			Entity newEnt = w.CopyEntity(prefabW, entity, all_component_list);
			contents.push_back(newEnt);
		}

		auto& prefabComp = w.GetComponent<PrefabComponent>(ent);
		prefabComp.IsChild = true;

		auto& entComp = prefabW.GetComponent<EntityComponent>(contents[0]);
		entComp.IsActive = false;

		if (prefabW.CheckComponent<MaterialComponent>(contents[0]))
		{
			auto& matComp = prefabW.GetComponent<MaterialComponent>(contents[0]);
			matComp.Highlight = false;
		}

		std::string destPath = GenerateFileName(entComp, path);
		mapPathToID[destPath] =  generatedID;
		PrefabIDSet.insert(generatedID);
		mapPIDToEID[generatedID] = contents[0];

		engine->szManager.SavePrefabFile(prefabComp.PrefabID, contents, destPath.c_str());
	}

	//Create object using prefab, by passing in the prefab data.
	//Currently 
	Entity PrefabManager::CreatePrefabInstance(const char* path)
	{
		if (!std::filesystem::exists(path))
		{
			std::string msg = path;
			msg += " does not exist.";
			EDITOR_LOG_WARN(msg.c_str());
			return MAX_ENTITY;
		}

		World& prefabW = engine->prefabWorld;
		World& w = engine->world;
		std::string source = path;
		Entity ent = mapPIDToEID[mapPathToID[source]];

		//Remember do for parent and child function in future.
		Entity newEnt = prefabW.CopyEntity(w, ent, all_component_list);

		RegisterForNewInstance(newEnt);

		return newEnt;
	}

	void PrefabManager::RegisterForNewInstance(Entity ent)
	{
		World& w = engine->world;
		TransformComponent defaultComp;
		auto& transformComp = w.GetComponent<TransformComponent>(ent);
		transformComp.position = defaultComp.position;

		auto& prefabComp = w.GetComponent<PrefabComponent>(ent);
		prefabComp.IsChild = true;


		engine->editorManager->RegisterExistingEntity(ent);

		if (w.CheckComponent<CameraComponent>(ent))
		{
			auto& camera = w.GetComponent<CameraComponent>(ent);
			engine->gCamera.ReInitCameraList(camera.camType, ent);
		}

		if (w.CheckComponent<AABBComponent>(ent))
		{
			auto& aabb = w.GetComponent<AABBComponent>(ent);
			engine->gCullingManager->Insert(aabb, ent);
			engine->gPicker.UpdateAabb(ent);
			engine->gDynamicAABBTree.InsertData(ent);
		}
	}

	void PrefabManager::Equalize(World& sourceWorld, World& targetWorld, Entity sourceEnt, Entity targetEnt)
	{
		auto& sourceTrans = sourceWorld.GetComponent<TransformComponent>(sourceEnt);
		auto& targetTrans = targetWorld.GetComponent<TransformComponent>(targetEnt);

		targetTrans.rotation = sourceTrans.rotation;
		targetTrans.scale = sourceTrans.scale;

		EqualizeEntity(sourceWorld, targetWorld, sourceEnt, targetEnt, list);
	}

	void PrefabManager::ApplyChangesToAll(Entity ent)
	{
		World& prefabW = engine->prefabWorld;
		World& w = engine->world;

		std::vector<Entity> changingEntities;

		auto& samplePrefabComp = w.GetComponent<PrefabComponent>(ent);
		if (PrefabIDSet.find(samplePrefabComp.PrefabID) != PrefabIDSet.end())
		{
			Entity prefabOwner = mapPIDToEID[samplePrefabComp.PrefabID];

			Equalize(w, prefabW, ent, prefabOwner);

			for (auto entity : w.GetSystem<PrefabSystem>()->mEntities)
			{
				auto& prefabComp = w.GetComponent<PrefabComponent>(entity);
				if (samplePrefabComp.PrefabID == prefabComp.PrefabID)
				{
					changingEntities.push_back(entity);
				}
			}
		}

		for (auto entity : changingEntities)
		{
			if (ent == entity)
			{
				continue;
			}

			Equalize(w, w, ent, entity);

			auto& entComp = w.GetComponent<EntityComponent>(entity);
			entComp.IsActive = false;

			if (w.CheckComponent<MaterialComponent>(entity))
			{
				auto& matComp = w.GetComponent<MaterialComponent>(entity);
				matComp.Highlight = false;
			}

			engine->gPicker.UpdateAabb(ent);
			engine->gDynamicAABBTree.UpdateData(ent);
		}
	}

	void PrefabManager::OverwritePrefab(const Entity& ent, const char* path)
	{
		World& prefabW = engine->prefabWorld;
		std::queue<Entity> copyQueue;
		std::vector<Entity> contents;

		//Push the first entity
		copyQueue.push(ent);

		//Putting child into queue eventually for prefab saving.
		while (!copyQueue.empty())
		{
			Entity entity = copyQueue.front();
			copyQueue.pop();
			/*
			//Uncomment when parent child is up and implement

			for (auto& child : children)
			{
				copyQueue.push(child);
			}
			*/
			contents.push_back(entity);
		}

		auto& prefabComp = prefabW.GetComponent<PrefabComponent>(ent);

		engine->szManager.SavePrefabFile(prefabComp.PrefabID, contents, path);
	}

	std::string PrefabManager::GetPath(long long unsigned int id)
	{
		std::string path;
		for (auto it = mapPathToID.begin(); it != mapPathToID.end(); ++it)
		{
			if (it->second == id)
			{
				path = it->first;
				break;
			}
		}
		return path;
	}

	void PrefabManager::UnloadSaving()
	{
		auto& entities = engine->prefabWorld.GetSystem<PrefabSystem>()->mEntities;
		for(auto ent : entities)
		{
			auto& prefabComp = engine->prefabWorld.GetComponent<PrefabComponent>(ent);
			if (prefabComp.IsChild)
			{
				continue;
			}

			std::string path = GetPath(prefabComp.PrefabID);
			if(std::filesystem::exists(path))
			{
				OverwritePrefab(ent, path.c_str());
			}
		}
		engine->szManager.SavePrefabWorldFile(entities);
	}
}