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

	static MonoObject* GetLeft(Entity entity)
	{
		TransformComponent& trans = engine->world.GetComponent<TransformComponent>(entity);
		glm::vec3 direction;
		std::cout << "C++: " << trans.rotation.x << " " << trans.rotation.y << " " << trans.rotation.z << std::endl;
		direction.x = sin(glm::radians(trans.rotation.y)) * cos(glm::radians(trans.rotation.x));
		direction.y = sin(glm::radians(trans.rotation.x));
		direction.z = cos(glm::radians(trans.rotation.y)) * cos(glm::radians(trans.rotation.x));
		direction = glm::normalize(direction);

		direction = glm::normalize(glm::cross(direction, glm::vec3(0, 1, 0)));

		return engine->mono.CreateVector3Class(direction.x, direction.y, direction.z);
	}

	static MonoObject* GetRight(Entity entity)
	{
		TransformComponent& trans = engine->world.GetComponent<TransformComponent>(entity);

		glm::vec3 direction;
		direction.x = cos(glm::radians(trans.rotation.y)) * cos(glm::radians(trans.rotation.x));
		direction.y = sin(glm::radians(trans.rotation.x));
		direction.z = sin(glm::radians(trans.rotation.y)) * cos(glm::radians(trans.rotation.x));
		direction = glm::normalize(direction);

		glm::vec3 rightVec = glm::normalize(glm::cross(direction, glm::vec3(0, 1, 0)));
		//glm::vec3 upVec = glm::normalize(glm::cross(_camera.rightVec, direction));
		std::cout << "C++: " << direction.x << " " << direction.y << " " << direction.z << std::endl;

		return engine->mono.CreateVector3Class(rightVec.x, rightVec.y, rightVec.z);
	}

	static MonoObject* GetForward(Entity entity)
	{
		TransformComponent& trans = engine->world.GetComponent<TransformComponent>(entity);
		glm::vec3 direction;
		direction.x = cos(glm::radians(trans.rotation.y)) * cos(glm::radians(trans.rotation.x));
		direction.y = sin(glm::radians(trans.rotation.x));
		direction.z = sin(glm::radians(trans.rotation.y)) * cos(glm::radians(trans.rotation.x));
		direction = glm::normalize(direction);

		direction = glm::normalize(glm::cross(direction, glm::vec3(0, -1, 0)));

		return engine->mono.CreateVector3Class(direction.x, direction.y, direction.z);
	}

	static MonoObject* GetBack(Entity entity)
	{
		TransformComponent& trans = engine->world.GetComponent<TransformComponent>(entity);
		glm::vec3 direction;
		direction.x = cos(glm::radians(trans.rotation.y)) * cos(glm::radians(trans.rotation.x));
		direction.y = sin(glm::radians(trans.rotation.x));
		direction.z = sin(glm::radians(trans.rotation.y)) * cos(glm::radians(trans.rotation.x));
		direction = glm::normalize(direction);

		direction = glm::normalize(glm::cross(direction, glm::vec3(0, 1, 0)));

		return engine->mono.CreateVector3Class(direction.x, direction.y, direction.z);
	}

	static MonoObject* GetUp(Entity entity)
	{
		TransformComponent& trans = engine->world.GetComponent<TransformComponent>(entity);
		glm::vec3 direction;
		direction.x = cos(glm::radians(trans.rotation.y)) * cos(glm::radians(trans.rotation.x));
		direction.y = sin(glm::radians(trans.rotation.x));
		direction.z = sin(glm::radians(trans.rotation.y)) * cos(glm::radians(trans.rotation.x));
		direction = glm::normalize(direction);

		direction = glm::normalize(glm::cross(direction, glm::vec3(0, 0, -1)));

		return engine->mono.CreateVector3Class(direction.x, direction.y, direction.z);
	}

	static MonoObject* GetDown(Entity entity)
	{
		TransformComponent& trans = engine->world.GetComponent<TransformComponent>(entity);
		glm::vec3 direction;
		direction.x = cos(glm::radians(trans.rotation.y)) * cos(glm::radians(trans.rotation.x));
		direction.y = sin(glm::radians(trans.rotation.x));
		direction.z = sin(glm::radians(trans.rotation.y)) * cos(glm::radians(trans.rotation.x));
		direction = glm::normalize(direction);

		direction = glm::normalize(glm::cross(direction, glm::vec3(0, 0, 1)));

		return engine->mono.CreateVector3Class(direction.x, direction.y, direction.z);
	}
}