#pragma once
#include "Global.h"
#include "pch.h"

namespace Eclipse
{
	static MonoObject* RaycastFunc(MonoObject* origin, MonoObject* dir)
	{
		PxRaycastBuffer hit;
		if (!engine->gPhysics.Raycast(
			engine->mono.ConvertVectorToECVec(origin),
			engine->mono.ConvertVectorToECVec(dir),
			5.0f,
			//std::numeric_limits<float>::infinity(),
			hit,
			"1"
		)) return nullptr;

		return engine->mono.CreateRayCastHit(hit.block.position.x, hit.block.position.y, hit.block.position.z);
	}
}