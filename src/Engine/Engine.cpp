#include "pch.h"
#include "Engine.h"

//#include "ECS/World.h"
#include "ECS/ComponentManager/Components/TransformComponent.h"
#include "ECS/SystemManager/Systems/TestSystem.h"

namespace Eclipse
{
	void Engine::Init()
	{

	}

	void Engine::Run()
	{
		world.RegisterComponent<TransformComponent>();
		world.RegisterSystem<TestSystem>();
		world.RegisterSystemSignature<TestSystem>(0000'0001);
		

		Entity ent = world.CreateEntity();
		world.AddComponent(ent, TransformComponent{ 4.0f, 5.0f, 6.0f });
		//world.DestroyComponent<TransformComponent>(ent);
		auto& wee = world.GetComponent<TransformComponent>(ent);

		std::cout << wee.x << std::endl;

		while (true)
		{

		}
	}
}