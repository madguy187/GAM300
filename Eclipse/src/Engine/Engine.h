#pragma once
#include "ECS/World.h" 
#include "Time/Clock.h"
#include "Vec.h" /*Please remove after graphics cleanup -Rachel*/
#include "Graphics/Camera/CameraManager.h"
#include "Graphics/Debugging/DebugRenderingManager.h"
#include "Editor/Manager/EditorManager.h"
#include "Physics/PhysicsManager.h"
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
		MonoManager mono;

		void Init();
		void Run();

		bool GetEditorState();
	private:
		bool EditorState{ true };
	};
}