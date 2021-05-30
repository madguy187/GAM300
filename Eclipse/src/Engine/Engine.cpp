#include "pch.h"
#include "Engine.h"

#include "ECS/ComponentManager/Components/TransformComponent.h"
#include "ECS/ComponentManager/Components/RenderComponent.h"
#include "ECS/ComponentManager/Components/CameraComponent.h"
#include "ECS/SystemManager/Systems/System/RenderSystem.h"
#include "ECS/SystemManager/Systems/System/CameraSystem.h"

namespace Eclipse
{
    void Engine::Init()
    {
      //ECVec2 woo;
      //glm::vec2 koo{ 1,2 };
      //woo += koo;
      //glm::vec2 loo = woo.ConvertToGlmVec2Type();

      /*ECMat4 mat{ 1,0,0,1,0,2,1,2,2,1,0,1,2,0,1,4 };
      ECMat4 mat2;
      ECMtx44Inverse(&mat2, mat);
      std::cout << mat2;*/
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
      world.RegisterSystem<CameraSystem>();

      // registering system signature
      Signature hi;
      hi.set(world.GetComponentType<TransformComponent>(), 1);
      hi.set(world.GetComponentType<Sprite>(), 1);

      world.RegisterSystemSignature<RenderSystem>(hi);

      Signature hi2;
      hi2.set(world.GetComponentType<TransformComponent>(), 1);
      hi2.set(world.GetComponentType<CameraComponent>(), 1);

      world.RegisterSystemSignature<CameraSystem>(hi2);

      Entity ent = world.CreateEntity();
      //world.AddComponent(ent, TransformComponent{ 4.0f, 5.0f, 6.0f });
      //world.AddComponent(ent, CameraComponent{ });
      world.AddComponent(ent, Sprite{ });

      //Check this! - Rachel
      CameraSystem::Init();

      while (!glfwWindowShouldClose(OpenGL_Context::ptr_window))
      {
          //Please remove when delta-time implementation added - Rachel
          OpenGL_Context::update_time(1.0f);

        world.Update<RenderSystem>();
        world.Update<CameraSystem>();
      }

      RenderSystem::unLoad();
    }
}