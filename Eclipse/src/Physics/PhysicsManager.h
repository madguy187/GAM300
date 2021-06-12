#pragma once
#include "Global.h"
#include "PxPhysicsAPI.h"
#include "ECS/ComponentManager/Components/RigidBodyComponent.h"
namespace Eclipse
{
	using namespace physx;
	class PhysicsManager
	{
		PxFoundation* Px_Foundation;
		PxPhysics* Px_Physics;
		PxCooking* Px_Cooking;
		PxScene* Px_Scene;
		std::map<Entity,PxRigidDynamic*> RigidObjects;
		std::map<Entity,PxRigidStatic*> StaticObjects;
	public:
		void Load();
		void Simulate();
		void Unload();

		void AttachBoxToActor(Entity ent,float halfx, float halfy, float halfz);
		void AttachSphereToActor(Entity ent,float radius);
		void AttachCapsuleToActor(Entity ent, float radius, float halfheight);
		void CreateActor(Entity ent, bool is_static);
		void AddActorToScene(Entity ent);
		void RemoveActorFromScene(Entity ent);
	};
}