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
		
	}
	
	void World::Clear()
	{
		entityManager->Clear();
		//componentManager->Clear();
	}
}