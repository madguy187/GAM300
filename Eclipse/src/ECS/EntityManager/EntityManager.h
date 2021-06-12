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
		void Destroy(Entity entity);
		void Clear();
		void AddComponent(Entity entity, unsigned index);
		void RemoveComponent(Entity entity, unsigned index);
		bool CheckComponent(Entity entity, Signature signature);
		Signature GetSignature(Entity entity);
	};
}