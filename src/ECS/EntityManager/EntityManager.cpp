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
		_size++;
		return entity;
	}

	void EntityManager::Destroy(Entity entity)
	{
		entityQueue.push(entity);
		entitySignature[entity].reset();
		_size--;
	}

	void EntityManager::Clear()
	{
		std::queue<Entity> queue;

		for (Entity i = 0; i < MAX_ENTITY; i++)
		{
			queue.push(i);
			entitySignature[i].reset();
		}

		_size = 0;
		entityQueue = queue;
	}

	void EntityManager::AddComponent(Entity entity, unsigned index)
	{
		entitySignature[entity].set(index, true);
	}

	void EntityManager::RemoveComponent(Entity entity, unsigned index)
	{
		entitySignature[entity].set(index, false);
	}
}