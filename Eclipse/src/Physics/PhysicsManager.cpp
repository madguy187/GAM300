#include "pch.h"
#include "PhysicsManager.h"
namespace Eclipse
{
	void PhysicsManager::Load()
	{
		static PxDefaultErrorCallback gDefaultErrorCallback;
		static PxDefaultAllocator gDefaultAllocatorCallback;

		Px_Foundation = PxCreateFoundation(PX_PHYSICS_VERSION, gDefaultAllocatorCallback, gDefaultErrorCallback);
		if (!Px_Foundation)
			std::cout << "PxCreateFoundation failed!" << std::endl;

		bool recordMemoryAllocations = true;

		//PxPvd* mPvd = PxCreatePvd(*Px_Foundation);
		//PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate(PVD_HOST, 5425, 10);
		//mPvd->connect(*transport, PxPvdInstrumentationFlag::eALL);
	
		Px_Physics = PxCreatePhysics(PX_PHYSICS_VERSION, *Px_Foundation,
			PxTolerancesScale(), recordMemoryAllocations);
		if (!Px_Physics)
			std::cout << "PxCreatePhysics failed!" << std::endl;

		Px_Cooking = PxCreateCooking(PX_PHYSICS_VERSION, *Px_Foundation, PxCookingParams(PxTolerancesScale()));
		if (!Px_Cooking)
			std::cout << "PxCreateCooking failed!" << std::endl;

		PxSceneDesc sceneDesc(Px_Physics->getTolerancesScale());
		sceneDesc.gravity = PxVec3(0.0f, -9.81f, 0.0f);
		
		if (!sceneDesc.cpuDispatcher)
		{
			PxDefaultCpuDispatcher* mCpuDispatcher = PxDefaultCpuDispatcherCreate(2);
			if (!mCpuDispatcher)
				std::cout << "PxDefaultCpuDispatcherCreate failed!" <<  std::endl;
			sceneDesc.cpuDispatcher = mCpuDispatcher;
		}
		if (!sceneDesc.filterShader)
			sceneDesc.filterShader = PxDefaultSimulationFilterShader;

//#ifdef PX_WINDOWS
//		if (!sceneDesc.gpuDispatcher && mCudaContextManager)
//		{
//			sceneDesc.gpuDispatcher = mCudaContextManager->getGpuDispatcher();
//		}
//#endif
		Px_Scene = Px_Physics->createScene(sceneDesc);
		if (!Px_Scene)
			std::cout << "createScene failed!" << std::endl;


	}

	void PhysicsManager::CreateActor(Entity ent,bool is_static)
	{
		if (RigidObjects.find(ent) != RigidObjects.end() || StaticObjects.find(ent) != StaticObjects.end())
			return;

		if (is_static)
		{
			PxRigidStatic* temp;
		}
		else
		{
			PxRigidDynamic* temp;
		}
	}

	void PhysicsManager::AddActorToScene(Entity ent)
	{
		auto& rigid = engine->world.GetComponent<RigidBodyComponent>(ent);
		if (rigid._Static)
		{
			if(StaticObjects.find(ent) != StaticObjects.end())
			{
				Px_Scene->addActor(*(StaticObjects[ent]));
				rigid.inScene = true;
			}
		}
		else
		{
			if (RigidObjects.find(ent) != RigidObjects.end())
			{
				Px_Scene->addActor(*(RigidObjects[ent]));
				rigid.inScene = true;
			}
		}
	}

	void PhysicsManager::RemoveActorFromScene(Entity ent)
	{
		auto& rigid = engine->world.GetComponent<RigidBodyComponent>(ent);
		if (rigid._Static)
		{
			if (StaticObjects.find(ent) != StaticObjects.end())
			{
				Px_Scene->removeActor(*(StaticObjects[ent]));
				rigid.inScene = false;
			}
		}
		else
		{
			if (RigidObjects.find(ent) != RigidObjects.end())
			{
				Px_Scene->removeActor(*(RigidObjects[ent]));
				rigid.inScene = false;
			}
		}
	}

	void PhysicsManager::Simulate()
	{
		Px_Scene->simulate(engine->Game_Clock.get_fixedDeltaTime());
	}

	void PhysicsManager::Unload()
	{
		Px_Physics->release();
		Px_Foundation->release();
	}
}