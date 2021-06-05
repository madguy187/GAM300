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
        RenderSystem::Init();

        if (EditorState)
            editorManager = std::make_unique<EditorManager>();
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

        // Render System
        Signature RenderSys = RenderSystem::RegisterAll();
        world.RegisterSystemSignature<RenderSystem>(RenderSys);

        Signature hi2;
        hi2.set(world.GetComponentType<TransformComponent>(), 1);
        hi2.set(world.GetComponentType<CameraComponent>(), 1);
        world.RegisterSystemSignature<CameraSystem>(hi2);

        //Check this! - Rachel
        CameraSystem::Init();

        float currTime = static_cast<float>(clock());
        float accumulatedTime = 0.0f;
        int framecount = 0;
        float dt = 0.0f;
        float updaterate = 4.0f;

        while (!glfwWindowShouldClose(OpenGL_Context::GetWindow()))
        {
            Game_Clock.set_timeSteps(0);
            float newTime = static_cast<float>(clock());
            Game_Clock.set_DeltaTime((newTime - currTime) / static_cast<float>(CLOCKS_PER_SEC));

            if (Game_Clock.get_DeltaTime() == 0.0f)
                Game_Clock.set_DeltaTime(0.0001f);

            dt += Game_Clock.get_DeltaTime();
            accumulatedTime += Game_Clock.get_DeltaTime();

            while (accumulatedTime >= Game_Clock.get_fixedDeltaTime())
            {
                Game_Clock.set_timeSteps(Game_Clock.get_timeSteps() + 1);
                accumulatedTime -= Game_Clock.get_fixedDeltaTime();

            }

            if (dt > 1.0f / updaterate)
            {
                Game_Clock.setFPS(static_cast<float>(framecount) / dt);
                framecount = 0;
                dt -= 1.0f / updaterate;
            }

            currTime = newTime;

            if (Game_Clock.get_timeSteps() > 10)
            {
                Game_Clock.set_timeSteps(10);
            }

            for (int step = 0; step < Game_Clock.get_timeSteps(); step++)
            {
                world.Update<CameraSystem>();
            }

            world.Update<RenderSystem>();
        }

        // unLoad
        gGraphics.end();
    }
    bool Engine::GetEditorState()
    {
        return EditorState;
    }
}