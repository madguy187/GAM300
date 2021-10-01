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
		//sceneDesc.filterShader = contactReportFilterShader;
		sceneDesc.simulationEventCallback = &Px_ContactReportCallback;
		if (!sceneDesc.cpuDispatcher)
		{
			Px_Dispatcher = PxDefaultCpuDispatcherCreate(2);
			if (!Px_Dispatcher)
				std::cout << "PxDefaultCpuDispatcherCreate failed!" << std::endl;
			sceneDesc.cpuDispatcher = Px_Dispatcher;
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

		if (Px_Actors[ent].actor != nullptr)
			return;

		PxVec3 temptrans;
		temptrans.x = transform.position.getX();
		temptrans.y = transform.position.getY();
		temptrans.z = transform.position.getZ();

		if (engine->world.CheckComponent<RigidBodyComponent>(ent))
		{
			Px_Actors[ent].actor = Px_Physics->createRigidDynamic(PxTransform(temptrans));
			Px_Actors[ent].type = ActorType::ACTOR_DYNAMIC;
		}
		else
		{
			Px_Actors[ent].actor = Px_Physics->createRigidStatic(PxTransform(temptrans));
			Px_Actors[ent].type = ActorType::ACTOR_STATIC;
		}

		Px_Actors[ent].actor->setName(std::to_string(ent).c_str());
		AddActorToScene(ent);
		AttachBoxToActor(ent, 2.5f, 2.5f, 2.5f);
	}

	void PhysicsManager::ChangeDynamicStatic(Entity ent)
	{
		auto& rigid = engine->world.GetComponent<RigidBodyComponent>(ent);
		auto& transform = engine->world.GetComponent<TransformComponent>(ent);

		if(rigid.inScene)
			RemoveActorFromScene(ent);
		PxShape** shapes = new PxShape*[dynamic_cast<PxRigidActor*>(Px_Actors[ent].actor)->getNbShapes()];

		dynamic_cast<PxRigidActor*>(Px_Actors[ent].actor)->getShapes(shapes, dynamic_cast<PxRigidActor*>(Px_Actors[ent].actor)->getNbShapes());
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

		Px_Actors[ent].actor->release();
		Px_Actors[ent].actor = temp;
		AddActorToScene(ent);
	}

	void PhysicsManager::ChangeStaticDynamic(Entity ent)
	{
		auto& rigid = engine->world.GetComponent<RigidBodyComponent>(ent);
		auto& transform = engine->world.GetComponent<TransformComponent>(ent);

		if (rigid.inScene)
			RemoveActorFromScene(ent);
		PxShape** shapes = new PxShape * [dynamic_cast<PxRigidActor*>(Px_Actors[ent].actor)->getNbShapes()];

		dynamic_cast<PxRigidActor*>(Px_Actors[ent].actor)->getShapes(shapes, dynamic_cast<PxRigidActor*>(Px_Actors[ent].actor)->getNbShapes());
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

		Px_Actors[ent].actor->release();
		Px_Actors[ent].actor = temp;
		AddActorToScene(ent);
	}

	void PhysicsManager::AttachBoxToActor(Entity ent, float hx, float hy, float hz)
	{
		if (Px_Actors[ent].actor == nullptr)
			return;

		PxMaterial* tempmat = Px_Physics->createMaterial(0.5f, 0.5f, 0.1f);
		if (!tempmat)
		{
			std::cout << "creatematerial failed" << std::endl;
			return;
		}
		PxShape* tempshape = PxRigidActorExt::createExclusiveShape(*static_cast<PxRigidActor*>(Px_Actors[ent].actor), PxBoxGeometry{ hx,hy,hz }, *tempmat);
	}

	void PhysicsManager::AttachSphereToActor(Entity ent, float radius)
	{
		if (Px_Actors[ent].actor == nullptr)
			return;

		PxMaterial* tempmat = Px_Physics->createMaterial(0.5f, 0.5f, 0.1f);
		if (!tempmat)
		{
			std::cout << "creatematerial failed" << std::endl;
			return;
		}
		PxShape* tempshape = PxRigidActorExt::createExclusiveShape(*static_cast<PxRigidActor*>(Px_Actors[ent].actor), PxSphereGeometry{ radius }, *tempmat);
	}

	void PhysicsManager::ChangeType(Entity ent)
	{
		if (engine->world.CheckComponent<RigidBodyComponent>(ent))
		{
			if (Px_Actors[ent].type == ActorType::ACTOR_STATIC)
				ChangeStaticDynamic(ent);
		}
		else
		{
			if (Px_Actors[ent].type == ActorType::ACTOR_DYNAMIC)
			{
				ChangeDynamicStatic(ent);
			}
		}
	}

	void PhysicsManager::AttachCapsuleToActor(Entity ent, float radius,float halfheight)
	{
		if (Px_Actors[ent].actor == nullptr)
			return;

		PxMaterial* tempmat = Px_Physics->createMaterial(0.5f, 0.5f, 0.1f);
		if (!tempmat)
		{
			std::cout << "creatematerial failed" << std::endl;
			return;
		}

		PxShape* tempshape = PxRigidActorExt::createExclusiveShape(*dynamic_cast<PxRigidActor*>(Px_Actors[ent].actor), PxCapsuleGeometry{ radius,halfheight }, *tempmat);
	}

	PxQuat PhysicsManager::AnglestoQuat(float degreeX, float degreeY, float degreeZ)
	{
		float yaw = static_cast<float>(degreeZ * (M_PI/180));
		float pitch = static_cast<float>(degreeY * (M_PI/180));
		float roll = static_cast<float>(degreeX * (M_PI/180));


		float cy = cosf(yaw * 0.5f);
		float sy = sinf(yaw * 0.5f);
		float cp = cosf(pitch * 0.5f);
		float sp = sinf(pitch * 0.5f);
		float cr = cosf(roll * 0.5f);
		float sr = sinf(roll* 0.5f);

		return PxQuat(sr * cp * cy - cr * sp * sy ,//X
					  cr * sp * cy + sr * cp * sy ,//Y
				      cr * cp * sy - sr * sp * cy ,//Z
			          cr * cp * cy + sr * sp * sy);//W
	}

	ECVec3 PhysicsManager::QuattoAngles(PxQuat quat)
	{
		ECVec3 temp{0,0,0};
		//x rotation
		float sinr_cosp = 2 * (quat.w * quat.x + quat.y * quat.z);
		float cosr_cosp = 1 - 2 * (quat.x * quat.x + quat.y * quat.y);
		
		temp.setX(static_cast<float>(std::atan2f(sinr_cosp, cosr_cosp) * (180.0f/M_PI)));

		//y rotation
		float sinp = 2 * (quat.w * quat.y - quat.z * quat.x);
		if (std::abs(sinp) >= 1)
			temp.setY(static_cast<float>(std::copysign(M_PI / 2, sinp) * (180.0/M_PI)));
		else
			temp.setY(static_cast<float>(std::asinf(sinp) * (180.0f/M_PI)));

		float siny_cosp = 2 * (quat.w * quat.z + quat.x * quat.y);
		float cosy_cosp = 1 - 2 * (quat.y * quat.y + quat.z * quat.z);

		temp.setZ(static_cast<float>(std::atan2f(siny_cosp, cosy_cosp) * (180.0f/M_PI)));

		return temp;
	}
	



	void PhysicsManager::AddActorToScene(Entity ent)
	{
		auto& rigid = engine->world.GetComponent<RigidBodyComponent>(ent);
		if (rigid.inScene)
			return;
		Px_Scene->addActor(*Px_Actors[ent].actor);
		rigid.inScene = true;
	}

	void PhysicsManager::RemoveActorFromScene(Entity ent)
	{
		auto& rigid = engine->world.GetComponent<RigidBodyComponent>(ent);

		Px_Scene->removeActor(*Px_Actors[ent].actor);
		rigid.inScene = false;
	}

	void PhysicsManager::UpdateActor(Entity ent)
	{
		auto& rigid = engine->world.GetComponent<RigidBodyComponent>(ent);
		auto& transform = engine->world.GetComponent<TransformComponent>(ent);
		if (!engine->world.CheckComponent<RigidBodyComponent>(ent))
		{
			//static object
			PxVec3 temptrans;
			temptrans.x = transform.position.x;
			temptrans.y = transform.position.y;
			temptrans.z = transform.position.z;

			static_cast<PxRigidStatic*>(Px_Actors[ent].actor)->setGlobalPose(PxTransform{ temptrans });
			return;
		}
		if (Px_Actors[ent].actor != nullptr)
		{
			PxVec3 tempforce{ 0,0,0 };
			PxVec3 tempangVelo{ 0,0,0 };
			PxVec3 temptrans{0,0,0};
			PxQuat temprot;

			tempforce.x = rigid.forces.getX();
			tempforce.y = rigid.forces.getY();
			tempforce.z = rigid.forces.getZ();

			tempangVelo.x = rigid.Angvelocity.getX();
			tempangVelo.y = rigid.Angvelocity.getY();
			tempangVelo.z = rigid.Angvelocity.getZ();


			temptrans.x = transform.position.x;
			temptrans.y = transform.position.y;
			temptrans.z = transform.position.z;

			temprot = AnglestoQuat(transform.rotation.getX(),transform.rotation.getY(),transform.rotation.getZ());
	
			static_cast<PxRigidDynamic*>(Px_Actors[ent].actor)->setGlobalPose(PxTransform{ temptrans,temprot});
			static_cast<PxRigidDynamic*>(Px_Actors[ent].actor)->setForceAndTorque(tempforce, { 0,0,0 });
			static_cast<PxRigidDynamic*>(Px_Actors[ent].actor)->setMass(rigid.mass);
			static_cast<PxRigidDynamic*>(Px_Actors[ent].actor)->setActorFlag(PxActorFlag::eDISABLE_GRAVITY,rigid.enableGravity ? false : true);
			static_cast<PxRigidDynamic*>(Px_Actors[ent].actor)->setAngularVelocity(tempangVelo);
			static_cast<PxRigidDynamic*>(Px_Actors[ent].actor)->setAngularDamping(0.f);
		}
	}

	void PhysicsManager::GetActorPosition(Entity ent)
	{
		auto& rigid = engine->world.GetComponent<RigidBodyComponent>(ent);
		auto& transform = engine->world.GetComponent<TransformComponent>(ent);

		PxTransform temp = static_cast<PxRigidDynamic*>(Px_Actors[ent].actor)->getGlobalPose();
		transform.position.setX(temp.p.x);
		transform.position.setY(temp.p.y);
		transform.position.setZ(temp.p.z);
		
		transform.rotation = QuattoAngles(temp.q);
		

		PxVec3 rb = static_cast<PxRigidBody*>(Px_Actors[ent].actor)->getLinearVelocity();
		rigid.velocity.setX(rb.x);
		rigid.velocity.setY(rb.y);
		rigid.velocity.setZ(rb.z);
	}

	void PhysicsManager::Simulate()
	{
		Px_Scene->simulate(engine->Game_Clock.get_fixedDeltaTime());
		Px_Scene->fetchResults(true);
	}

	void PhysicsManager::Unload()
	{
		Px_Scene->release();
		Px_Dispatcher->release();
		Px_Cooking->release();
		Px_Physics->release();
		Px_Foundation->release();
	}
}