#pragma once
#include "Global.h"

namespace Eclipse
{
	class PrefabManager
	{
		std::priority_queue<int> freeID;
		std::unordered_map<std::string, Entity> mapPathToID;
		int CountID;

	public:
		static const std::string PrefabPath;

		PrefabManager();

		void LoadPrefab(const char* path);

		void GeneratePrefab(const Entity& ent, const char* path);

		Entity CreatePrefabInstance(const char* path);

		~PrefabManager();
	};
}