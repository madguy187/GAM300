#include "pch.h"
#include "PhysicsSystem.h"

void PhysicsSystem::Update()
{
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
}