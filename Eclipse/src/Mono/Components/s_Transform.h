#pragma once
#include "Global.h"
#include "pch.h"

namespace Eclipse
{
	static void RotateEuler(Entity entity, float Rx, float Ry, float Rz)
	{
		TransformComponent& trans = engine->world.GetComponent<TransformComponent>(entity);
		trans.rotation.setX(trans.rotation.getX() + Rx);
		trans.rotation.setY(trans.rotation.getY() + Ry);
		trans.rotation.setZ(trans.rotation.getZ() + Rz);
	}
}