#pragma once
#include "pch.h"
#include "mono/jit/jit.h"
#include "Global.h"

namespace Eclipse
{
	static void AddForce(Entity ent, float x, float y, float z, int mode)
	{
		engine->gPhysics.AddForce(ent, { x, y, z }, static_cast<ForceMode>(mode));
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

	static void setGravBool(Entity ent, int state)
	{
		RigidBodyComponent& rigid = engine->world.GetComponent<RigidBodyComponent>(ent);
		rigid.enableGravity = state ? true : false;
	}
}