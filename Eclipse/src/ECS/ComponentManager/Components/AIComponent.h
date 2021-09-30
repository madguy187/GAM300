#pragma once
#include "Global.h"
namespace Eclipse
{
	struct AIComponent
	{
		std::vector<Entity> waypoints;
		Entity Target;
		bool patrolling;
	};
}