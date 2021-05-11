#include "pch.h"
#include "Engine.h"

#include "ECS/World.h"
#include "ECS/ComponentManager/Components/TransformComponent.h"

namespace Eclipse
{
	void Engine::Init()
	{

	}

	void Engine::Run()
	{
		World world;
		world.RegisterComponent<TransformComponent>();

		Entity ent = world.CreateEntity();
		world.AddComponent(ent, TransformComponent{ 4.0f, 5.0f, 6.0f });
		world.DestroyComponent<TransformComponent>(ent);
		//world.GetComponent<TransformComponent>(ent);

		while (true)
		{

		}
	}
}