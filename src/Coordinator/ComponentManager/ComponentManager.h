#pragma once
#include "Global.h"

namespace Eclipse
{
	class ComponentManager
	{
		unsigned ID = 0;
	public:
		template <typename T>
		void AddComponent(Entity entity, T component);

		template <typename T>
		void DestroyComponent(Entity entity);
	};
}