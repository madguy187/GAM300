#pragma once
#include "Global.h"
#include "PxPhysicsAPI.h"
#include "Reflection/Meta.h"
#include "Reflection/RemTypeQual.h"

namespace Eclipse
{
	struct EC_Force
	{
		ECVec3 force{0,0,0};
		ForceMode mode{ForceMode::Default};
	};
	struct RigidBodyComponent
	{
		META_DATA(RigidBodyComponent);
		std::vector<EC_Force> forces;
		ECVec3 velocity{ 0,0,0 };
		ECVec3 Angvelocity{ 0,0.5f,0 };
		float MaxVelocity{10.0f};
		float mass{1.0f};
		float drag{0.0f};
		float angdrag{0.0f};
		bool _Kinematic{ false };
		bool enableGravity{ false };
		bool enableRotation{ false };
	};
}