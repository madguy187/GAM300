#include "pch.h"
#include "ParentSystem.h"
#include "Global.h"

namespace Eclipse
{
	void ParentSystem::Update()
	{
		for (auto& entity : mEntities)
		{
			ParentComponent& parentComp = engine->world.GetComponent<ParentComponent>(entity);

			//for (auto& childEntity : parentComp.child)
		}
	}
}