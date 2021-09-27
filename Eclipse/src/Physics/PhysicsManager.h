#pragma once
#include "Global.h"
#include "PxPhysicsAPI.h"
#include "ECS/ComponentManager/Components/RigidBodyComponent.h"
namespace Eclipse
{
	using namespace physx;
	


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
			void onConstraintBreak(PxConstraintInfo* constraints, PxU32 count) { PX_UNUSED(constraints); PX_UNUSED(count); }
			void onWake(PxActor** actors, PxU32 count) { PX_UNUSED(actors); PX_UNUSED(count); }
			void onSleep(PxActor** actors, PxU32 count) { PX_UNUSED(actors); PX_UNUSED(count); }
			void onTrigger(PxTriggerPair* pairs, PxU32 count) { PX_UNUSED(pairs); PX_UNUSED(count); }
			void onAdvance(const PxRigidBody* const*, const PxTransform*, const PxU32) {}
			void onContact(const PxContactPairHeader& pairHeader, const PxContactPair* pairs, PxU32 nbPairs)
			{
				pairHeader.actors
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
		PxPhysics* Px_Physics;
		PxCooking* Px_Cooking;
		PxScene* Px_Scene;
		ContactReportCallback Px_ContactReportCallback;
		std::array<physx::PxActor*, MAX_ENTITY> Px_Actors;
		std::vector<std::pair<Entity, Entity>> CollisionPairs;
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
		void GetActorPosition(Entity ent);
		void ChangeRigidStatic(Entity ent);
		void ChangeStaticRigid(Entity ent);
		void AddActorToScene(Entity ent);
		void RemoveActorFromScene(Entity ent);
	};
}