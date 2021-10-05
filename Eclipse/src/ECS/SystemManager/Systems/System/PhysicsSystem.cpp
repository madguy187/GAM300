#include "pch.h"
#include "PhysicsSystem.h"
namespace Eclipse
{
	void PhysicsSystem::Update()
	{

		engine->Timer.SetName({ SystemName::PHYSICS });
		engine->Timer.tracker.system_start = glfwGetTime();

		for (auto const& entity : mEntities)
		{
			// Ps Guan
			auto& Transform = engine->world.GetComponent<TransformComponent>(entity);
			engine->gPicker.UpdateAabb(entity);

			engine->gPhysics.InitActor(entity);
			engine->gPhysics.UpdateActor(entity);
		}
		engine->gPhysics.Simulate();

		for (auto const& entity : mEntities)
		{
			engine->gPhysics.GetActorPosition(entity);
		}

		engine->Timer.tracker.system_end = glfwGetTime();
		engine->Timer.UpdateTimeContainer(engine->Timer.tracker);
	}
}