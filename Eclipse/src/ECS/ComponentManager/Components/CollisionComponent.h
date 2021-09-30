#pragma once
#include "Global.h"
#include "PxPhysicsAPI.h"
namespace Eclipse
{
	struct EC_Shape
	{
		PxShapeType shape;
		float hx;
		float hy;
		float hz;
		float radius;
		float hheight;
	};

	struct CollisionComponent
	{
		std::vector<Entity> Collided;
		EC_Shape shape;
	};
}