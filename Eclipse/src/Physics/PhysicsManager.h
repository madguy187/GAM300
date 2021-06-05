#pragma once
#include "Global.h"
#include "PxPhysicsAPI.h"
namespace Eclipse
{
	using namespace physx;
	class PhysicsManager
	{
		PxFoundation* Px_Foundation;
		PxPhysics* Px_Physics;
		PxCooking* Px_Cooking;
		PxScene* Px_Scene;
	public:
		void Load();
		void Simulate();
		void Unload();
	};
}