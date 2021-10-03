#include "pch.h"
#include "Engine.h"

#include "ECS/ComponentManager/Components/EntityComponent.h"
#include "ECS/ComponentManager/Components/TransformComponent.h"
#include "ECS/ComponentManager/Components/CameraComponent.h"
#include "ECS/ComponentManager/Components/AABBComponent.h"
#include "ECS/ComponentManager/Components/DirectionalLightComponent.h"
#include "ECS/ComponentManager/Components/SpotLightComponent.h"
#include "ECS/ComponentManager/Components/MaterialComponent.h"
#include "ECS/ComponentManager/Components/MeshComponent.h"
#include "ECS/ComponentManager/Components/RigidBodyComponent.h"
#include "ECS/ComponentManager/Components/TextureComponent.h"
#include "ECS/ComponentManager/Components/ModelInfoComponent.h"
#include "ECS/ComponentManager/Components/LightComponent.h"
#include "ECS/ComponentManager/Components/ScriptComponent.h"

#include "ECS/SystemManager/Systems/System/RenderSystem/RenderSystem.h"
#include "ECS/SystemManager/Systems/System/CameraSystem.h"
#include "ECS/SystemManager/Systems/System/Editor/EditorSystem.h"
#include "ECS/SystemManager/Systems/System/LightingSystem/LightingSystem.h"
#include "ECS/SystemManager/Systems/System/PickingSystem.h"
#include "ECS/SystemManager/Systems/System/PhysicsSystem.h"
#include "ImGui/Setup/ImGuiSetup.h"
#include "ECS/SystemManager/Systems/System/MaterialSystem/MaterialSystem.h"
#include "Serialization/SerializationManager.h"
#include "ECS/SystemManager/Systems/System/GridSystem/GridSystem.h"
#include "Editor/ECGuiAPI/ECGuiInputHandler.h"
#include "ECS/SystemManager/Systems/System/MonoSystem/MonoSystem.h"
#include "ECS/SystemManager/Systems/System/Audio/AudioSystem.h"
#include "ECS/SystemManager/Systems/System/FileWatchSystem/FileWatchSystem.h"

bool Tester1(const Test1& e)
{
    std::cout << "Engine.cpp Tester1" << std::endl;
    return false;
}

bool Tester2(const Test1& e)
{
    std::cout << "Engine.cpp Tester2" << std::endl;
    return false;
}

namespace Eclipse
{
    void Engine::Init()
    {
        mono.Init();

        // multiple listener calls
        EventSystem<Test1>::registerListener(Tester1);
        EventSystem<Test1>::registerListener(Tester2);
        EventSystem<Test1>::registerListener(std::bind(&World::TempFunc, &world, std::placeholders::_1));

        struct Test1 t {};
        EventSystem<Test1>::dispatchEvent(t);
        std::cout << "ENDED" << std::endl;

        engine->GraphicsManager.Pre_Render();
    	
        ImGuiSetup::Init(IsEditorActive);

        if (IsEditorActive)
            editorManager = std::make_unique<EditorManager>();

        /*bool x = false;
        std::string msg = "woo";
        ENGINE_LOG_ASSERT(x, msg.c_str());*/
        glfwSetWindowCloseCallback(OpenGL_Context::GetWindow(), GraphicsManager.WindowCloseCallback);
    }

