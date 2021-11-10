#include "pch.h"
#include "PrefabManager.h"
#include "ECS/ComponentManager/ComponentList.h"
#include "ECS/SystemManager/Systems/System/PrefabSystem/PrefabSystem.h"

namespace Eclipse
{
	const std::string PrefabManager::PrefabPath = "src\\Assets\\Prefabs\\";

	bool PrefabManager::CheckPrefabExistence(const EUUID& prefabID)
	{
		return !(PrefabIDSet.find(prefabID) == PrefabIDSet.end());
	}

	void PrefabManager::InsertPrefab(const Entity& ent, const char* path, const EUUID& prefabID)
	{
		if (strcmp(path, ""))
		{
			mapPathToID[path] = prefabID;
		}

		PrefabIDSet.insert(prefabID);
		mapPIDToEID[prefabID] = ent;
	}

	void PrefabManager::RecurseInsertPrefab(const Entity& ent, const char* path, const EUUID& prefabID)
	{
		auto& prefabW = engine->prefabWorld;
		InsertPrefab(ent, path, prefabID);

		if (prefabW.CheckComponent<ParentComponent>(ent))
		{
			auto& parentComp = prefabW.GetComponent<ParentComponent>(ent);

			for (auto& child : parentComp.child)
			{
				auto& prefabComp = prefabW.GetComponent<PrefabComponent>(child);
				RecurseInsertPrefab(child, "", prefabComp.PrefabID);
			}
		}
	}

	//void PrefabManager::SavePrefabChanges(const Entity& updatedPrefabEnt)
	//{
	//	auto& prefabW = engine->prefabWorld;
	//	auto& prefabComp = prefabW.GetComponent<PrefabComponent>(updatedPrefabEnt);
	//	const std::string& path = GetPath(prefabComp.PrefabID);

	//	//Load old prefab
	//	Entity oldPrefabEnt;
	//	engine->szManager.LoadPrefabFile(oldPrefabEnt, path.c_str());

	//	std::vector<Entity> changingEntities;

	//	//Update instances
	//	for (auto entity : changingEntities)
	//	{
	//		CopyToInstance(oldPrefabEnt, prefabW, updatedPrefabEnt, entity);

	//		CleanUpForInstancesAfterCopy(entity);
	//	}

	//	//Update prefab
	//	std::string prefabPath = GetPath(prefabComp.PrefabID);

	//	if (std::filesystem::exists(prefabPath))
	//	{
	//		OverwritePrefab(updatedPrefabEnt, prefabPath.c_str());
	//	}

	//	//Destroy
	//	prefabW.DestroyEntity(oldPrefabEnt);
	//}

	void PrefabManager::UpdatePrefabSignature(World& sourceW, const Entity& ent, const size_t& setBit, bool setTo)
	{
		auto& prefabComp = sourceW.GetComponent<PrefabComponent>(ent);

		prefabComp.CompChanges.set(setBit, setTo);
	}

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

	void PrefabManager::PostUpdate()
	{
		auto& w = engine->world;
		auto& prefabW = engine->prefabWorld;
		std::vector<Entity> instances = GetInstanceList();

		for (auto& ent : instances)
		{
			auto& prefab = w.GetComponent<PrefabComponent>(ent);
			if (prefab.IsChild)
			{
				continue;
			}
			auto& prefabOwner = mapPIDToEID[prefab.PrefabID];

			CompareMap_Hierarchy hierMap;
			CompareMap_NewChild newChildMap;
			CompareHierarchy(prefabW, prefabOwner, w, ent, hierMap, newChildMap);

			for (auto& pair : hierMap)
			{
				if (pair.second == MAX_ENTITY)
				{
					continue;
				}

				auto& target = pair.first;
				auto& copySource = pair.second;

				SignatureBaseCopy(prefabW, w, copySource, target);
				CleanUpForInstancesAfterCopy(target);
			}

			RecursiveUpdateParentChildTransform(ent);

			for (auto& pair : newChildMap)
			{
				for (auto& newChild : pair.second)
				{
					RecursiveCreatePrefabInstances(newChild, pair.first);
				}
			}
		}
	}

