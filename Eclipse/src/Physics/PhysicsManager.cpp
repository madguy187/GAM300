#include "pch.h"
#include "PhysicsManager.h"
namespace Eclipse
{
	void PhysicsManager::Init()
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

	void PhysicsManager::InitActor(Entity ent)
	{
		auto& transform = engine->world.GetComponent<TransformComponent>(ent);

		if (Px_Actors[ent] != nullptr)
			return;

		PxVec3 temptrans;
		temptrans.x = transform.position.getX();
		temptrans.y = transform.position.getY();
		temptrans.z = transform.position.getZ();

		if (engine->world.CheckComponent<RigidBodyComponent>(ent))
		{
			auto& rigidbody = engine->world.GetComponent<RigidBodyComponent>(ent);
			if(rigidbody._Static)
				Px_Actors[ent] = Px_Physics->createRigidStatic(PxTransform(temptrans));
			else
				Px_Actors[ent] = Px_Physics->createRigidDynamic(PxTransform(temptrans));
		}
		else
			Px_Actors[ent] = Px_Physics->createRigidStatic(PxTransform(temptrans));

		AddActorToScene(ent);
	}

	void PhysicsManager::ChangeRigidStatic(Entity ent)
	{
		auto& rigid = engine->world.GetComponent<RigidBodyComponent>(ent);
		auto& transform = engine->world.GetComponent<TransformComponent>(ent);

		if(rigid.inScene)
			RemoveActorFromScene(ent);
		PxShape** shapes = new PxShape*[dynamic_cast<PxRigidActor*>(Px_Actors[ent])->getNbShapes()];

		dynamic_cast<PxRigidActor*>(Px_Actors[ent])->getShapes(shapes, dynamic_cast<PxRigidActor*>(Px_Actors[ent])->getNbShapes());
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

		Px_Actors[ent]->release();
		Px_Actors[ent] = temp;
		AddActorToScene(ent);
	}

	void PhysicsManager::ChangeStaticRigid(Entity ent)
	{
		auto& rigid = engine->world.GetComponent<RigidBodyComponent>(ent);
		auto& transform = engine->world.GetComponent<TransformComponent>(ent);

		if (rigid.inScene)
			RemoveActorFromScene(ent);
		PxShape** shapes = new PxShape * [dynamic_cast<PxRigidActor*>(Px_Actors[ent])->getNbShapes()];

		dynamic_cast<PxRigidActor*>(Px_Actors[ent])->getShapes(shapes, dynamic_cast<PxRigidActor*>(Px_Actors[ent])->getNbShapes());
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

		Px_Actors[ent]->release();
		Px_Actors[ent] = temp;
		AddActorToScene(ent);
	}

	void PhysicsManager::AttachBoxToActor(Entity ent, float hx, float hy, float hz)
	{
		if (Px_Actors[ent] == nullptr)
			return;

		PxMaterial* tempmat = Px_Physics->createMaterial(0.5, 0.5, 0.1);
		if (!tempmat)
		{
			std::cout << "creatematerial failed" << std::endl;
			return;
		}
		PxShape* tempshape = PxRigidActorExt::createExclusiveShape(*dynamic_cast<PxRigidActor*>(Px_Actors[ent]), PxBoxGeometry{ hx,hy,hz }, *tempmat);
	}

	void PhysicsManager::AttachSphereToActor(Entity ent, float radius)
	{
		if (Px_Actors[ent] == nullptr)
			return;

		PxMaterial* tempmat = Px_Physics->createMaterial(0.5, 0.5, 0.1);
		if (!tempmat)
		{
			std::cout << "creatematerial failed" << std::endl;
			return;
		}
		PxShape* tempshape = PxRigidActorExt::createExclusiveShape(*dynamic_cast<PxRigidActor*>(Px_Actors[ent]), PxSphereGeometry{ radius }, *tempmat);
	}

