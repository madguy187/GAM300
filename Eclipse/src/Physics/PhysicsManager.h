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
		std::array<physx::PxActor*, MAX_ENTITY> Px_Actors;
	public:
		void Load();
		void Simulate();
		void Unload();

		void AttachBoxToActor(Entity ent,float halfx, float halfy, float halfz);
		void AttachSphereToActor(Entity ent,float radius);
		void AttachCapsuleToActor(Entity ent, float radius, float halfheight);
		void CreateActor(Entity ent, bool is_static);
		void UpdateActor(Entity ent);
		void GetActorPosition(Entity ent);
		void SetActorPosition(Entity ent);
		void ChangeRigidStatic(Entity ent);
		void AddActorToScene(Entity ent);
		void RemoveActorFromScene(Entity ent);
	};
}