#include "pch.h"
#include "EntityCompSystem.h"

namespace Eclipse
{
	void EntityCompSystem::Update()
	{
	}

	Entity EntityCompSystem::FindEntity(const std::string& entName)
	{
		for (auto& entity : mEntities)
		{
			EntityComponent& entComp = engine->world.GetComponent<EntityComponent>(entity);
			if (entComp.Name == entName) return entity;
		}

		return MAX_ENTITY;
	}
}