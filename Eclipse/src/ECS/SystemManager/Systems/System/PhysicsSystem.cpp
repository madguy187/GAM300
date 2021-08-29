#include "pch.h"
#include "PhysicsSystem.h"

void PhysicsSystem::Update()
{
	for (auto const& entity : mEntities)
	{
		engine->gPhysics.InitActor(entity);
		engine->gPhysics.UpdateActor(entity);
	}
	engine->gPhysics.Simulate();

	for (auto const& entity : mEntities)
	{
		engine->gPhysics.GetActorPosition(entity);
	}
}