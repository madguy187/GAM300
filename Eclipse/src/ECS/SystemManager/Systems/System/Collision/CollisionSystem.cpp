#include "pch.h"
#include "CollisionSystem.h"

void CollisionSystem::Update()
{
	for (auto const& entity : mEntities)
	{
		engine->gPhysics.CreateShape(entity);
		engine->gPhysics.UpdateShapes(entity);
		if (!engine->world.CheckComponent<RigidBodyComponent>(entity))
		{
			engine->gPhysics.InitActor(entity);
			engine->gPhysics.UpdateActor(entity);
		}
	}
}