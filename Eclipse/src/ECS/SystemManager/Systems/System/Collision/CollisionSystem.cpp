#include "pch.h"
#include "CollisionSystem.h"

void CollisionSystem::Update()
{
	for (auto const& entity : mEntities)
	{
		if (!engine->world.CheckComponent<RigidBodyComponent>(entity))
		{
			engine->gPhysics.InitActor(entity);
			engine->gPhysics.UpdateActor(entity);
		}
	}
}