#pragma once
#include "pch.h"
#include "mono/jit/jit.h"
#include "Global.h"

namespace Eclipse
{
	static void SetForce(Entity ent, float x, float y, float z)
	{
		engine->gPhysics.SetForce(ent, { x, y, z });
	}
}