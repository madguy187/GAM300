#pragma once
#include "Global.h"

namespace Eclipse
{
	class Coordinator
	{
	public:
		Entity CreateEntity();
		void DestroyEntity(Entity entity);
		
		template <typename T>
		void AddComponent(Entity entity, T component);

		template <typename T>
		void DestroyComponent(Entity entity);

		void Clear();
	};
}