#pragma once
#include "Global.h"

namespace Eclipse
{
	class PrefabManager
	{
		std::set<EUUID> PrefabIDSet;
		std::unordered_map<EUUID, Entity> mapPIDToEID;
		std::unordered_map<std::string, EUUID> mapPathToID;

		using PrefabUseList = ComponentTypeList<
			CameraComponent,
			DirectionalLightComponent,
			LightComponent,
			MaterialComponent,
			MeshComponent,
			ModelComponent,
			PointLightComponent,
			RigidBodyComponent,
			ScriptComponent,
			//ParentComponent,
			//ChildComponent,
			CollisionComponent,
			AudioComponent,
			AIComponent,
			SpotLightComponent
		>;

		PrefabUseList list{};

		void LoadPrefab(const char* path);

		std::string GenerateFileName(EntityComponent& entComp, const char* path);

		void RegisterForNewInstance(Entity ent);

		void SignatureBaseCopy(World& sourceWorld, World& targetWorld, Entity sourceEnt, Entity targetEnt);

		void CopyToInstance(Entity comparingPrefabEnt, World& copySourceWorld, Entity copyingSourceEnt, Entity instancesEnt);

		std::vector<Entity> GetInstanceList(const EUUID& prefabID);

		template <typename ...T>
		void SignatureBaseCopying(World& sourceWorld, World& targetWorld, Entity sourceEnt, Entity targetEnt, TypeList<T...>)
		{
			((SignatureBaseCopyingComponent<T>(sourceWorld, targetWorld, sourceEnt, targetEnt)), ...);
		}

		template <typename T>
		void SignatureBaseCopyingComponent(World& sourceWorld, World& targetWorld, Entity sourceEnt, Entity targetEnt)
		{
			auto& sourcePrefabComp = sourceWorld.GetComponent<PrefabComponent>(sourceEnt);
			auto& targetPrefabComp = targetWorld.GetComponent<PrefabComponent>(targetEnt);

			const auto& changesSource = sourcePrefabComp.CompChanges;
			const auto& changesTarget = targetPrefabComp.CompChanges;

			if (changesSource.test(sourceWorld.GetComponentType<T>()) || changesTarget.test(targetWorld.GetComponentType<T>()))
			{
				return;
			}

			if (sourceWorld.CheckComponent<T>(sourceEnt) && targetWorld.CheckComponent<T>(targetEnt))
			{
				T& sourceComp = sourceWorld.GetComponent<T>(sourceEnt);
				T& targetComp = targetWorld.GetComponent<T>(targetEnt);

				targetComp = sourceComp;
			}
			else if(sourceWorld.CheckComponent<T>(sourceEnt))
			{
				T& sourceComp = sourceWorld.GetComponent<T>(sourceEnt);

				targetWorld.AddComponent<T>(targetEnt, sourceComp);
			}
			else if(targetWorld.CheckComponent<T>(targetEnt))
			{
				targetWorld.DestroyComponent<T>(targetEnt);
			}
			else
			{

			}
		}
		
		template <typename ...T>
		void CopyToPrefabInstances(Entity comparingPrefabEnt, World& copySourceWorld, Entity copyingSourceEnt, Entity instancesEnt, TypeList<T...>)
		{
			((CopyToInstancesComponent<T>(comparingPrefabEnt, copySourceWorld, copyingSourceEnt, instancesEnt)), ...);
		}

		template <typename T>
		void CopyToInstancesComponent(Entity comparingPrefabEnt, World& copySourceWorld, Entity copyingSourceEnt, Entity instancesEnt)
		{
			World& prefabW = engine->prefabWorld;
			World& entW = engine->world;

			if (prefabW.CheckComponent<T>(comparingPrefabEnt) && entW.CheckComponent<T>(instancesEnt))
			{
				T& comparingComp = prefabW.GetComponent<T>(comparingPrefabEnt);
				T& targetComp = entW.GetComponent<T>(instancesEnt);

				if (SerializationManager::CompareComponentData(targetComp, comparingComp))
				{
					if (copySourceWorld.CheckComponent<T>(copyingSourceEnt))
					{
						T& copyingComp = copySourceWorld.GetComponent<T>(copyingSourceEnt);
						targetComp = copyingComp;
					}
					else
					{
						entW.DestroyComponent<T>(instancesEnt);
					}

					UpdatePrefabSignature(entW, instancesEnt, entW.GetComponentType<T>(), 0);
				}
				else
				{
					UpdatePrefabSignature(entW, instancesEnt, entW.GetComponentType<T>(), 1);
				}
			}
			else if(prefabW.CheckComponent<T>(comparingPrefabEnt) || entW.CheckComponent<T>(instancesEnt))
			{
				UpdatePrefabSignature(entW, instancesEnt, entW.GetComponentType<T>(), 1);
			}
			else
			{
				if (copySourceWorld.CheckComponent<T>(copyingSourceEnt))
				{
					T& copyingComp = copySourceWorld.GetComponent<T>(copyingSourceEnt);
					entW.AddComponent<T>(instancesEnt, copyingComp);
				}
				UpdatePrefabSignature(entW, instancesEnt, entW.GetComponentType<T>(), 0);
			}
		}

		std::string GetPath(const EUUID& id);

		void OverwritePrefab(const Entity& ent, const char* path);

		bool CheckPrefabExistence(const EUUID& prefabID);

		void InsertPrefab(const Entity& ent, const char* path, const EUUID& prefabID);

		//For mesh(prefab) editor save button
		void SavePrefabChanges(const Entity& ent);

		//Perform updates of prefab to instances right after loading of scene
		//void PostUpdate_Prefab();

		void UpdatePrefabSignature(World& sourceW, const Entity& ent, const size_t& setBit, bool setTo);

	public:
		static const std::string PrefabPath;

		PrefabManager();

		void LoadAllPrefab();

		void ApplyChangesToAll(Entity ent);

		void GeneratePrefab(const Entity& ent, const char* path);

		Entity CreatePrefabInstance(const char* path);

		void UnloadSaving();
	};
}