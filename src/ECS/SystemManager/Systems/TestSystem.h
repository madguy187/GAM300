#pragma once

#include "pch.h"
#include "System/System.h"
#include "ECS/ComponentManager/Components/TransformComponent.h"

namespace Eclipse
{
	class TestSystem : public System
	{
	public:
		void Update()
		{
			for (auto const& entity : mEntities)
			{
				TransformComponent& trans = engine->world.GetComponent<TransformComponent>(entity);
				trans.x++;
			}
		}
	};
}