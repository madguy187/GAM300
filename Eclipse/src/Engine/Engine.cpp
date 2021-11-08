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
#include "ECS/ComponentManager/Components/ModelComponent.h"
#include "ECS/ComponentManager/Components/LightComponent.h"
#include "ECS/ComponentManager/Components/ScriptComponent.h"
#include "ECS/ComponentManager/Components/PrefabComponent.h"
#include "ECS/ComponentManager/Components/ParentComponent.h"
#include "ECS/ComponentManager/Components/ChildComponent.h"
#include "ECS/ComponentManager/Components/AudioComponent.h"
#include "ECS/ComponentManager/Components/ParentComponent.h"
#include "ECS/ComponentManager/Components/ChildComponent.h"
#include "ECS/ComponentManager/Components/NavMeshVolumeComponent.h"

#include "ECS/SystemManager/Systems/System/RenderSystem/RenderSystem.h"
#include "ECS/SystemManager/Systems/System/CameraSystem/CameraSystem.h"
#include "ECS/SystemManager/Systems/System/Editor/EditorSystem.h"
#include "ECS/SystemManager/Systems/System/LightingSystem/LightingSystem.h"
#include "ECS/SystemManager/Systems/System/PickingSystem/PickingSystem.h"
#include "ECS/SystemManager/Systems/System/PhysicsSystem/PhysicsSystem.h"
#include "ImGui/Setup/ImGuiSetup.h"
#include "ECS/SystemManager/Systems/System/MaterialSystem/MaterialSystem.h"
#include "Serialization/SerializationManager.h"
#include "ECS/SystemManager/Systems/System/GridSystem/GridSystem.h"
#include "Editor/ECGuiAPI/ECGuiInputHandler.h"
#include "ECS/SystemManager/Systems/System/MonoSystem/MonoSystem.h"
#include "ECS/SystemManager/Systems/System/Audio/AudioSystem.h"
#include "ECS/SystemManager/Systems/System/FileWatchSystem/FileWatchSystem.h"
#include "ECS/SystemManager/Systems/System/Collision/CollisionSystem.h"
#include <ECS/SystemManager/Systems/System/ParentChildSystem/ParentSystem/ParentSystem.h>
#include <ECS/SystemManager/Systems/System/ParentChildSystem/ChildSystem/ChildSystem.h>
#include "ECS/SystemManager/Systems/System/PrefabSystem/PrefabSystem.h"
#include "ECS/SystemManager/Systems/System/AI/AISystem.h"
#include "ECS/SystemManager/Systems/System/InputSystem/InputSystem.h"
#include "Editor/Windows/NodeEditor/NodeEditor.h"

#include "ECS/SystemManager/Systems/System/NavMeshSystem/NavMeshSystem.h"
bool Tester1(const Test1&)
{
    std::cout << "Engine.cpp Tester1" << std::endl;
    return false;
}

