#pragma once
#include "ECS/World.h" 
#include "Time/Clock.h"
#include "Graphics/Camera/CameraManager.h"
#include "Graphics/Debugging/DebugRenderingManager.h"
#include "Editor/Manager/EditorManager.h"
#include "Graphics/Lighting/LightManager.h"
#include "Mono/Manager/MonoManager.h"

namespace Eclipse
{
	class Engine
	{
	public:
		World world;
		Clock Game_Clock;
		std::unique_ptr<EditorManager> editorManager;

		// GRAPHICS MANAGERS
		CameraManager gCamera;
		GraphicsManager gGraphics;
		DebugRenderingManager gDebugManager;
		Lights LightManager;

		MonoManager mono;

		void Init();
		void Run();

		bool GetEditorState();
	private:
		bool EditorState{ true };
	};
}