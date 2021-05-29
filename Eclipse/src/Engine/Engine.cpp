#include "pch.h"
#include "Engine.h"

#include "ECS/ComponentManager/Components/TransformComponent.h"
#include "ECS/ComponentManager/Components/RenderComponent.h"
#include "ECS//ComponentManager/Components/CameraComponent.h"
#include "ECS/SystemManager/Systems/System/RenderSystem.h"

namespace Eclipse
{
    void Engine::Init()
    {
      RenderSystem::Load();
    }

    void Engine::Run()
    {
      // register component
      world.RegisterComponent<TransformComponent>();
      world.RegisterComponent<RenderComponent>();
      world.RegisterComponent<CameraComponent>();
      world.RegisterComponent<Sprite>();

      // registering system
      world.RegisterSystem<RenderSystem>();

      // registering system signature
      Signature hi;
      hi.set(world.GetComponentType<TransformComponent>(), 1);
      hi.set(world.GetComponentType<Sprite>(), 1);

      world.RegisterSystemSignature<RenderSystem>(hi);

      Entity ent = world.CreateEntity();
      world.AddComponent(ent, TransformComponent{ 4.0f, 5.0f, 6.0f });
      world.AddComponent(ent, CameraComponent{ });
      world.AddComponent(ent, Sprite{ });

      while (!glfwWindowShouldClose(GLHelper::ptr_window))
      {

        // Darren was here
        world.Update<RenderSystem>();




      }

      RenderSystem::unLoad();
    }
}