#pragma once
#include "Global.h"
#include "PxPhysicsAPI.h"
namespace Eclipse
{
	struct EC_Shape
	{
		PxShapeType shape{PxShapeType::Px_CUBE};
		float hx{2.5f};
		float hy{2.5f};
		float hz{2.5f};
		float radius{1};
		float hheight{1};
	};

	struct CollisionComponent
	{
		META_DATA(CollisionComponent);
		std::vector<Entity> Collided;
		EC_Shape shape;
		bool created{ false };
	};
}