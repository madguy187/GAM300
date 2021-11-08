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

	static MonoObject* GetTransform(Entity entity)
	{
		TransformComponent& trans = engine->world.GetComponent<TransformComponent>(entity);
		return engine->mono.CreateVector3Class(trans.position.getX(), trans.position.getY(), trans.position.getZ());
	}

	static void SetTransform(Entity entity, MonoObject* pos)
	{
		TransformComponent& trans = engine->world.GetComponent<TransformComponent>(entity);
		trans.position = engine->mono.ConvertVectorToECVec(pos);
	}

	static MonoObject* GetRotation(Entity entity)
	{
		TransformComponent& trans = engine->world.GetComponent<TransformComponent>(entity);
		return engine->mono.CreateQuaternionClass(trans.rotation.getX(), trans.rotation.getY(), trans.rotation.getZ());
	}
	
	static void SetRotation(Entity entity, MonoObject* pos)
	{
		TransformComponent& trans = engine->world.GetComponent<TransformComponent>(entity);
		trans.rotation = engine->mono.ConvertQuaternionToECVec3(pos);
	}
}