    void Engine::Run()
    {
       //ZoneScopedN("Engine")
        // register component
        world.RegisterComponent<EntityComponent>();
        world.RegisterComponent<TransformComponent>();
        world.RegisterComponent<MeshComponent>();
        world.RegisterComponent<CameraComponent>();
        world.RegisterComponent<PointLightComponent>();
        world.RegisterComponent<DirectionalLightComponent>();
        world.RegisterComponent<AABBComponent>();
        world.RegisterComponent<SpotLightComponent>();
        world.RegisterComponent<MaterialComponent>();
        world.RegisterComponent<RigidBodyComponent>();
        world.RegisterComponent<TextureComponent>();
        world.RegisterComponent<ModeLInforComponent>();
        world.RegisterComponent<LightComponent>();
        world.RegisterComponent<ScriptComponent>();
        world.RegisterComponent<PrefabComponent>();

        prefabWorld.RegisterComponent<EntityComponent>();
        prefabWorld.RegisterComponent<TransformComponent>();
        prefabWorld.RegisterComponent<MeshComponent>();
        prefabWorld.RegisterComponent<CameraComponent>();
        prefabWorld.RegisterComponent<PointLightComponent>();
        prefabWorld.RegisterComponent<DirectionalLightComponent>();
        prefabWorld.RegisterComponent<AABBComponent>();
        prefabWorld.RegisterComponent<SpotLightComponent>();
        prefabWorld.RegisterComponent<MaterialComponent>();
        prefabWorld.RegisterComponent<RigidBodyComponent>();
        prefabWorld.RegisterComponent<TextureComponent>();
        prefabWorld.RegisterComponent<ModeLInforComponent>();
        prefabWorld.RegisterComponent<ParentChildComponent>();
        prefabWorld.RegisterComponent<LightComponent>();
        prefabWorld.RegisterComponent<ScriptComponent>();
        prefabWorld.RegisterComponent<PrefabComponent>();

        // registering system
        world.RegisterSystem<RenderSystem>();
        world.RegisterSystem<CameraSystem>();
        world.RegisterSystem<LightingSystem>();
        world.RegisterSystem<MaterialSystem>();
        world.RegisterSystem<GridSystem>();
        world.RegisterSystem<PickingSystem>();
        world.RegisterSystem<PhysicsSystem>();
        world.RegisterSystem<MonoSystem>();
        world.RegisterSystem<AudioSystem>();
        world.RegisterSystem<FileWatchSystem>();

        // Render System
        Signature RenderSys = RenderSystem::RegisterAll();
        world.RegisterSystemSignature<RenderSystem>(RenderSys);

        Signature hi2;
        hi2.set(world.GetComponentType<TransformComponent>(), 1);
        hi2.set(world.GetComponentType<CameraComponent>(), 1);
        world.RegisterSystemSignature<CameraSystem>(hi2);

        Signature hi3;
        hi3.set(world.GetComponentType<LightComponent>(), 1);
        world.RegisterSystemSignature<LightingSystem>(hi3);

        Signature mat;
        mat.set(world.GetComponentType<MaterialComponent>(), 1);
        world.RegisterSystemSignature<MaterialSystem>(mat);

        Signature picking;
        picking.set(world.GetComponentType<AABBComponent>(), 1);
        picking.set(world.GetComponentType<TransformComponent>(), 1);
        picking.set(world.GetComponentType<MaterialComponent>(), 1);
        world.RegisterSystemSignature<PickingSystem>(picking);

        Signature gridCol;
        gridCol.set(world.GetComponentType<AABBComponent>(), 1);
        gridCol.set(world.GetComponentType<TransformComponent>(), 1);
        world.RegisterSystemSignature<GridSystem>(gridCol);

        Signature hi4;
        hi4.set(world.GetComponentType<TransformComponent>(), 1);
        hi4.set(world.GetComponentType<RigidBodyComponent>(), 1);
        world.RegisterSystemSignature<PhysicsSystem>(hi4);

        Signature hi5;
        hi5.set(world.GetComponentType<ScriptComponent>(), 1);
        world.RegisterSystemSignature<MonoSystem>(hi5);

        //Check this! - Rachel
        RenderSystem::Init();
        CameraSystem::Init();
        gPhysics.Init();
        audioManager.Init();

        if (IsEditorActive)
            IsInPlayState = false;
        else
            IsInPlayState = true;

        float currTime = static_cast<float>(clock());
        float accumulatedTime = 0.0f;
        int framecount = 0;
        float dt = 0.0f;
        float updaterate = 4.0f;

        SceneManager::Initialize();

        // Darren - Please keep this before Game Loop
        engine->GraphicsManager.MassInit();

        //Deserialization(temp)
        /*audioManager.PlaySounds("src/Assets/Sounds/WIN.wav", 0.5f, true);*/
        while (!glfwWindowShouldClose(OpenGL_Context::GetWindow()))
        {
            glfwPollEvents();
            engine->GraphicsManager.mRenderContext.SetClearColor({ 0.1f, 0.2f, 0.3f, 1.f });

            Game_Clock.set_timeSteps(0);
            framecount++;
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
            ECGuiInputHandler::Update();

            ImGuiSetup::Begin(IsEditorActive);

            EditorSystem::Update();

            if (IsInStepState)
            {
                Game_Clock.set_timeSteps(1);
                IsInPauseState = false;
            }
            else
            {
                if (Game_Clock.get_timeSteps() > 10)
                {
                    Game_Clock.set_timeSteps(10);
                }
            }

            // GRID SYSTEM =============================
            //world.Update<GridSystem>();

            world.Update<CameraSystem>();

            if (IsScenePlaying())
            {
                for (int step = 0; step < Game_Clock.get_timeSteps(); step++)
                {
                    world.Update<PhysicsSystem>();
                }
            }

            world.Update<FileWatchSystem>();

            // FRAMEBUFFER BIND =============================
            engine->GraphicsManager.GlobalFrameBufferBind();

            // Reset DebugBoxes =============================
            //engine->GraphicsManager.ResetInstancedDebugBoxes();

            // LIGHTINGSYSTEM =============================
            world.Update<LightingSystem>();

            world.Update<PickingSystem>();

            // AUDIOSYSTEM =============================
            world.Update<AudioSystem>();

            // RENDERSYSTEM =============================
            world.Update<RenderSystem>();

            // Material SYstem =============================
            //world.Update<MaterialSystem>();

            // GRID DRAW ============================= Must be last of All Renders
            engine->GridManager->DrawGrid(engine->GraphicsManager.mRenderContext.GetFramebuffer(Eclipse::FrameBufferMode::FBM_SCENE)->GetFrameBufferID());

            if (IsScenePlaying())
            {
                world.Update<MonoSystem>();
            }

            // FRAMEBUFFER DRAW ==========================
            engine->GraphicsManager.GlobalFrmeBufferDraw();

            ImGuiSetup::End(IsEditorActive);
            OpenGL_Context::post_render();
            SceneManager::ProcessScene();

            ProfilerWindow::engine_time = 0;

            if (IsInStepState)
            {
                IsInStepState = false;
                IsInPauseState = true;
            }
        }

        //Serialization(Temp)
        szManager.SaveSceneFile();

        // unLoad
        mono.Terminate();
        GraphicsManager.End();
        AssimpManager.CleanUpAllModelsMeshes();
        ImGuiSetup::Destroy(IsEditorActive);
        gPhysics.Unload();
        CommandHistory::Clear();

        //FrameMark
    }

    bool Engine::GetEditorState()
    {
        return IsEditorActive;
    }

    bool Engine::GetPlayState()
    {
        return IsInPlayState;
    }

    bool Engine::GetPauseState()
    {
        return IsInPauseState;
    }

    bool Engine::GetStepState()
    {
        return IsInStepState;
    }

    bool Engine::IsScenePlaying()
    {
        return IsInPlayState && !IsInPauseState;
    }

    void Engine::SetEditorState(bool check)
    {
        IsEditorActive = check;
    }

    void Engine::SetPlayState(bool check)
    {
        IsInPlayState = check;
    }

    void Engine::SetPauseState(bool check)
    {
        IsInPauseState = check;
    }

    void Engine::SetStepState(bool check)
    {
        IsInStepState = check;
    }
}
