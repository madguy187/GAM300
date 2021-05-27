#pragma once
#include "ECS/World.h"
#include "Mono/Manager/MonoManager.h"

namespace Eclipse
{
	class Engine
	{
	public:
		World world;
		MonoManager mono;

		void Init();
		void Run();
	};
}