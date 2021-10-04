#pragma once
#include "Global.h"

namespace Eclipse
{
	class PrefabManager
	{
		std::priority_queue<int> freeID;
		std::unordered_map<std::string, Entity> mapPathToID;
		int CountID;

		void LoadPrefab(const char* path);

		std::string GenerateFileName(EntityComponent& entComp, const char* path);

	public:
		static const std::string PrefabPath;

		PrefabManager();

		void LoadAllPrefab();

		void GeneratePrefab(const Entity& ent, const char* path);

		Entity CreatePrefabInstance(const char* path);

		void Test();
	};
}