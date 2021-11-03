#include "pch.h"
#include "PhysicsManager.h"
#include "Global.h"

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
			//need to change to static but do dynamic first
			Px_Actors[ent].actor = Px_Physics->createRigidStatic(PxTransform(temptrans));
			Px_Actors[ent].type = ActorType::ACTOR_STATIC;
		}

		Px_Actors[ent].actor->setName(std::to_string(ent).c_str());
		AddActorToScene(ent);
	}

	void PhysicsManager::ChangeDynamicStatic(Entity ent)
	{
		auto& transform = engine->world.GetComponent<TransformComponent>(ent);
		auto& collision = engine->world.GetComponent<CollisionComponent>(ent);
		if(Px_Actors[ent].InScene)
			RemoveActorFromScene(ent);
		PxVec3 temptrans;
		temptrans.x = transform.position.getX();
		temptrans.y = transform.position.getY();
		temptrans.z = transform.position.getZ();
		PxRigidStatic* temp = Px_Physics->createRigidStatic(PxTransform(temptrans));
		PxU32 numofshapes = static_cast<PxRigidDynamic*>(Px_Actors[ent].actor)->getNbShapes();

		if (numofshapes > 0)
		{
			PxShape** shapes = new PxShape * [numofshapes];
			static_cast<PxRigidActor*>(Px_Actors[ent].actor)->getShapes(shapes, numofshapes);

			for (PxU32 i = 0; i < numofshapes; ++i)
			{
				PxMaterial* tempmat = Px_Physics->createMaterial(0.5f, 0.5f, 0.1f);
				if (!tempmat)
				{
					std::cout << "creatematerial failed" << std::endl;
					return;
				}
				
				switch (collision.shape.shape)
				{
				case PxShapeType::Px_CUBE :
					PxRigidActorExt::createExclusiveShape(*static_cast<PxRigidActor*>(temp), PxBoxGeometry{ collision.shape.hx,collision.shape.hy,collision.shape.hz }, *tempmat);
					break;
				case PxShapeType::Px_SPHERE :
					PxRigidActorExt::createExclusiveShape(*static_cast<PxRigidActor*>(temp), PxSphereGeometry{ collision.shape.radius }, *tempmat);
					break;
				case PxShapeType::Px_CAPSULE:
					PxRigidActorExt::createExclusiveShape(*static_cast<PxRigidActor*>(temp), PxCapsuleGeometry{ collision.shape.radius,collision.shape.hheight }, *tempmat);
					break;
				}
				
			}

		}
		Px_Actors[ent].actor->release();
		Px_Actors[ent].actor = temp;
		Px_Actors[ent].type = ActorType::ACTOR_STATIC;
		AddActorToScene(ent);
	}

	void PhysicsManager::ChangeStaticDynamic(Entity ent)
	{
		auto& transform = engine->world.GetComponent<TransformComponent>(ent);
		auto& collision = engine->world.GetComponent<CollisionComponent>(ent);

		if (Px_Actors[ent].InScene)
			RemoveActorFromScene(ent);
		PxU32 numofshapes = static_cast<PxRigidActor*>(Px_Actors[ent].actor)->getNbShapes();
		PxVec3 temptrans;
		temptrans.x = transform.position.getX();
		temptrans.y = transform.position.getY();
		temptrans.z = transform.position.getZ();
		PxRigidDynamic* temp = Px_Physics->createRigidDynamic(PxTransform(temptrans));
		if (numofshapes > 0)
		{
			PxShape** shapes = new PxShape * [numofshapes];
			static_cast<PxRigidActor*>(Px_Actors[ent].actor)->getShapes(shapes, numofshapes);
			for (PxU32 i = 0; i < numofshapes; ++i)
			{
				PxMaterial* tempmat = Px_Physics->createMaterial(0.5f, 0.5f, 0.1f);
				if (!tempmat)
				{
					std::cout << "creatematerial failed" << std::endl;
					return;
				}

				switch (collision.shape.shape)
				{
				case PxShapeType::Px_CUBE:
					PxRigidActorExt::createExclusiveShape(*static_cast<PxRigidActor*>(temp), PxBoxGeometry{ collision.shape.hx,collision.shape.hy,collision.shape.hz }, *tempmat);
					break;
				case PxShapeType::Px_SPHERE:
					PxRigidActorExt::createExclusiveShape(*static_cast<PxRigidActor*>(temp), PxSphereGeometry{ collision.shape.radius }, *tempmat);
					break;
				case PxShapeType::Px_CAPSULE:
					PxRigidActorExt::createExclusiveShape(*static_cast<PxRigidActor*>(temp), PxCapsuleGeometry{ collision.shape.radius,collision.shape.hheight }, *tempmat);
					break;
				}
			}
		}
		Px_Actors[ent].actor->release();
		Px_Actors[ent].actor = temp;
		Px_Actors[ent].type = ActorType::ACTOR_DYNAMIC;
		AddActorToScene(ent);
	}

	void PhysicsManager::AttachBoxToActor(Entity ent, float hx, float hy, float hz)
	{
		auto& collision = engine->world.GetComponent<CollisionComponent>(ent);
		if (Px_Actors[ent].actor == nullptr)
			return;

		PxMaterial* tempmat = Px_Physics->createMaterial(0.5f, 0.5f, 0.1f);
		if (!tempmat)
		{
			std::cout << "creatematerial failed" << std::endl;
			return;
		}
		PxRigidActorExt::createExclusiveShape(*static_cast<PxRigidActor*>(Px_Actors[ent].actor), PxBoxGeometry{ hx,hy,hz }, *tempmat);
		collision.created = true;
		collision.shape.hx = hx;
		collision.shape.hy = hy;
		collision.shape.hz = hz;
	}

	void PhysicsManager::AttachSphereToActor(Entity ent, float radius)
	{
		auto& collision = engine->world.GetComponent<CollisionComponent>(ent);
		if (Px_Actors[ent].actor == nullptr)
			return;

		PxMaterial* tempmat = Px_Physics->createMaterial(0.5f, 0.5f, 0.1f);
		if (!tempmat)
		{
			std::cout << "creatematerial failed" << std::endl;
			return;
		}
		PxRigidActorExt::createExclusiveShape(*static_cast<PxRigidActor*>(Px_Actors[ent].actor), PxSphereGeometry{ radius }, *tempmat);
		collision.created = true;
		collision.shape.radius = radius;
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

	bool PhysicsManager::Raycast(ECVec3 origin, ECVec3 dir, float dist, PxRaycastBuffer& hit)
	{
		PxVec3 _origin;
		PxVec3 _dir;

		_origin.x = origin.getX();
		_origin.y = origin.getY();
		_origin.z = origin.getZ();

		_dir.x = dir.getX();
		_dir.y = dir.getY();
		_dir.z = dir.getZ();

		return Px_Scene->raycast(_origin,_dir, dist, hit);
	}




	void PhysicsManager::AttachCapsuleToActor(Entity ent, float radius,float halfheight)
	{
		auto& collision = engine->world.GetComponent<CollisionComponent>(ent);
		if (Px_Actors[ent].actor == nullptr)
			return;

		PxMaterial* tempmat = Px_Physics->createMaterial(0.5f, 0.5f, 0.1f);
		if (!tempmat)
		{
			std::cout << "creatematerial failed" << std::endl;
			return;
		}
		PxRigidActorExt::createExclusiveShape(*static_cast<PxRigidActor*>(Px_Actors[ent].actor), PxCapsuleGeometry{ radius,halfheight }, *tempmat);
		collision.created = true;
		collision.shape.radius = radius;
		collision.shape.hheight = halfheight;
	}

	void PhysicsManager::CleanupScene()
	{

		for (auto entity : engine->editorManager->GetEntityListByConstRef())
		{
			engine->gPhysics.RemoveActor(entity);
		}
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
		if (Px_Actors[ent].InScene)
			return;
		Px_Scene->addActor(*Px_Actors[ent].actor);
		Px_Actors[ent].InScene = true;
	}

	void PhysicsManager::RemoveActorFromScene(Entity ent)
	{
		Px_Scene->removeActor(*Px_Actors[ent].actor);
		Px_Actors[ent].InScene = false;
	}

	void PhysicsManager::RemoveActor(Entity ent)
	{
		if (Px_Actors[ent].actor == nullptr)
			return;
		RemoveActorFromScene(ent);
		Px_Actors[ent].actor->release();
		Px_Actors[ent].actor = nullptr;
		Px_Actors[ent].type = ActorType::ACTOR_UNASSIGNED;
	}

	void PhysicsManager::UpdateActor(Entity ent)
	{
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
		auto& rigid = engine->world.GetComponent<RigidBodyComponent>(ent);
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
			static_cast<PxRigidDynamic*>(Px_Actors[ent].actor)->addForce(tempforce,PxForceMode::eIMPULSE);
			static_cast<PxRigidDynamic*>(Px_Actors[ent].actor)->setMaxLinearVelocity(static_cast<PxReal>(rigid.MaxVelocity));
			static_cast<PxRigidDynamic*>(Px_Actors[ent].actor)->setMass(rigid.mass);
			static_cast<PxRigidDynamic*>(Px_Actors[ent].actor)->setActorFlag(PxActorFlag::eDISABLE_GRAVITY,rigid.enableGravity ? false : true);
			static_cast<PxRigidDynamic*>(Px_Actors[ent].actor)->setAngularVelocity(tempangVelo);
			static_cast<PxRigidDynamic*>(Px_Actors[ent].actor)->setAngularDamping(0.f);
		}
	}

	void PhysicsManager::UpdateShapes(Entity ent)
	{
		auto& collision = engine->world.GetComponent<CollisionComponent>(ent);
		if (!collision.created)
			return;
		PxU32 numofshapes = static_cast<PxRigidDynamic*>(Px_Actors[ent].actor)->getNbShapes();
		if (numofshapes > 0)
		{
			PxShape** shapes = new PxShape * [numofshapes];
			static_cast<PxRigidDynamic*>(Px_Actors[ent].actor)->getShapes(shapes, numofshapes);
			for (PxU32 i = 0; i < numofshapes; ++i)
			{
				switch (collision.shape.shape)
				{
				case PxShapeType::Px_CUBE:
					if (shapes[i]->getGeometryType() == PxGeometryType::eBOX)
					{
						shapes[i]->setGeometry(PxBoxGeometry{ collision.shape.hx,collision.shape.hy,collision.shape.hz });
					}
					else
					{
						//shape don't tally
						static_cast<PxRigidActor*>(Px_Actors[ent].actor)->detachShape(*shapes[i]);
						AttachBoxToActor(ent, collision.shape.hx, collision.shape.hy, collision.shape.hz);
					}
					break;
				case PxShapeType::Px_SPHERE:
					if (shapes[i]->getGeometryType() == PxGeometryType::eSPHERE)
					{
						shapes[i]->setGeometry(PxSphereGeometry{ collision.shape.radius });
					}
					else
					{
						static_cast<PxRigidActor*>(Px_Actors[ent].actor)->detachShape(*shapes[i]);
						AttachSphereToActor(ent, collision.shape.radius);
					}
					break;
				case PxShapeType::Px_CAPSULE:
					if (shapes[i]->getGeometryType() == PxGeometryType::eCAPSULE)
					{
						shapes[i]->setGeometry(PxCapsuleGeometry{ collision.shape.radius, collision.shape.hheight });
					}
					else
					{
						static_cast<PxRigidActor*>(Px_Actors[ent].actor)->detachShape(*shapes[i]);
						AttachCapsuleToActor(ent, collision.shape.radius, collision.shape.hheight);
					}
					break;
				}
			}
			delete[] shapes;
		}
	}

	void PhysicsManager::ChangeShape(Entity ent,PxShapeType shape)
	{
		auto& collision = engine->world.GetComponent<CollisionComponent>(ent);
		switch (shape)
		{
		case PxShapeType::Px_CAPSULE:
			collision.shape.shape = PxShapeType::Px_CAPSULE;
			break;
		case PxShapeType::Px_SPHERE:
			collision.shape.shape = PxShapeType::Px_SPHERE;
			break;
		case PxShapeType::Px_CUBE:
			collision.shape.shape = PxShapeType::Px_CUBE;
			break;
		}
	}


	void PhysicsManager::CreateShape(Entity ent)
	{
		auto& collision = engine->world.GetComponent<CollisionComponent>(ent);
		if (collision.created)
			return;

		switch (collision.shape.shape)
		{
		case PxShapeType::Px_CUBE :
			AttachBoxToActor(ent, collision.shape.hx, collision.shape.hy, collision.shape.hz);
			break;
		case PxShapeType::Px_SPHERE :
			AttachSphereToActor(ent, collision.shape.radius);
			break;
		case PxShapeType::Px_CAPSULE :
			AttachCapsuleToActor(ent, collision.shape.radius, collision.shape.hheight);
			break;
		}
	}

	void Eclipse::PhysicsManager::SetForce(Entity ent, ECVec3 force)
	{
		if (!engine->world.CheckComponent<RigidBodyComponent>(ent))
			return;

		auto& rigidbody = engine->world.GetComponent<RigidBodyComponent>(ent);
		rigidbody.forces = force;
	}

	void Eclipse::PhysicsManager::UpdateVariables(Entity ent)
	{
		auto& collision = engine->world.GetComponent<CollisionComponent>(ent);
		switch (collision.shape.shape)
		{
		case PxShapeType::Px_CUBE :
			auto& transform = engine->world.GetComponent<TransformComponent>(ent);
			collision.shape.hx = transform.scale.getX() / 2;
			collision.shape.hy = transform.scale.getY() / 2;
			collision.shape.hz = transform.scale.getZ() / 2;
			break;
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