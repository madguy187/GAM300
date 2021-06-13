#pragma once
#include "ECS/World.h" 
#include "Time/Clock.h"
#include "Graphics/Camera/CameraManager.h"
#include "Graphics/Debugging/DebugRenderingManager.h"
#include "Editor/Manager/EditorManager.h"
#include "Physics/PhysicsManager.h"
#include "Graphics/Lighting/LightManager.h"
#include "Mono/Manager/MonoManager.h"

namespace Eclipse
{
	class Engine
	{
	public:
		World world;
		CameraManager gCamera;
		GraphicsManager gGraphics;
		PhysicsManager gPhysics;
		Clock Game_Clock;
		DebugRenderingManager gDebugManager;
		std::unique_ptr<EditorManager> editorManager;
		Lights LightManager;
		MonoManager mono;

		void Init();
		void Run();

		bool GetEditorState();
	private:
		bool EditorState{ true };
	};
}