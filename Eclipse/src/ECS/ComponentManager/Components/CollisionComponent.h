#pragma once
#include "Global.h"
#include "PxPhysicsAPI.h"
namespace Eclipse
{
	struct EC_Shape
	{
		PxShapeType shape{PxShapeType::Px_CUBE};
		float hx{0.5f};
		float hy{0.5f};
		float hz{0.5};
		float radius{1};
		float hheight{1};
	};

	struct CollisionComponent
	{
		META_DATA(CollisionComponent);
		bool created{false};
		std::vector<Entity> Collided;
		EC_Shape shape;
	};
}