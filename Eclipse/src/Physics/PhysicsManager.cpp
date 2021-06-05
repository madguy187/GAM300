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

	void PhysicsManager::Simulate()
	{
		PxRigidActor* myactor;

		PxReal deltatime = 0.0f;
		Px_Scene->simulate(deltatime);
	}

	void PhysicsManager::Unload()
	{
		Px_Physics->release();
		Px_Foundation->release();
	}
}