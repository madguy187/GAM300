#include "pch.h"
#include "ParentSystem.h"
#include "Global.h"
#include "ECS/ComponentManager/Components/ParentComponent.h"
#include "ECS/ComponentManager/Components/ChildComponent.h"

namespace Eclipse
{
	void ParentSystem::Update()
	{
		for (auto& entity : mEntities)
		{
			ParentComponent& parentComp = engine->world.GetComponent<ParentComponent>(entity);

			//for (auto& childEntity : parentComp.child)
			for (auto& childEntity : parentComp.child)
			{
				UpdateChildPosition(entity, childEntity);
			}
		}
	}

	void ParentSystem::UpdateChildPosition(Entity parentEnt, Entity childEnt)
	{
		ChildComponent& childComp = engine->world.GetComponent<ChildComponent>(childEnt);
		TransformComponent& childTransComp = engine->world.GetComponent<TransformComponent>(childEnt);
		TransformComponent& parentTransComp = engine->world.GetComponent<TransformComponent>(parentEnt);

		childTransComp.position = parentTransComp.position + childComp.offset;
	}
}