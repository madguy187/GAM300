#include "pch.h"
#include "EntityManager.h"

namespace Eclipse
{
	EntityManager::EntityManager() :
		_size{0}
	{
		for (Entity i = 0; i < MAX_ENTITY; i++)
		{
			entityQueue.push(i);
		}
	}

	Entity EntityManager::CreateEntity()
	{
		Entity entity = entityQueue.front();
		entityQueue.pop();
		return entity;
	}

	void EntityManager::DestroyEntity(Entity entity)
	{
		entityQueue.push(entity);
	}

	void EntityManager::Clear()
	{
		//for (unsigned i = 0; i < )
	}
}