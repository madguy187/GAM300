#pragma once
#include "Global.h"
#include "pch.h"

namespace Eclipse
{
	static MonoObject* RaycastFunc(MonoObject* origin, MonoObject* dir, float dist, MonoString* mask)
	{
		if (dist == 0.0f) dist = (std::numeric_limits<float>::max)();
		std::string test = mono_string_to_utf8(mask);
		PxRaycastBuffer hit;
		if (!engine->gPhysics.Raycast(
			engine->mono.ConvertVectorToECVec(origin),
			engine->mono.ConvertVectorToECVec(dir),
			dist,
			hit,
			test
		)) return nullptr;

		Entity ent = *(Entity*)hit.block.actor->userData;

		return engine->mono.CreateRayCastHit(ent, hit.block.position.x, hit.block.position.y, hit.block.position.z);
	}
}