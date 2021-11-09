#include "pch.h"
#include "EntityManager.h"

namespace Eclipse
{
	EntityManager::EntityManager() :
		_size{0}
	{
		for (Entity i = MAX_ENTITY - 1; i-- > 0; )
		{
			entityQueue.push(i);
		}
	}

	Entity EntityManager::CreateEntity()
	{
		Entity entity = entityQueue.top();
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
		std::stack<Entity> queue;

		for (Entity i = MAX_ENTITY - 1; i-- > 0; )
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

	Signature EntityManager::GetSignature(Entity entity)
	{
		return entitySignature[entity];
	}
}