#include "pch.h"
#include "../PhysicsSystem/PhysicsSystem.h"

void PhysicsSystem::Update()
{
	ZoneScopedN("Physics System")
	engine->Timer.SetName({ SystemName::PHYSICS });
	engine->Timer.tracker.system_start = static_cast<float>(glfwGetTime());

	for (auto const& entity : mEntities)
	{
		// Ps Guan
		engine->gPicker.UpdateAabb(entity);
		engine->gPhysics.InitActor(entity);
		engine->gPhysics.UpdateActor(entity);
	}
	engine->gPhysics.Simulate();

	for (auto const& entity : mEntities)
	{
		engine->gPhysics.GetActorPosition(entity);
	}

	engine->Timer.tracker.system_end = static_cast<float>(glfwGetTime());
	engine->Timer.UpdateTimeContainer(engine->Timer.tracker);
	FrameMark
}