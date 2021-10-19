#pragma once
#include "Global.h"

namespace Eclipse
{
	class PrefabManager
	{
		std::set<long long unsigned int> PrefabIDSet;
		std::unordered_map<long long unsigned int, Entity> mapPIDToEID;
		std::unordered_map<std::string, long long unsigned int> mapPathToID;
		static long long unsigned int CountID;

		using PrefabUseList = ComponentTypeList<
			AABBComponent,
			CameraComponent,
			DirectionalLightComponent,
			LightComponent,
			MaterialComponent,
			MeshComponent,
			ModelComponent,
			PointLightComponent,
			RigidBodyComponent,
			ScriptComponent,
			ParentComponent,
			ChildComponent,
			CollisionComponent,
			AIComponent,
			SpotLightComponent
		>;

		PrefabUseList list{};

		void LoadPrefab(const char* path);

		std::string GenerateFileName(EntityComponent& entComp, const char* path);

		void RegisterForNewInstance(Entity ent);

		void Equalize(World& sourceWorld, World& targetWorld, Entity sourceEnt, Entity targetEnt);

		template <typename ...T>
		void EqualizeEntity(World& sourceWorld, World& targetWorld, Entity sourceEnt, Entity targetEnt, TypeList<T...>)
		{
			((EqualizeEntityComponent<T>(sourceWorld, targetWorld, sourceEnt, targetEnt)), ...);
		}

		template <typename T>
		void EqualizeEntityComponent(World& sourceWorld, World& targetWorld, Entity sourceEnt, Entity targetEnt)
		{
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

		std::string GetPath(long long unsigned int id);

		void OverwritePrefab(const Entity& ent, const char* path);

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