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

		long long unsigned int GetUniqueIdentifier(PrefabComponent& prefab);

		void LoadPrefab(const char* path);

		std::string GenerateFileName(EntityComponent& entComp, const char* path);

		void RegisterForNewInstance(Entity ent);

	public:
		static const std::string PrefabPath;

		PrefabManager();

		void LoadAllPrefab();

		void GeneratePrefab(const Entity& ent, const char* path);

		Entity CreatePrefabInstance(const char* path);

		void Test();
	};
}