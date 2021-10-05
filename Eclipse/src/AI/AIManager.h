#pragma once
#include "ECS/ComponentManager/Components/AIComponent.h"
class AIManager
{
public:
	void patrol(Entity ent);
	void AddWaypoint(Entity AItoaddto, Entity waypointent);
	void AddTargetPointEntity(Entity ent);
	void RemoveDeletedWaypoints(Entity ent,Entity deleteent);
	const std::vector<Entity>& GetTargetPoints();
private:
	std::vector<Entity> TargetPoints;
};