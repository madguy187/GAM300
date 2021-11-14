#pragma once
#include "ECS/World.h" 
#include "Time/Clock.h"
#include "Graphics/Camera/CameraManager.h"
#include "Graphics/Debugging/DebugRenderingManager.h"
#include "Editor/Manager/EditorManager.h"
#include "Graphics/Lighting/LightManager.h"
#include "Mono/Manager/MonoManager.h"
#include "AssimpModel/AssimpManager.h"
#include "Graphics/Material/MaterialManager.h"
#include "Graphics/Picker/PickingManager.h"
#include "Graphics/Grid/AABBTree.h"
#include "Physics/PhysicsManager.h"
#include "Editor/Windows/AssetBrowser/AssetBrowser.h"
#include "Graphics/Culling/CullingManager.h"
#include "Editor/Windows/Profiler/Profiler.h"
#include "Audio/AudioManager.h"
#include "Graphics/FileWatch/FileWatch.h"
#include "Graphics/Picker/DynamicAABBTree.h"
#include "PrefabManager/PrefabManager.h"
#include "Graphics/EngineCompiler/EngineCompiler.h"
#include "AI/AIManager.h"
#include "Graphics/FrameBuffer/FrameBufferManager.h"
#include "Graphics/Material/PBR/PBRManager.h"
#include "Graphics/InputHandler/LogicalInput.h"
#include "Graphics/Instancing/DebugManager/DebugManager.h"
#include "Graphics/Animation/AnimationManager.h"
#include "NavMesh/NavMeshManager.h"
#include "FSM/Manager/FSMManager.h"

namespace Eclipse
{
    class Engine
    {
    public:
        bool CheckEditor = false;

        World world;
        World prefabWorld;
        CameraManager gCamera;
        GraphicsManager GraphicsManager;
        ProfilerWindow Timer;
        Clock Game_Clock;
        std::unique_ptr<EditorManager> editorManager;
        LightManager LightManager;
        MonoManager mono;
        AssimpModelManager AssimpManager;
        PickingManager gPicker;
        MaterialManager MaterialManager{ true , true };
        AABBTree CollisionGridTree{ 150 };
        std::unique_ptr<Grid> GridManager;
        DebugRenderingManager gDebugManager;
        SerializationManager szManager;
        AudioManager audioManager;
        DynamicAABBTree gDynamicAABBTree;
        PrefabManager pfManager;
        PhysicsManager gPhysics;
        NavMeshManager gNavMesh;
        FSMManager gFSM;
        AIManager gAI;
        AnimationManager gAnimationManager;
        std::unique_ptr<CullingManager> gCullingManager;
        std::unique_ptr<EclipseFileWatcher> gFileWatchManager;
        std::unique_ptr<EngineCompiler> gEngineCompiler;
        std::unique_ptr<FrameBufferManager> gFrameBufferManager;
        std::unique_ptr<PBRManager> gPBRManager;
        std::unique_ptr<LogicalInput> InputManager;
        std::unique_ptr<DebugManager> gDebugDrawManager;

        void Init();
        void Run();
        bool GetEditorState();
        bool GetPlayState();
        bool GetPauseState();
        bool GetStepState();
        bool IsScenePlaying();
        void DestroyGameObject(const Entity& ent);
        void CleanUp(const Entity& ent);

        void SetEditorState(bool check);
        void SetPlayState(bool check);
        void SetPauseState(bool check);
        void SetStepState(bool check);
    private:
        bool IsEditorActive{ true };
        bool IsInPlayState{ true };
        bool IsInPauseState{ false };
        bool IsInStepState{ false };
    };
}