#pragma once
#include "Global.h"
#include "PxPhysicsAPI.h"
namespace Eclipse
{
	struct RigidBodyComponent
	{
		bool _Static{false};
		bool _Kinematic{false};
		bool enableGravity{false};
		bool enableRotation{false};
		bool inScene{false};

		float mass{1.0f};
		float drag{0.0f};
		float angdrag{0.0f};

		ECVec3 velocity{0,0,0};
		ECVec3 Angvelocity{ 0,0,0 };
		ECVec3 forces{0,0,0};
	};
}