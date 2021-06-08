#pragma once
#include "Global.h"
#include "PxPhysicsAPI.h"
namespace Eclipse
{
	struct RigidBodyComponent
	{
		bool _Static;
		bool _Kinematic;
		bool enableGravity;
		bool enableRotation;

		float mass;
		float drag;
		float angdrag;

		ECVec3 velocity;
		ECVec3 forces;
	};
}