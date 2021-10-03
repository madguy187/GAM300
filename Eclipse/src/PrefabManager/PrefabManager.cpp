#include "pch.h"
#include "PrefabManager.h"
#include "../ECS/ComponentManager/Components/PrefabComponent.h"
#include "../ECS/ComponentManager/Components/ParentComponent.h"
#include "../ECS/ComponentManager/Components/ChildComponent.h"
#include "../ECS/ComponentManager/Components/ScriptComponent.h"

namespace Eclipse
{
	PrefabManager::PrefabManager()
		:CountID{ 0 }
	{
		std::filesystem::create_directories(AssetPath);
	}

	void PrefabManager::LoadPrefab()
	{
		
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
	void PrefabManager::CreatePrefabInstance(const Entity& ent)
	{

	}
}