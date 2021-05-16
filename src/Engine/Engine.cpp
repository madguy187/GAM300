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
#include "imgui_impl_opengl3.h"
#include "imgui_impl_glfw.h"

static void unload();
static void load();
static void init();
static void update();
static void draw();
vec4 t_color = vec4(0.4f, 0.28f, 0.30f, 1.00f);

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
    world.RegisterComponent<Camera>();
    world.RegisterComponent<Sprite>();

    // registering system
    world.RegisterSystem<TestSystem>();

    // registering system signature
    world.RegisterSystemSignature<TestSystem>(0000'0001);

    Entity ent = world.CreateEntity();
    world.AddComponent(ent, TransformComponent{ 4.0f, 5.0f, 6.0f });
    world.AddComponent(ent, Camera{ } );
    world.AddComponent(ent, Sprite{ });
    Camera& test = engine->world.GetComponent<Camera>(ent);
    test.prevPos.x = 9999;

    Sprite& sprite = engine->world.GetComponent<Sprite>(ent);
    sprite.shaderRef = Graphics::shaderpgms.find("shader3DShdrpgm");
    sprite.modelRef = Graphics::models.find("sphere");
    sprite.layerNum = 10;
    Graphics::sprites.emplace(sprite.layerNum, &sprite);

    auto& wee = world.GetComponent<TransformComponent>(ent);
    std::cout << wee.x << std::endl;

    world.GetSystem<TestSystem>();

    while (!glfwWindowShouldClose(GLHelper::ptr_window))
    {
      update();

      glBindFramebuffer(GL_FRAMEBUFFER, Graphics::framebuffer);
      glClear(GL_COLOR_BUFFER_BIT);
      world.Update<TestSystem>();
      draw();
      // draw
      //draw(world);
      //

    }

    //while (true)
    //{
    //	world.Update<TestSystem>();

    //	
    //}

    unload();
  }
}

static void update()
{
  float fixedDeltaTime = 1.0 / 60.0;
  glfwPollEvents();

  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();

  Graphics::update(fixedDeltaTime);

  std::stringstream sstr;
  sstr << std::fixed << std::setprecision(2) << GLHelper::title << " | FPS: " << GLHelper::fps;
  glfwSetWindowTitle(GLHelper::ptr_window, sstr.str().c_str());
}

static void draw()
{
  Graphics::draw();

  ImGui::Render();
  int Width, Height;

  glClearColor(t_color.x, t_color.y, t_color.z, t_color.w);

  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
  glfwGetFramebufferSize(GLHelper::ptr_window, &Width, &Height);

  glViewport(0, 0, Width, Height);

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

  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO(); (void)io;
  ImGui_ImplGlfw_InitForOpenGL(GLHelper::ptr_window, true);
  ImGui_ImplOpenGL3_Init();
  ImGui::StyleColorsClassic();
  io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
}

void init()
{
}
