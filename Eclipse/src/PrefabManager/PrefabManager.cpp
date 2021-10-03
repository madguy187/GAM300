#include "pch.h"
#include "PrefabManager.h"
#include "../ECS/ComponentManager/Components/PrefabComponent.h"
#include "../ECS/ComponentManager/Components/ParentComponent.h"
#include "../ECS/ComponentManager/Components/ChildComponent.h"
#include "../ECS/ComponentManager/Components/ScriptComponent.h"

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
				LoadPrefab(entry.path().string().c_str());
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

		engine->szManager.SavePrefabFile(prefabComp.PrefabID, contents, path);
	}

	//Create object using prefab, by passing in the prefab data.
	//Currently 
	Entity PrefabManager::CreatePrefabInstance(const char* path)
	{
		return 0;
	}

	PrefabManager::~PrefabManager()
	{

	}
}