bool Tester2(const Test1&)
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

        engine->gFrameBufferManager = std::make_unique<FrameBufferManager>();
        engine->gDebugDrawManager = std::make_unique<DebugManager>();

        engine->GraphicsManager.Pre_Render();

        ImGuiSetup::Init(IsEditorActive);

        if (IsEditorActive)
            editorManager = std::make_unique<EditorManager>();

        InputManager = std::make_unique<LogicalInput>();
        glfwSetWindowCloseCallback(OpenGL_Context::GetWindow(), GraphicsManager.WindowCloseCallback);
    }

    void Engine::Run()
    {
        ZoneScopedN("Engine")

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
        world.RegisterComponent<ModelComponent>();
        world.RegisterComponent<LightComponent>();
        world.RegisterComponent<ScriptComponent>();
        world.RegisterComponent<AudioComponent>();
        world.RegisterComponent<ParentComponent>();
        world.RegisterComponent<ChildComponent>();
        world.RegisterComponent<CollisionComponent>();
        world.RegisterComponent<PrefabComponent>();
        world.RegisterComponent<AIComponent>();
        world.RegisterComponent<NodeEditor>();
        world.RegisterComponent<NavMeshVolumeComponent>();

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
        prefabWorld.RegisterComponent<ModelComponent>();
        prefabWorld.RegisterComponent<LightComponent>();
        prefabWorld.RegisterComponent<ScriptComponent>();
        prefabWorld.RegisterComponent<AudioComponent>();
        prefabWorld.RegisterComponent<ParentComponent>();
        prefabWorld.RegisterComponent<ChildComponent>();
        prefabWorld.RegisterComponent<CollisionComponent>();
        prefabWorld.RegisterComponent<PrefabComponent>();
        prefabWorld.RegisterComponent<AIComponent>();
        prefabWorld.RegisterComponent <NavMeshVolumeComponent>();

        prefabWorld.RegisterComponent<NodeEditor>();
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
        world.RegisterSystem<ParentSystem>();
        world.RegisterSystem<ChildSystem>();
        world.RegisterSystem<CollisionSystem>();
        world.RegisterSystem<PrefabSystem>();
        world.RegisterSystem<AISystem>();
        world.RegisterSystem<InputSystem>();
        prefabWorld.RegisterSystem<PrefabSystem>();

        // Render System
        Signature SystemSignature;
        SystemSignature.set(engine->world.GetComponentType<TransformComponent>(), 1);
        SystemSignature.set(engine->world.GetComponentType<MeshComponent>(), 1);
        engine->world.RegisterSystemSignature<RenderSystem>(SystemSignature);

        Signature CameraSys;
        CameraSys.set(world.GetComponentType<TransformComponent>(), 1);
        CameraSys.set(world.GetComponentType<CameraComponent>(), 1);
        world.RegisterSystemSignature<CameraSystem>(CameraSys);

        Signature LightingSys;
        LightingSys.set(world.GetComponentType<LightComponent>(), 1);
        world.RegisterSystemSignature<LightingSystem>(LightingSys);

        Signature MateriakSys;
        MateriakSys.set(world.GetComponentType<MaterialComponent>(), 1);
        world.RegisterSystemSignature<MaterialSystem>(MateriakSys);

        Signature PickingSys;
        PickingSys.set(world.GetComponentType<AABBComponent>(), 1);
        PickingSys.set(world.GetComponentType<TransformComponent>(), 1);
        PickingSys.set(world.GetComponentType<MaterialComponent>(), 1);
        world.RegisterSystemSignature<PickingSystem>(PickingSys);

        Signature gridCol;
        gridCol.set(world.GetComponentType<AABBComponent>(), 1);
        gridCol.set(world.GetComponentType<TransformComponent>(), 1);
        world.RegisterSystemSignature<GridSystem>(gridCol);

        Signature hi4;
        hi4.set(world.GetComponentType<TransformComponent>(), 1);
        hi4.set(world.GetComponentType<RigidBodyComponent>(), 1);
        world.RegisterSystemSignature<PhysicsSystem>(hi4);

        Signature scriptSignature;
        scriptSignature.set(world.GetComponentType<ScriptComponent>(), 1);
        world.RegisterSystemSignature<MonoSystem>(scriptSignature);

        Signature parentSignature;
        parentSignature.set(world.GetComponentType<ParentComponent>(), 1);
        world.RegisterSystemSignature<ParentSystem>(parentSignature);

        Signature childSignature;
        childSignature.set(world.GetComponentType<ChildComponent>(), 1);
        world.RegisterSystemSignature<ChildSystem>(childSignature);

        Signature hi6;
        hi6.set(world.GetComponentType<CollisionComponent>(), 1);
        world.RegisterSystemSignature<CollisionSystem>(hi6);

        Signature audioSignature;
        audioSignature.set(world.GetComponentType<AudioComponent>(), 1);
        audioSignature.set(world.GetComponentType<TransformComponent>(), 1);
        world.RegisterSystemSignature<AudioSystem>(audioSignature);

        Signature prefabSig;
        prefabSig.set(world.GetComponentType<PrefabComponent>(), 1);
        world.RegisterSystemSignature<PrefabSystem>(prefabSig);

        Signature prefabSig2;
        prefabSig2.set(prefabWorld.GetComponentType<PrefabComponent>(), 1);
        prefabWorld.RegisterSystemSignature<PrefabSystem>(prefabSig2);

        Signature AIsig;
        AIsig.set(world.GetComponentType<AIComponent>(), 1);
        AIsig.set(world.GetComponentType<TransformComponent>(), 1);
        AIsig.set(world.GetComponentType<RigidBodyComponent>(), 1);
        world.RegisterSystemSignature<AISystem>(AIsig);

        Signature parentSys;
        parentSys.set(world.GetComponentType<ParentComponent>(), 1);
        world.RegisterSystemSignature<ParentSystem>(parentSys);

        //Check this! - Rachel
        CameraSystem::Init();
        RenderSystem::Init();
        engine->editorManager->TextureIconInit();
        gPhysics.Init();
        audioManager.Init();

        pfManager.LoadAllPrefab();

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

        // Check for Recovery File
        if (IsEditorActive)
            engine->editorManager->SetRecoveryFileExistence(szManager.CheckBackUpPathExistence());

        /*audioManager.PlaySounds("src/Assets/Sounds/WIN.wav", 0.5f, true);*/
        //audioManager.PlayEvent("event:/WaterEffect");

        /*TransformComponent trans1;
        TransformComponent trans2;
        trans2.position.setX(5.0f);
        trans1.position.setX(5.0f);
        RefVariant ob1 = trans1;
        RefVariant ob2 = trans2;

        if (SerializationManager::CompareComponentData(ob1, ob2))
            std::cout << "its the same!" << std::endl;
        else
            std::cout << "its the not same!" << std::endl;*/

        while (!glfwWindowShouldClose(OpenGL_Context::GetWindow()))
        {
            glfwPollEvents();
            engine->gFrameBufferManager->MainWindowSettings();
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
            //ECGuiInputHandler::Update();
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
                world.Update<AISystem>();

            world.Update<CollisionSystem>();
            if (IsScenePlaying())
            {
                for (int step = 0; step < Game_Clock.get_timeSteps(); step++)
                {
                    world.Update<PhysicsSystem>();

                    mono.fixUpdate = true;
                    world.Update<MonoSystem>();
                    mono.fixUpdate = false;
                }
            }

            world.Update<FileWatchSystem>();

            world.Update<ParentSystem>();
            world.Update<ChildSystem>();

            engine->gFrameBufferManager->GlobalBind();

            // Reset DebugBoxes =============================
            engine->gDebugDrawManager->Reset();

            // LIGHTINGSYSTEM =============================
            world.Update<LightingSystem>();

            // // PICKINGSYSTEM =============================
            world.Update<PickingSystem>();

            // // AUDIOSYSTEM =============================
            world.Update<AudioSystem>();

            // MATERIALSYSTEM =============================
            world.Update<MaterialSystem>();

            // RENDERSYSTEM =============================
            world.Update<RenderSystem>();

            // Final DRAW ================================ 
            //engine->gDebugDrawManager->Render();
            engine->GraphicsManager.FinalRender();

            if (IsScenePlaying())
            {
                world.Update<MonoSystem>();
                mono.UpdateInvokers();
            }

            // FRAMEBUFFER DRAW ==========================
            engine->gFrameBufferManager->FrameBufferDraw();

            ImGuiSetup::End(IsEditorActive);
            OpenGL_Context::post_render();

            SceneManager::ProcessScene();

            ProfilerWindow::engine_time = 0;

            if (IsInStepState)
            {
                IsInStepState = false;
                IsInPauseState = true;
            }

            world.Update<InputSystem>();
        }

        //Serialization(Temp)
        szManager.SaveSceneFile();
        pfManager.UnloadSaving();
        // unLoad
        mono.Terminate();
        GraphicsManager.End();
        ImGuiSetup::Destroy(IsEditorActive);
        gPhysics.Unload();
        CommandHistory::Clear();

        FrameMark
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

    void Engine::DestroyGameObject(const Entity& ent)
    {
        if (engine->world.CheckComponent<ParentComponent>(ent))
        {
            auto& parentComp = engine->world.GetComponent<ParentComponent>(ent);

            for (auto& child : parentComp.child)
            {
                DestroyGameObject(child);
            }

            CleanUp(ent);
        }
        else
        {
            CleanUp(ent);
        }
    }

    void Engine::CleanUp(const Entity& ent)
    {
        engine->gDynamicAABBTree.RemoveData(ent);
        engine->gCullingManager->Remove(ent);
        engine->LightManager.DestroyLight(ent);
        engine->gPhysics.RemoveActor(ent);

        if (IsEditorActive)
        {
            engine->editorManager->DestroyEntity(ent);
            engine->gPicker.SetCurrentCollisionID(engine->editorManager->GetSelectedEntity());
        }
        else
        {
            world.DestroyEntity(ent);
        }
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
