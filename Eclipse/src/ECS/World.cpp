#include "pch.h"
#include "World.h"

namespace Eclipse
{
	World::World()
	{
		componentManager = std::make_unique<ComponentManager>();
		entityManager = std::make_unique<EntityManager>();
		systemManager = std::make_unique<SystemManager>();
	}

	Entity World::CreateEntity()
	{
		return entityManager->CreateEntity();
	}

	void World::DestroyEntity(Entity entity)
	{
		return entityManager->Destroy(entity);
	}

	bool World::CheckComponent(Entity entity, Signature signature)
	{
		return entityManager->CheckComponent(entity, signature);
	}
	
	void World::Clear()
	{
		entityManager->Clear();
		componentManager->Clear();
	}
}