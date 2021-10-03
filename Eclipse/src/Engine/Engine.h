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
#include "PrefabManager/PrefabManager.h"

namespace Eclipse
{
    class Engine
    {
    public:
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
        DragAndDrop dragDrop;
        PrefabManager pfManager;

        void Init();
        void Run();
        PhysicsManager gPhysics;
        std::unique_ptr<CullingManager> gCullingManager;
        std::unique_ptr<EclipseFileWatcher> gFileWatchManager;

        bool GetEditorState();
        bool GetPlayState();
        bool GetPauseState();
        bool GetStepState();
        bool IsScenePlaying();

        void SetEditorState(bool check);
        void SetPlayState(bool check);
        void SetPauseState(bool check);
        void SetStepState(bool check);
    private:
        bool IsEditorActive{ true };
        bool IsInPlayState{ false };
        bool IsInPauseState{ false };
        bool IsInStepState{ false };
    };
}