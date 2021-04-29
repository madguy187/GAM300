#include "pch.h"
#include "Coordinator.h"

namespace Eclipse
{
	Entity Coordinator::CreateEntity()
	{
		return 0;
	}

	void Coordinator::DestroyEntity(Entity entity)
	{

	}

	template <typename T>
	void Coordinator::AddComponent(Entity entity, T component)
	{

	}

	template <typename T>
	void Coordinator::DestroyComponent(Entity entity)
	{

	}

	void Coordinator::Clear()
	{

	}
}