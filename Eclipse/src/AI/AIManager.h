#pragma once
#include "ECS/ComponentManager/Components/AIComponent.h"
class AIManager
{
public:
	void patrol(Entity ent);
	void AddWaypoint(Entity AItoaddto, Entity waypointent);
};