	void PrefabManager::EndUpdate()
	{
		auto& w = engine->world;
		auto& prefabW = engine->prefabWorld;
		std::vector<Entity> instances = GetInstanceList();

		for (auto& ent : instances)
		{
			auto& prefab = w.GetComponent<PrefabComponent>(ent);
			if (prefab.IsChild)
			{
				continue;
			}
			auto& prefabOwner = mapPIDToEID[prefab.PrefabID];

			CompareMap_Hierarchy hierMap;
			CompareMap_NewChild newChildMap;
			CompareHierarchy(prefabW, prefabOwner, w, ent, hierMap, newChildMap);

			for (auto& pair : hierMap)
			{
				if (pair.second == MAX_ENTITY)
				{
					continue;
				}

				auto& target = pair.first;
				auto& copySource = pair.second;

				CopyToInstance(copySource, prefabW, copySource, target, true);
				//SignatureBaseCopy(prefabW, w, prefabOwner, ent);
				CleanUpForInstancesAfterCopy(target);
			}
			RecursiveUpdateParentChildTransform(ent);
			for (auto& pair : newChildMap)
			{
				for (auto& newChild : pair.second)
				{
					RecursiveCreatePrefabInstances(newChild, pair.first);
				}
			}
		}
	}

	void PrefabManager::CleanUpBeforeDestroyChild(World& w, const Entity& ent)
	{
		if (w.CheckComponent<ChildComponent>(ent))
		{
			auto& child = w.GetComponent<ChildComponent>(ent);
			if (w.CheckComponent<ParentComponent>(child.parentIndex))
			{
				auto& parent = w.GetComponent<ParentComponent>(child.parentIndex);

				parent.child.erase(std::find(parent.child.begin(), parent.child.end(), ent));

				if (!parent.child.empty())
				{
					w.DestroyComponent<ParentComponent>(child.parentIndex);
				}
			}
		}
	}

