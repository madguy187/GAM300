#pragma once

#include "PxPhysicsAPI.h"
#include "ECS/ComponentManager/Components/RigidBodyComponent.h"
#include "ECS/ComponentManager/Components/CollisionComponent.h"
namespace Eclipse
{
	using namespace physx;
	
	struct EC_Actor
	{
		PxActor* actor;
		ActorType type;
		bool InScene;
	};

	class PhysicsManager
	{
		PxFilterFlags contactReportFilterShader(PxFilterObjectAttributes attributes0, PxFilterData filterData0,
			PxFilterObjectAttributes attributes1, PxFilterData filterData1,
			PxPairFlags& pairFlags, const void* constantBlock, PxU32 constantBlockSize)
		{
			PX_UNUSED(attributes0);
			PX_UNUSED(attributes1);
			PX_UNUSED(filterData0);
			PX_UNUSED(filterData1);
			PX_UNUSED(constantBlockSize);
			PX_UNUSED(constantBlock);

			// all initial and persisting reports for everything, with per-point data
			pairFlags = PxPairFlag::eSOLVE_CONTACT | PxPairFlag::eDETECT_DISCRETE_CONTACT
				| PxPairFlag::eNOTIFY_TOUCH_FOUND
				| PxPairFlag::eNOTIFY_TOUCH_PERSISTS
				| PxPairFlag::eNOTIFY_CONTACT_POINTS
				| PxPairFlag::eMODIFY_CONTACTS;
			return PxFilterFlag::eDEFAULT;
		}

		class ContactReportCallback : public PxSimulationEventCallback
		{
		public:
			std::vector<std::pair<Entity, Entity>> CollisionPairs;

		private:
			void onConstraintBreak(PxConstraintInfo* constraints, PxU32 count) { PX_UNUSED(constraints); PX_UNUSED(count); }
			void onWake(PxActor** actors, PxU32 count) { PX_UNUSED(actors); PX_UNUSED(count); }
			void onSleep(PxActor** actors, PxU32 count) { PX_UNUSED(actors); PX_UNUSED(count); }
			void onTrigger(PxTriggerPair* pairs, PxU32 count) { PX_UNUSED(pairs); PX_UNUSED(count); }
			void onAdvance(const PxRigidBody* const*, const PxTransform*, const PxU32) {}
			void onContact(const PxContactPairHeader& pairHeader, const PxContactPair* pairs, PxU32 nbPairs)
			{
				PX_UNUSED(nbPairs);
				PX_UNUSED(pairs);
				Entity Collided1;
				Entity Collided2;
				std::stringstream ent1{ pairHeader.actors[0]->getName() };
				std::stringstream ent2{ pairHeader.actors[1]->getName() };

				ent1 >> Collided1;
				ent2 >> Collided2;

				CollisionPairs.push_back(std::make_pair(Collided1,Collided2));

				/*PX_UNUSED((pairHeader));
				std::vector<PxContactPairPoint> contactPoints;

				for (PxU32 i = 0; i < nbPairs; i++)
				{
					PxU32 contactCount = pairs[i].contactCount;
					if (contactCount)
					{
						contactPoints.resize(contactCount);
						pairs[i].extractContacts(&contactPoints[0], contactCount);

						for (PxU32 j = 0; j < contactCount; j++)
						{
							gContactPositions.push_back(contactPoints[j].position);
							gContactImpulses.push_back(contactPoints[j].impulse);
						}
					}
				}*/
			}
		};
		PxFoundation* Px_Foundation;
		PxDefaultCpuDispatcher* Px_Dispatcher;
		PxPhysics* Px_Physics;
		PxCooking* Px_Cooking;
		PxScene* Px_Scene;
		ContactReportCallback Px_ContactReportCallback;
		std::array<EC_Actor, MAX_ENTITY> Px_Actors;
	public:
		void Init();
		void Simulate();
		void Unload();

		void AttachBoxToActor(Entity ent,float halfx, float halfy, float halfz);
		void AttachSphereToActor(Entity ent,float radius);
		void AttachCapsuleToActor(Entity ent, float radius, float halfheight);

		PxQuat AnglestoQuat(float degreeX, float degreeY, float degreeZ);
		ECVec3 QuattoAngles(PxQuat quat);
		void InitActor(Entity ent);
		void UpdateActor(Entity ent);
		void CreateShape(Entity ent);
		void SetForce(Entity ent, ECVec3 force);
		void UpdateVariables(Entity ent);
		void UpdateShapes(Entity ent);
		void GetActorPosition(Entity ent);
		void ChangeDynamicStatic(Entity ent);
		void ChangeStaticDynamic(Entity ent);
		void AddActorToScene(Entity ent);
		void RemoveActorFromScene(Entity ent);
		void RemoveActor(Entity ent);
		void ChangeType(Entity ent);
		void CleanupScene();
	};
}