	void PhysicsManager::AttachCapsuleToActor(Entity ent, float radius,float halfheight)
	{
		if (Px_Actors[ent] == nullptr)
			return;

		PxMaterial* tempmat = Px_Physics->createMaterial(0.5, 0.5, 0.1);
		if (!tempmat)
		{
			std::cout << "creatematerial failed" << std::endl;
			return;
		}

		PxShape* tempshape = PxRigidActorExt::createExclusiveShape(*dynamic_cast<PxRigidActor*>(Px_Actors[ent]), PxCapsuleGeometry{ radius,halfheight }, *tempmat);
	}
	
	void PhysicsManager::AddActorToScene(Entity ent)
	{
		auto& rigid = engine->world.GetComponent<RigidBodyComponent>(ent);

		Px_Scene->addActor(*Px_Actors[ent]);
		rigid.inScene = true;
	}

	void PhysicsManager::RemoveActorFromScene(Entity ent)
	{
		auto& rigid = engine->world.GetComponent<RigidBodyComponent>(ent);

		Px_Scene->removeActor(*Px_Actors[ent]);
		rigid.inScene = false;
	}

	void PhysicsManager::UpdateActor(Entity ent)
	{
		auto& rigid = engine->world.GetComponent<RigidBodyComponent>(ent);
		auto& transform = engine->world.GetComponent<TransformComponent>(ent);
		if (rigid._Static)
		{
			PxVec3 temptrans;
			temptrans.x = transform.position.x;
			temptrans.y = transform.position.y;
			temptrans.z = transform.position.z;

			dynamic_cast<PxRigidStatic*>(Px_Actors[ent])->setGlobalPose(PxTransform{ temptrans });
			return;
		}
		if (Px_Actors[ent] != nullptr)
		{
			PxVec3 tempforce{ 0,0,0 };
			PxVec3 temptrans{0,0,0};
			PxQuat temprot{0,0,0,0};

			tempforce.x = rigid.forces.getX();
			tempforce.y = rigid.forces.getY();
			tempforce.z = rigid.forces.getZ();

			temptrans.x = transform.position.x;
			temptrans.y = transform.position.y;
			temptrans.z = transform.position.z;

			temprot.x = sinf(transform.rotation.getX() / 2);
			temprot.y = sinf(transform.rotation.getY() / 2);
			temprot.z = sinf(transform.rotation.getZ() / 2);
	
			static_cast<PxRigidDynamic*>(Px_Actors[ent])->setGlobalPose(PxTransform{ temptrans,temprot });
			static_cast<PxRigidDynamic*>(Px_Actors[ent])->setForceAndTorque(tempforce, { 0,0,0 });
			static_cast<PxRigidDynamic*>(Px_Actors[ent])->setMass(rigid.mass);
			static_cast<PxRigidDynamic*>(Px_Actors[ent])->setActorFlag(PxActorFlag::eDISABLE_GRAVITY,rigid.enableGravity ? false : true);
			static_cast<PxRigidDynamic*>(Px_Actors[ent])->setAngularVelocity(PxVec3{ 0,0,5 });
			static_cast<PxRigidDynamic*>(Px_Actors[ent])->setAngularDamping(0.f);
		}
	}

	void PhysicsManager::GetActorPosition(Entity ent)
	{
		auto& rigid = engine->world.GetComponent<RigidBodyComponent>(ent);
		if (rigid._Static)
			return;
		auto& transform = engine->world.GetComponent<TransformComponent>(ent);

		PxTransform temp = static_cast<PxRigidDynamic*>(Px_Actors[ent])->getGlobalPose();
		transform.position.setX(temp.p.x);
		transform.position.setY(temp.p.y);
		transform.position.setZ(temp.p.z);
		
		transform.rotation.setX(asinf(temp.q.x) * 2);
		transform.rotation.setY(asinf(temp.q.y) * 2);
		transform.rotation.setZ(asinf(temp.q.z) * 2);
	}

	void PhysicsManager::Simulate()
	{
		Px_Scene->simulate(engine->Game_Clock.get_fixedDeltaTime());
		Px_Scene->fetchResults(true);
	}

	void PhysicsManager::Unload()
	{
		Px_Cooking->release();
		Px_Physics->release();
		Px_Foundation->release();
	}
}