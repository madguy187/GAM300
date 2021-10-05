#pragma once
#include "Global.h"
namespace Eclipse
{
	struct AIComponent
	{
		META_DATA(AIComponent);
		std::vector<Entity> waypoints;
		size_t target{ 0 };
		float MinDisttoChange{ 10.0f };
		float PatrolSpeed{ 20.0f };
		bool patrolling{true};
	};
}