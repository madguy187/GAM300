#include "pch.h"
#include "CollisionSystem.h"
namespace Eclipse
{
	void CollisionSystem::Update()
	{
		ZoneScopedN("Collision System")
		engine->Timer.SetName({ SystemName::COLLISION });
		engine->Timer.tracker.system_start = static_cast<float>(glfwGetTime());
		for (auto const& entity : mEntities)
		{
			engine->gPhysics.UpdateVariables(entity);

			if(engine->IsScenePlaying())
			{ 
				engine->gPhysics.InitActor(entity);
				engine->gPhysics.CreateShape(entity);
				engine->gPhysics.UpdateShapes(entity);
				engine->gPhysics.ChangeType(entity);
				if (!engine->world.CheckComponent<RigidBodyComponent>(entity))
				{
					engine->gPhysics.UpdateActor(entity);
				}
			}
		}
		engine->Timer.tracker.system_end = static_cast<float>(glfwGetTime());
		engine->Timer.UpdateTimeContainer(engine->Timer.tracker);
		FrameMark
	}
}