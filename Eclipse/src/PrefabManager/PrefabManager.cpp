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
	const std::string PrefabManager::PrefabPath = "src//Assets//Prefabs//";

	PrefabManager::PrefabManager()
		:CountID{ 0 }
	{
		std::filesystem::create_directories(PrefabPath);
	}

	void PrefabManager::LoadAllPrefab()
	{
		for (auto& entry : std::filesystem::recursive_directory_iterator())
		{
			auto& extension = entry.path().extension().string();
			if (!extension.compare(".prefab"))
			{
				//LoadPrefab(entry.path().string().c_str());
			}
		}
	}

	void PrefabManager::LoadPrefab(const char* path)
	{
		int PrefabID = engine->szManager.LoadPrefabFile(path) >= 0;
		if (PrefabID >= 0)
		{
			mapPathToID[path] = PrefabID;
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
		comp.PrefabID = CountID++;
		w.AddComponent(ent, comp);

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
		std::string destPath = GenerateFileName(entComp, path);

		engine->szManager.SavePrefabFile(prefabComp.PrefabID, contents, destPath.c_str());
	}

	//Create object using prefab, by passing in the prefab data.
	//Currently 
	Entity PrefabManager::CreatePrefabInstance(const char* path)
	{
		return 0;
	}

	PrefabManager::~PrefabManager()
	{
		engine->szManager.SavePrefabWorldFile(engine->prefabWorld.GetSystem<PrefabSystem>()->mEntities);
	}
}