#include "pch.h"
#include "AIManager.h"
void AIManager::patrol(Entity ent)
{
	auto& AI = engine->world.GetComponent<AIComponent>(ent);
	if (!AI.patrolling || AI.waypoints.empty())
		return;
	auto& transform = engine->world.GetComponent<TransformComponent>(ent);
	auto& targettransform = engine->world.GetComponent<TransformComponent>(AI.waypoints[AI.target]);
	auto& rigidbody = engine->world.GetComponent<RigidBodyComponent>(ent);

	ECVec3 direction = targettransform.position - transform.position;
	VectorNormalize<float, 3>(rigidbody.forces, direction);
	rigidbody.forces *= AI.PatrolSpeed;


	float distance = fabs(sqrtf(
		((targettransform.position.getX() - transform.position.getX()) * (targettransform.position.getX() - transform.position.getX())) +
		((targettransform.position.getY() - transform.position.getY()) * (targettransform.position.getY() - transform.position.getY())) +
		((targettransform.position.getZ() - transform.position.getZ()) * (targettransform.position.getZ() - transform.position.getZ()))));


	if (distance <= AI.MinDisttoChange)
	{
		if (AI.target >= AI.waypoints.size() - 1)
			AI.target = 0;
		else
			AI.target++;
	}

}

void AIManager::AddWaypoint(Entity AItoaddto, Entity waypointent)
{
	auto& AI = engine->world.GetComponent<AIComponent>(AItoaddto);
	AI.waypoints.push_back(waypointent);
}

void AIManager::AddTargetPointEntity(Entity ent)
{
	TargetPoints.push_back(ent);
}

const std::vector<Entity>& AIManager::GetTargetPoints()
{
	return TargetPoints;
}
