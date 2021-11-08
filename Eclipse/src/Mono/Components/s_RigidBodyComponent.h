#pragma once
#include "pch.h"
#include "mono/jit/jit.h"
#include "Global.h"

namespace Eclipse
{
	static void AddForce(Entity ent, float x, float y, float z,ForceMode mode)
	{
		engine->gPhysics.AddForce(ent, { x, y, z },mode);
	}

	static float getMass(Entity ent)
	{
		RigidBodyComponent& rigid = engine->world.GetComponent<RigidBodyComponent>(ent);
		return rigid.mass;
	}

	static float getX(Entity ent)
	{
		RigidBodyComponent& rigid = engine->world.GetComponent<RigidBodyComponent>(ent);
		return rigid.velocity.getX();
	}

	static float getY(Entity ent)
	{
		RigidBodyComponent& rigid = engine->world.GetComponent<RigidBodyComponent>(ent);
		return rigid.velocity.getY();
	}

	static float getZ(Entity ent)
	{
		RigidBodyComponent& rigid = engine->world.GetComponent<RigidBodyComponent>(ent);
		return rigid.velocity.getZ();
	}
}