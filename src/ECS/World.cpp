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

	template<typename ...Types>
	void World::RegisterComponent()
	{
		componentManager->RegisterComponent<Types>();
	}

	template <typename T>
	void World::AddComponent(Entity entity, T component)
	{
		entityManager->AddComponent(entity, componentManager->GetComponentType<T>())
		componentManager->AddComponent<T>(entity, component);
	}

	template <typename T>
	void World::DestroyComponent(Entity entity)
	{
		componentManager->RemoveComponent<T>(entity);
	}

	void World::Clear()
	{

	}
}