	void PrefabManager::LoadPrefab(const char* path)
	{
		Entity ent = MAX_ENTITY;
		EUUID PrefabID = engine->szManager.LoadPrefabFile(ent, path);

		if (PrefabID != 0)
		{
			if (!CheckPrefabExistence(PrefabID))
			{
				RecurseInsertPrefab(ent, path, PrefabID);
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
	void PrefabManager::UpdateNewGeneratedPrefab(const Entity& prefabEnt, const Entity& parentEnt)
	{
		World& prefabW = engine->prefabWorld;

		//Entity component updates
		auto& entComp = prefabW.GetComponent<EntityComponent>(prefabEnt);
		entComp.IsActive = false;
		entComp.hightLightChild = false;

		//Prefab component updates
		auto& prefabComp = prefabW.GetComponent<PrefabComponent>(prefabEnt);
		prefabComp.IsInstance = false;
		
		//ClearHighlight
		if (prefabW.CheckComponent<MaterialComponent>(prefabEnt))
		{
			auto& matComp = prefabW.GetComponent<MaterialComponent>(prefabEnt);
			matComp.Highlight = false;
		}

		if (prefabW.CheckComponent<ParentComponent>(prefabEnt))
		{
			auto& parentComp = prefabW.GetComponent<ParentComponent>(prefabEnt);
			parentComp.child.clear();
		}

		if (prefabW.CheckComponent<ChildComponent>(prefabEnt))
		{
			if (parentEnt == MAX_ENTITY)
			{
				prefabW.DestroyComponent<ChildComponent>(prefabEnt);
			}
			else
			{
				auto& childComp = prefabW.GetComponent<ChildComponent>(prefabEnt);
				childComp.parentIndex = parentEnt;

				auto& parent = prefabW.GetComponent<ParentComponent>(parentEnt);
				parent.child.push_back(prefabEnt);
			}
		}

		if (prefabW.CheckComponent<AIComponent>(prefabEnt))
		{
			auto& parentComp = prefabW.GetComponent<AIComponent>(prefabEnt);
			parentComp.waypoints.clear();
		}
	}

	Entity PrefabManager::RecursiveGeneratePrefab(const Entity& sourceEnt, const char* path, EUUID generatedID, const Entity& parentEnt)
	{
		//Declaration
		World& prefabW = engine->prefabWorld;
		World& w = engine->world;

		//Init Prefab Component
		PrefabComponent comp;
		comp.PrefabID = generatedID;
		comp.IsInstance = true;

		if (parentEnt != MAX_ENTITY)
		{
			comp.IsChild = true;
		}

		if (!w.CheckComponent<PrefabComponent>(sourceEnt))
		{
			comp.PrefabID = generatedID;
			w.AddComponent(sourceEnt, comp);
		}
		else
		{
			auto& existing = w.GetComponent<PrefabComponent>(sourceEnt);
			existing = comp;
		}

		//Copy from main world to prefab world
		Entity newEnt = w.CopyEntity(prefabW, sourceEnt, all_component_list);

		//Update prefab to ideal condition.
		UpdateNewGeneratedPrefab(newEnt, parentEnt);

		InsertPrefab(newEnt, path, generatedID);

		if (w.CheckComponent<ParentComponent>(sourceEnt))
		{
			auto& parentComp = w.GetComponent<ParentComponent>(sourceEnt);

			//Generate child prefab
			for (auto& child : parentComp.child)
			{
				RecursiveGeneratePrefab(child, "", ++generatedID, newEnt);
			}
		}

		return newEnt;
	}

	void PrefabManager::GeneratePrefab(const Entity& ent, const char* path)
	{
		//Declaration
		World& w = engine->world;

		EUUID generatedID = UUIDGenerator::GenerateUUID();

		auto& entComp = w.GetComponent<EntityComponent>(ent);
		//Update map
		std::string destPath;

		destPath = GenerateFileName(entComp, path);

		Entity newEnt = RecursiveGeneratePrefab(ent, destPath.c_str(), generatedID, MAX_ENTITY);

		engine->szManager.SavePrefabFile(generatedID, newEnt, destPath.c_str());
	}

	void PrefabManager::RecursiveCreatePrefabInstances(const Entity& childPrefabEnt, const Entity& parentInstanceEnt)
	{
		World& prefabW = engine->prefabWorld;
		World& w = engine->world;

		//Remember do for parent and child function in future.
		Entity newEnt = prefabW.CopyEntity(w, childPrefabEnt, all_component_list);

		RegisterForNewInstance(newEnt, parentInstanceEnt);

		if (prefabW.CheckComponent<ParentComponent>(childPrefabEnt))
		{
			auto& parentComp = prefabW.GetComponent<ParentComponent>(childPrefabEnt);

			for (auto& child : parentComp.child)
			{
				RecursiveCreatePrefabInstances(child, newEnt);
			}
		}
	}

	void PrefabManager::CompareHierarchy(World& compareSampleWorld, const Entity& compareSample, World& compareTargetWorld,
		const Entity& compareTarget, CompareMap_Hierarchy& hierMap, CompareMap_NewChild& newChildMap)
	{
		//First, put the base into the map
		hierMap[compareTarget] = compareSample;
		
		if (compareSampleWorld.CheckComponent<ParentComponent>(compareSample))
		{
			auto& sampleParentComp = compareSampleWorld.GetComponent<ParentComponent>(compareSample);
			std::vector<Entity> newChildren;
			std::vector<Entity> targetChildren;

			for (auto& sampleChild : sampleParentComp.child)
			{
				if (compareTargetWorld.CheckComponent<ParentComponent>(compareTarget))
				{
					bool isFound = false;

					if (compareSampleWorld.CheckComponent<PrefabComponent>(sampleChild))
					{
						auto& sampleChildPrefabID = compareSampleWorld.GetComponent<PrefabComponent>(sampleChild).PrefabID;

						auto& targetParentComp = compareTargetWorld.GetComponent<ParentComponent>(compareTarget);
						targetChildren = targetParentComp.child;

						for (auto& targetChild : targetChildren)
						{
							if (compareTargetWorld.CheckComponent<PrefabComponent>(targetChild))
							{
								auto& targetChildPrefabID = compareTargetWorld.GetComponent<PrefabComponent>(targetChild).PrefabID;

								if (sampleChildPrefabID == targetChildPrefabID)
								{
									CompareHierarchy(compareSampleWorld, sampleChild, compareTargetWorld, targetChild, hierMap, newChildMap);
									isFound = true;
									targetChildren.erase(std::remove(targetChildren.begin(), targetChildren.end(), targetChild));
									break;
								}
							}
						}
					}

					if (!isFound)
					{
						newChildren.push_back(sampleChild);
					}
				}
				else
				{
					newChildren.push_back(sampleChild);
				}
			}

			if (newChildren.size())
			{
				newChildMap[compareTarget] = newChildren;
			}

			for (auto& child : targetChildren)
			{
				hierMap[child] = MAX_ENTITY;
			}
		}
		else
		{
			if (compareTargetWorld.CheckComponent<ParentComponent>(compareTarget))
			{
				auto& targetParentComp = compareTargetWorld.GetComponent<ParentComponent>(compareTarget);

				for (auto& child : targetParentComp.child)
				{
					hierMap[child] = MAX_ENTITY;
				}

			}
		}

	}

	void PrefabManager::print(CompareMap_Hierarchy& hierMap, CompareMap_NewChild& newChildMap)
	{
		std::cout << "Hierarchy Map" << std::endl;
		std::cout << "OldPrefab\tNewPrefab" << std::endl;
		for (auto& pair : hierMap)
		{
			std::cout << pair.first << "\t" << pair.second << std::endl;
		}
		std::cout << "End" << std::endl;
		std::cout << std::endl;

		std::cout << "New Children Map" << std::endl;
		std::cout << "OldPrefab_Children" << std::endl;
		for (auto& pair : newChildMap)
		{
			std::cout << pair.first << ": ";
			for (auto& ent : pair.second)
			{
				std::cout << ent << ", ";
			}
			std::cout << std::endl;
		}
		std::cout << "End" << std::endl;
		std::cout << std::endl;
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

		RegisterForNewInstance(newEnt, MAX_ENTITY);

		if (prefabW.CheckComponent<ParentComponent>(ent))
		{
			auto& parentComp = prefabW.GetComponent<ParentComponent>(ent);

			for (auto& child : parentComp.child)
			{
				RecursiveCreatePrefabInstances(child, newEnt);
			}
		}

		return newEnt;
	}

	Entity PrefabManager::CreatePrefabInstanceSetTransform(const EUUID& prefabID, const ECVec3& position, const ECVec3& rotation)
	{
		std::string path =  GetPath(prefabID);
		
		if (!path.empty())
		{
			return MAX_ENTITY;
		}

		Entity newEnt = CreatePrefabInstance(path.c_str());

		if(newEnt != MAX_ENTITY)
		{
			auto& transform = engine->world.GetComponent<TransformComponent>(newEnt);

			transform.position = position;
			transform.rotation = rotation;
		}

		return newEnt;
	}

	Entity PrefabManager::CreatePrefabInstanceSetTransform(const char* path, const ECVec3& position, const ECVec3& rotation)
	{
		Entity newEnt = CreatePrefabInstance(path);

		if (newEnt != MAX_ENTITY)
		{
			auto& transform = engine->world.GetComponent<TransformComponent>(newEnt);

			transform.position = position;
			transform.rotation = rotation;
		}

		return newEnt;
	}

	void PrefabManager::UpdateParentChildTransform(const Entity& childEnt, const Entity& parentEnt)
	{
		ChildComponent& childComp = engine->world.GetComponent<ChildComponent>(childEnt);
		TransformComponent& childTransComp = engine->world.GetComponent<TransformComponent>(childEnt);
		TransformComponent& parentTransComp = engine->world.GetComponent<TransformComponent>(parentEnt);

		glm::mat4 T = glm::mat4(1.0f);
		glm::mat4 R = glm::mat4(1.0f);
		glm::mat4 identityMatrix = glm::mat4(1.0f);
		T = glm::translate(T, parentTransComp.position.ConvertToGlmVec3Type());
		R = glm::rotate(R, glm::radians(parentTransComp.rotation.getX()), glm::vec3(1.0f, 0.0f, 0.0f));
		R = glm::rotate(R, glm::radians(parentTransComp.rotation.getY()), glm::vec3(0.0f, 1.0f, 0.0f));
		R = glm::rotate(R, glm::radians(parentTransComp.rotation.getZ()), glm::vec3(0.0f, 0.0f, 1.0f));

		glm::mat4 model = T * R;
		ParentComponent& parentComp = engine->world.GetComponent<ParentComponent>(parentEnt);
		parentComp.model = model;

		model = glm::translate(model, childComp.PosOffset.ConvertToGlmVec3Type());
		glm::vec4 temp = glm::vec4{ 0, 0, 0, 1 };
		glm::vec3 newPos = model * temp;
		childTransComp.position = newPos;

		childTransComp.rotation = parentTransComp.rotation + childComp.RotOffset;
		childTransComp.scale.setX(parentTransComp.scale.getX() * childComp.ScaleOffset.getX());
		childTransComp.scale.setY(parentTransComp.scale.getY() * childComp.ScaleOffset.getY());
		childTransComp.scale.setZ(parentTransComp.scale.getZ() * childComp.ScaleOffset.getZ());
	}

	void PrefabManager::RecursiveUpdateParentChildTransform(const Entity& ent)
	{
		if (engine->world.CheckComponent<ParentComponent>(ent))
		{
			auto& parent = engine->world.GetComponent<ParentComponent>(ent);

			for (auto& child : parent.child)
			{
				UpdateParentChildTransform(child, ent);
				RecursiveUpdateParentChildTransform(child);
			}
		}
	}

	void PrefabManager::RegisterForNewInstance(const Entity& ent, const Entity& parentEnt)
	{
		World& w = engine->world;
		//EntityComponent
		auto& entComp = w.GetComponent<EntityComponent>(ent);

		TransformComponent defaultComp;
		auto& transformComp = w.GetComponent<TransformComponent>(ent);
		//Transform Component Update
		if (parentEnt == MAX_ENTITY)
		{
			transformComp.position = defaultComp.position;
		}
		else
		{
			if (!w.CheckComponent<ParentComponent>(parentEnt))
			{
				w.AddComponent<ParentComponent>(parentEnt, {});
			}

			UpdateParentChildTransform(ent, parentEnt);
		}


		//PrefabComponent update
		auto& prefabComp = w.GetComponent<PrefabComponent>(ent);
		prefabComp.IsInstance = true;

		engine->editorManager->RegisterExistingEntity(ent);

		//CameraComponent update
		if (w.CheckComponent<CameraComponent>(ent))
		{
			auto& camera = w.GetComponent<CameraComponent>(ent);
			engine->gCamera.ReInitCameraList(camera.camType, ent);
		}

		//AABBComponent update
		if (w.CheckComponent<AABBComponent>(ent))
		{
			engine->gPicker.GenerateAabb(ent, transformComp, entComp.Tag);
		}

		if (w.CheckComponent<ParentComponent>(ent))
		{
			auto& parentComp = w.GetComponent<ParentComponent>(ent);
			parentComp.child.clear();
		}

		if (w.CheckComponent<ChildComponent>(ent))
		{
			if (parentEnt == MAX_ENTITY)
			{
				w.DestroyComponent<ChildComponent>(ent);
			}
			else
			{
				auto& childComp = w.GetComponent<ChildComponent>(ent);
				childComp.parentIndex = parentEnt;

				auto& parent = w.GetComponent<ParentComponent>(parentEnt);
				parent.child.push_back(ent);
			}
		}
	}

	void PrefabManager::CleanUpForInstancesAfterCopy(const Entity& ent)
	{
		auto& w = engine->world;
		auto& entComp = w.GetComponent<EntityComponent>(ent);
		entComp.IsActive = false;

		if (w.CheckComponent<MaterialComponent>(ent))
		{
			auto& matComp = w.GetComponent<MaterialComponent>(ent);
			matComp.Highlight = false;
		}

		engine->gPicker.UpdateAabb(ent);
		engine->gDynamicAABBTree.UpdateData(ent);
	}

	void PrefabManager::SignatureBaseCopy(World& sourceWorld, World& targetWorld, const Entity& sourceEnt, const Entity& targetEnt)
	{
		SignatureBaseCopying(sourceWorld, targetWorld, sourceEnt, targetEnt, all_component_list);
	}

	void PrefabManager::CopyToInstance(const Entity& comparingPrefabEnt, World& copySourceWorld, const Entity& copyingSourceEnt, const Entity& instancesEnt, const bool& UpdateInstancesOnly)
	{
		CopyToPrefabInstances(comparingPrefabEnt, copySourceWorld, copyingSourceEnt, instancesEnt, all_component_list, UpdateInstancesOnly);
	}

	std::vector<Entity> PrefabManager::GetInstanceList(const EUUID& prefabID)
	{
		std::vector<Entity> changingEntities;
		auto& w = engine->world;
		auto& set = w.GetSystem<PrefabSystem>()->mEntities;

		if (prefabID == 0)
		{
			std::copy(set.begin(), set.end(), std::back_inserter(changingEntities));

			/*std::copy_if(set.begin(), set.end(), std::back_inserter(changingEntities), [&w](const Entity& ent) {
				auto& comp = w.GetComponent<PrefabComponent>(ent);
				return !comp.IsChild;
				});*/
			return changingEntities;
		}

		for (auto entity : w.GetSystem<PrefabSystem>()->mEntities)
		{
			auto& prefabComp = w.GetComponent<PrefabComponent>(entity);
			if (prefabID == prefabComp.PrefabID)
			{
				changingEntities.push_back(entity);
			}
		}
		return changingEntities;
	}

	void PrefabManager::ApplyChangesToAll(Entity ent)
	{
		RecursiveUpdateParentChildTransform(ent);
		World& prefabW = engine->prefabWorld;
		World& w = engine->world;
		Entity prefabOwner = MAX_ENTITY;
		std::vector<Entity> changingEntities;

		auto& samplePrefabComp = w.GetComponent<PrefabComponent>(ent);
		samplePrefabComp.CompChanges &= 0;

		if (PrefabIDSet.find(samplePrefabComp.PrefabID) != PrefabIDSet.end())
		{
			prefabOwner = mapPIDToEID[samplePrefabComp.PrefabID];
			
			changingEntities = GetInstanceList(samplePrefabComp.PrefabID);
		}
		//break straigtaway if no prefab found.
		//which is unlikely to happen in future
		if (prefabOwner == MAX_ENTITY)
		{
			return;
		}

		CompareMap_Hierarchy hierMap;
		CompareMap_NewChild newChildMap;

		CompareHierarchy(w, ent, prefabW, prefabOwner, hierMap, newChildMap);
		//print(hierMap, newChildMap);

		for (auto entity : changingEntities)
		{
			if (ent == entity)
			{
				continue;
			}

			CompareMap_Hierarchy childHierMap;
			CompareMap_NewChild childNewChildMap;
			CompareHierarchy(prefabW, prefabOwner, w, entity, childHierMap, childNewChildMap);
			//print(childHierMap, childNewChildMap);

			for (auto& pair : childHierMap)
			{
				if (pair.second == MAX_ENTITY)
				{
					continue;
				}

				auto& target = pair.first;
				auto& sample = pair.second;
				auto& copySource = hierMap[pair.second];

				if (copySource == MAX_ENTITY)
				{
					CleanUpBeforeDestroyChild(w, target);
					w.DestroyEntity(target);
				}
				else
				{
					CopyToInstance(sample, w, copySource, target);
					CleanUpForInstancesAfterCopy(target);
				}

			}
			RecursiveUpdateParentChildTransform(entity);

			for (auto& pair : childNewChildMap)
			{
				for (auto& newChild : pair.second)
				{
					RecursiveCreatePrefabInstances(newChild, pair.first);
				}
			}
		}

		for (auto& pair : hierMap)
		{
			if (pair.second == MAX_ENTITY)
			{
				CleanUpBeforeDestroyChild(prefabW, pair.first);
				prefabW.DestroyEntity(pair.first);
				continue;
			}
			auto& target = pair.first;
			auto& copySource = pair.second;

			SignatureBaseCopy(w, prefabW, copySource, target);
		}

		EUUID generatedID = UUIDGenerator::GenerateUUID();

		for (auto& pair : newChildMap)
		{
			for (auto& newChild : pair.second)
			{
				RecursiveGeneratePrefab(newChild, "", generatedID, pair.first);
				generatedID += pair.second.size();
			}
		}
	}

	void PrefabManager::OverwritePrefab(const Entity& ent, const char* path, bool IsFromMainWorld)
	{
		World& prefabW = IsFromMainWorld ? engine->world : engine->prefabWorld;

		auto& prefabComp = prefabW.GetComponent<PrefabComponent>(ent);

		engine->szManager.SavePrefabFile(prefabComp.PrefabID, ent, path, IsFromMainWorld);
	}

	std::string PrefabManager::GetPath(const EUUID& id)
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
			if (prefabComp.IsInstance || prefabComp.IsChild)
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