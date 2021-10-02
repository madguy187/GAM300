#pragma once
#include "Global.h"

namespace Eclipse
{
	class PrefabManager
	{
		const std::string AssetPath = "src//Assets//Prefabs";
		std::priority_queue<unsigned int> freeID;
		unsigned int CountID;

	public:
		PrefabManager();

		void LoadPrefab();

		void GeneratePrefab(const Entity& ent, const char* path);

		void CreatePrefabInstance(const Entity& ent);
	};
}