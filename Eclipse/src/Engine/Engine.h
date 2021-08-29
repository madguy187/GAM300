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

namespace Eclipse
{
	class Engine
	{
	public:
		World world;
		CameraManager gCamera;
		GraphicsManager GraphicsManager;
		Clock Game_Clock;
		DebugRenderingManager gDebugManager;
		std::unique_ptr<EditorManager> editorManager;
		LightManager LightManager;
		MonoManager mono;
	    AssimpModelManager AssimpManager;
		PickingManager gPicker;
        MaterialManager MaterialManager{ true , true };
        
        void Init();
        void Run();

        bool GetEditorState();
    private:
        bool EditorState{ true };
    };
}