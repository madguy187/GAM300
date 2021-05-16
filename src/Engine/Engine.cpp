#include "pch.h"
#include "Engine.h"

//#include "ECS/World.h"
#include "ECS/ComponentManager/Components/TransformComponent.h"
#include "ECS/ComponentManager/Components/RenderComponent.h"
#include "ECS/SystemManager/Systems/TestSystem.h"

#include "../src/Graphics/include/GLHelper.h"
#include "../src/Graphics/include/Graphics.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <memory>
#include "imgui.h"

static void unload();
static void load();
static void init();
static void update();
static void draw(World& world);

namespace Eclipse
{
  void Engine::Init()
  {
    load();
    Graphics::init();
  }

  void Engine::Run()
  {
    // register component
    world.RegisterComponent<TransformComponent>();
    world.RegisterComponent<RenderComponent>();

    // registering system
    world.RegisterSystem<TestSystem>();

    // registering system signature
    world.RegisterSystemSignature<TestSystem>(0000'0001);

    Entity ent = world.CreateEntity();
    world.AddComponent(ent, TransformComponent{ 4.0f, 5.0f, 6.0f });
    //world.DestroyComponent<TransformComponent>(ent);

    auto& wee = world.GetComponent<TransformComponent>(ent);
    std::cout << wee.x << std::endl;

    world.GetSystem<TestSystem>();

    while (!glfwWindowShouldClose(GLHelper::ptr_window))
    {
      world.Update<TestSystem>();
      update();

      // draw
      //draw(world);
      //

    }

    //while (true)
    //{
    //	world.Update<TestSystem>();

    //	
    //}

    //unload();
  }
}

static void update()
{
  float fixedDeltaTime = 1.0 / 60.0;
  glfwPollEvents();

  //Graphics::update(fixedDeltaTime, world);

  std::stringstream sstr;
  sstr << std::fixed << std::setprecision(2) << GLHelper::title << " | FPS: " << GLHelper::fps;
  glfwSetWindowTitle(GLHelper::ptr_window, sstr.str().c_str());
}

static void draw(World& world)
{
  Graphics::draw(world);
  glfwSwapBuffers(GLHelper::ptr_window);
}

static void unload()
{
  //ImGui_ImplOpenGL3_Shutdown();
  ImGui::DestroyContext();
  GLHelper::cleanup();
  Graphics::cleanup();
}

void load()
{
  if (!GLHelper::init("Configuration/configuration.json"))
  {
    std::cout << "Unable to create OpenGL context" << std::endl;
    std::exit(EXIT_FAILURE);
  }

  Graphics::load();
}

void init()
{
}
