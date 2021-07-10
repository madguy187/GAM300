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
		{
			std::cout << "PxCreateFoundation failed!" << std::endl;
			return;
		}

		bool recordMemoryAllocations = true;

		//PxPvd* mPvd = PxCreatePvd(*Px_Foundation);
		//PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate(PVD_HOST, 5425, 10);
		//mPvd->connect(*transport, PxPvdInstrumentationFlag::eALL);

		Px_Physics = PxCreatePhysics(PX_PHYSICS_VERSION, *Px_Foundation,
			PxTolerancesScale(), recordMemoryAllocations);
		if (!Px_Physics)
		{
			std::cout << "PxCreatePhysics failed!" << std::endl;
			return;
		}

		Px_Cooking = PxCreateCooking(PX_PHYSICS_VERSION, *Px_Foundation, PxCookingParams(PxTolerancesScale()));
		if (!Px_Cooking)
		{
			std::cout << "PxCreateCooking failed!" << std::endl;
			return;
		}

		PxSceneDesc sceneDesc(Px_Physics->getTolerancesScale());
		sceneDesc.gravity = PxVec3(0.0f, -9.81f, 0.0f);

		if (!sceneDesc.cpuDispatcher)
		{
			PxDefaultCpuDispatcher* mCpuDispatcher = PxDefaultCpuDispatcherCreate(2);
			if (!mCpuDispatcher)
				std::cout << "PxDefaultCpuDispatcherCreate failed!" << std::endl;
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

	void PhysicsManager::CreateActor(Entity ent, bool is_static)
	{
		auto& transform = engine->world.GetComponent<TransformComponent>(ent);
		if (RigidObjects.find(ent) != RigidObjects.end() || StaticObjects.find(ent) != StaticObjects.end())
			return;

		PxVec3 temptrans;
		temptrans.x = transform.position.x;
		temptrans.y = transform.position.y;
		temptrans.z = transform.position.z;
		if (is_static)
		{
			PxRigidStatic* temp = Px_Physics->createRigidStatic(PxTransform(temptrans));
			StaticObjects.insert(std::make_pair(ent, temp));
		}
		else
		{
			PxRigidDynamic* temp = Px_Physics->createRigidDynamic(PxTransform(temptrans));
			RigidObjects.insert(std::make_pair(ent, temp));
		}
	}

	void PhysicsManager::ChangeRigidStatic(Entity ent)
	{
		auto& rigid = engine->world.GetComponent<RigidBodyComponent>(ent);
		auto& transform = engine->world.GetComponent<TransformComponent>(ent);
		auto it = StaticObjects.find(ent);
		auto it2 = RigidObjects.find(ent);
		if (rigid._Static && it != StaticObjects.end())
			return;
		else if (!rigid._Static && it2 != RigidObjects.end())
			return;

		if (rigid._Static && it2 != RigidObjects.end())
		{
			if(rigid.inScene)
				RemoveActorFromScene(ent);
			PxShape** shapes = new PxShape*[RigidObjects[ent]->getNbShapes()];

			RigidObjects[ent]->getShapes(shapes, RigidObjects[ent]->getNbShapes());
			PxVec3 temptrans;
			temptrans.x = transform.position.x;
			temptrans.y = transform.position.y;
			temptrans.z = transform.position.z;

			PxRigidStatic* temp = Px_Physics->createRigidStatic(PxTransform(temptrans));


			PxShape** shapeit = shapes;
			while (shapeit != nullptr)
			{
				temp->attachShape(**shapeit);
				shapeit++;
			}

			RigidObjects[ent]->release();
			RigidObjects.erase(ent);
			StaticObjects.insert(std::make_pair(ent, temp));
		}
		if (!rigid._Static && it != StaticObjects.end())
		{
			if (rigid.inScene)
				RemoveActorFromScene(ent);

			PxShape** shapes = new PxShape * [StaticObjects[ent]->getNbShapes()];

			StaticObjects[ent]->getShapes(shapes, StaticObjects[ent]->getNbShapes());
			PxVec3 temptrans;
			temptrans.x = transform.position.x;
			temptrans.y = transform.position.y;
			temptrans.z = transform.position.z;

			PxRigidDynamic* temp = Px_Physics->createRigidDynamic(PxTransform(temptrans));


			PxShape** shapeit = shapes;
			while (shapeit != nullptr)
			{
				temp->attachShape(**shapeit);
				shapeit++;
			}

			StaticObjects[ent]->release();
			StaticObjects.erase(ent);
			RigidObjects.insert(std::make_pair(ent, temp));
		}
	}

	void PhysicsManager::AttachBoxToActor(Entity ent, float hx, float hy, float hz)
	{
		if (StaticObjects.find(ent) == StaticObjects.end() || RigidObjects.find(ent) == RigidObjects.end())
			return;

		PxMaterial* tempmat = Px_Physics->createMaterial(0.5, 0.5, 0.1);
		if (!tempmat)
		{
			std::cout << "creatematerial failed" << std::endl;
			return;
		}

		if (StaticObjects.find(ent) != StaticObjects.end())
		{
			PxRigidStatic* temp = StaticObjects[ent];
			PxShape* tempshape = PxRigidActorExt::createExclusiveShape(*temp, PxBoxGeometry{ hx,hy,hz }, *tempmat);
			temp->attachShape(*tempshape);

		}

		if (RigidObjects.find(ent) != RigidObjects.end())
		{
			PxRigidDynamic* temp = RigidObjects[ent];
			PxShape* tempshape = PxRigidActorExt::createExclusiveShape(*temp, PxBoxGeometry{ hx,hy,hz }, *tempmat);
			temp->attachShape(*tempshape);
		}
	}

	void PhysicsManager::AttachSphereToActor(Entity ent, float radius)
	{
		if (StaticObjects.find(ent) == StaticObjects.end() || RigidObjects.find(ent) == RigidObjects.end())
			return;

		PxMaterial* tempmat = Px_Physics->createMaterial(0.5, 0.5, 0.1);
		if (!tempmat)
		{
			std::cout << "creatematerial failed" << std::endl;
			return;
		}

		if (StaticObjects.find(ent) != StaticObjects.end())
		{
			PxRigidStatic* temp = StaticObjects[ent];
			PxShape* tempshape = PxRigidActorExt::createExclusiveShape(*temp, PxSphereGeometry{ radius }, *tempmat);
			temp->attachShape(*tempshape);

		}

		if (RigidObjects.find(ent) != RigidObjects.end())
		{
			PxRigidDynamic* temp = RigidObjects[ent];
			PxShape* tempshape = PxRigidActorExt::createExclusiveShape(*temp, PxSphereGeometry{ radius }, *tempmat);
			temp->attachShape(*tempshape);
		}
	}

	void PhysicsManager::AttachCapsuleToActor(Entity ent, float radius,float halfheight)
	{
		if (StaticObjects.find(ent) == StaticObjects.end() || RigidObjects.find(ent) == RigidObjects.end())
			return;

		PxMaterial* tempmat = Px_Physics->createMaterial(0.5, 0.5, 0.1);
		if (!tempmat)
		{
			std::cout << "creatematerial failed" << std::endl;
			return;
		}

		if (StaticObjects.find(ent) != StaticObjects.end())
		{
			PxRigidStatic* temp = StaticObjects[ent];
			PxShape* tempshape = PxRigidActorExt::createExclusiveShape(*temp, PxCapsuleGeometry{ radius,halfheight }, *tempmat);
			temp->attachShape(*tempshape);
		}

		if (RigidObjects.find(ent) != RigidObjects.end())
		{
			PxRigidDynamic* temp = RigidObjects[ent];
			PxShape* tempshape = PxRigidActorExt::createExclusiveShape(*temp, PxCapsuleGeometry{ radius,halfheight }, *tempmat);
			temp->attachShape(*tempshape);
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

	void PhysicsManager::UpdateActor(Entity ent)
	{
		auto& rigid = engine->world.GetComponent<RigidBodyComponent>(ent);
		if (rigid._Static)
			return;

		if (RigidObjects.find(ent) != RigidObjects.end())
		{
			PxVec3 temp;
			temp.x =rigid.forces.getX();
			temp.y = rigid.forces.getY();
			temp.z = rigid.forces.getZ();
			RigidObjects[ent]->setForceAndTorque(temp, { 0,0,0 });
			RigidObjects[ent]->setMass(rigid.mass);
			RigidObjects[ent]->setActorFlag(PxActorFlag::eDISABLE_GRAVITY,rigid.enableGravity ? false : true);
		}
	}

	void PhysicsManager::GetActorPosition(Entity ent)
	{
		auto& rigid = engine->world.GetComponent<RigidBodyComponent>(ent);
	}

	void PhysicsManager::Simulate()
	{
		Px_Scene->simulate(engine->Game_Clock.get_fixedDeltaTime());
	}

	void PhysicsManager::Unload()
	{
		Px_Cooking->release();
		Px_Physics->release();
		Px_Foundation->release();
	}
}