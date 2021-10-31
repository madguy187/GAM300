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
			CollisionComponent,
			AudioComponent,
			SpotLightComponent
		>;
		//Cannot compare entity, aabb, parent, child, transform, ai, prefab
		PrefabUseList list{};

		void LoadPrefab(const char* path);

		std::string GenerateFileName(EntityComponent& entComp, const char* path);

		void RegisterForNewInstance(const Entity& ent, const Entity& parentEnt);

		void CleanUpForInstancesAfterCopy(const Entity& ent);

		void SignatureBaseCopy(World& sourceWorld, World& targetWorld, const Entity& sourceEnt, const Entity& targetEnt);

		void CopyToInstance(const Entity& comparingPrefabEnt, World& copySourceWorld, const Entity& copyingSourceEnt, const Entity& instancesEnt);

		std::vector<Entity> GetInstanceList(const EUUID& prefabID = 0);

		template <typename ...T>
		void SignatureBaseCopying(World& sourceWorld, World& targetWorld, const Entity& sourceEnt, const Entity& targetEnt, TypeList<T...>)
		{
			((SignatureBaseCopyingComponent<T>(sourceWorld, targetWorld, sourceEnt, targetEnt)), ...);
		}

		template <typename T>
		void SignatureBaseCopyingComponent(World& sourceWorld, World& targetWorld, const Entity& sourceEnt, const Entity& targetEnt)
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
		void CopyToPrefabInstances(const Entity& comparingPrefabEnt, World& copySourceWorld, const Entity& copyingSourceEnt, const Entity& instancesEnt, TypeList<T...>, const bool& UpdateSignatureOnly = false)
		{
			((CopyToInstancesComponent<T>(comparingPrefabEnt, copySourceWorld, copyingSourceEnt, instancesEnt, UpdateSignatureOnly)), ...);
		}

		template <typename T>
		void CopyToInstancesComponent(const Entity& comparingPrefabEnt, World& copySourceWorld, const Entity& copyingSourceEnt, const Entity& instancesEnt, const bool& UpdateSignatureOnly)
		{
			World& prefabW = engine->prefabWorld;
			World& entW = engine->world;

			if (prefabW.CheckComponent<T>(comparingPrefabEnt) && entW.CheckComponent<T>(instancesEnt))
			{
				T& comparingComp = prefabW.GetComponent<T>(comparingPrefabEnt);
				T& targetComp = entW.GetComponent<T>(instancesEnt);

				if (SerializationManager::CompareComponentData(targetComp, comparingComp))
				{
					if (!UpdateSignatureOnly)
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
				if (!UpdateSignatureOnly)
				{
					if (copySourceWorld.CheckComponent<T>(copyingSourceEnt))
					{
						T& copyingComp = copySourceWorld.GetComponent<T>(copyingSourceEnt);
						entW.AddComponent<T>(instancesEnt, copyingComp);
					}
				}
				UpdatePrefabSignature(entW, instancesEnt, entW.GetComponentType<T>(), 0);
			}
		}

		std::string GetPath(const EUUID& id);

		void OverwritePrefab(const Entity& ent, const char* path);

		bool CheckPrefabExistence(const EUUID& prefabID);

		void InsertPrefab(const Entity& ent, const char* path, const EUUID& prefabID);

		void RecurseInsertPrefab(const Entity& ent, const char* path, const EUUID& prefabID);

		//For mesh(prefab) editor save button
		void SavePrefabChanges(const Entity& ent);

		//Perform updates of prefab to instances right after loading of scene
		//void PostUpdate_Prefab();

		void UpdatePrefabSignature(World& sourceW, const Entity& ent, const size_t& setBit, bool setTo);

		Entity RecursiveGeneratePrefab(const Entity& sourceEnt, const char* path, EUUID generatedID, const Entity& parentEnt);

		void UpdateNewGeneratedPrefab(const Entity& prefabEnt, const Entity& parentEnt);

		void RecursiveCreatePrefabInstances(const Entity& childEnt, const Entity& parentEnt);

	public:
		static const std::string PrefabPath;

		PrefabManager();

		void LoadAllPrefab();

		void PostUpdate();

		void EndUpdate();

		void ApplyChangesToAll(Entity ent);

		void GeneratePrefab(const Entity& ent, const char* path);

		Entity CreatePrefabInstance(const char* path);

		void UnloadSaving();
	};
}