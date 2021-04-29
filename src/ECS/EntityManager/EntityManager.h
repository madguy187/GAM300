#pragma once
#include "Global.h"

namespace Eclipse
{
	class EntityManager
	{
		std::queue<Entity> entityQueue;
		std::array<Signature, MAX_ENTITY> entitySignature;
		unsigned _size;
	public:
		EntityManager();
		Entity CreateEntity();
		void DestroyEntity(Entity entity);
		void Clear();
	};
}