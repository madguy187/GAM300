#pragma once
#include "ECS/World.h"
#include "Time/Clock.h"
#include "Vec.h" /*Please remove after graphics cleanup -Rachel*/
#include "Graphics/Camera/CameraManager.h"
#include "Graphics/Debugging/DebugRenderingManager.h"

namespace Eclipse
{
	class Engine
	{
	public:
		World world;
		CameraManager gCamera;
		GraphicsManager gGraphics;
		Clock Game_Clock;
		DebugRenderingManager gDebugManager;

		void Init();
		void Run();
	};
}