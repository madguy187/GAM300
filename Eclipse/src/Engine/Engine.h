#pragma once
#include "ECS/World.h"

namespace Eclipse
{
	class Engine
	{
	public:
		World world;

		void Init();
		void Run();
	};
}