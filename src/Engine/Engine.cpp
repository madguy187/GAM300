#include "pch.h"
#include "Engine.h"

#include "ECS/ComponentManager/Components/TransformComponent.h"
#include "ECS/ComponentManager/Components/RenderComponent.h"

namespace Eclipse
{
    void Engine::Init()
    {

    }

    void Engine::Run()
    {
        // register component
        world.RegisterComponent<TransformComponent>();
        world.RegisterComponent<RenderComponent>();

        // registering system

        // registering system signature
        Signature hi;
        hi.set(world.GetComponentType<TransformComponent>(), 1);

        Entity ent = world.CreateEntity();
        world.AddComponent(ent, TransformComponent{ 4.0f, 5.0f, 6.0f });

        auto& wee = world.GetComponent<TransformComponent>(ent);

        while (true)
        {
            std::cout << world.GetComponent<TransformComponent>(ent).x << std::endl;
        }
    }
}