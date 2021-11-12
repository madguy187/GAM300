#pragma once
#include "Global.h"
#include "pch.h"
#include "GLM/glm/gtx/euler_angles.hpp"

namespace Eclipse
{
	static void RotateEuler(Entity entity, float Rx, float Ry, float Rz)
	{
		TransformComponent& trans = engine->world.GetComponent<TransformComponent>(entity);
		trans.rotation.setX(trans.rotation.getX() + Rx);
		trans.rotation.setY(trans.rotation.getY() + Ry);
		trans.rotation.setZ(trans.rotation.getZ() + Rz);
		//glm::mat4 rotMat = glm::mat4(1.0f);
		//rotMat = glm::eulerAngleXYZ(glm::radians(trans.rotation.getX()), glm::radians(trans.rotation.getY()), glm::radians(trans.rotation.getZ()));

		//glm::mat4 additiveRotMat = glm::mat4(1.0f);
		//additiveRotMat = glm::eulerAngleXYZ(glm::radians(Rx), glm::radians(Ry), glm::radians(Rz));

		//glm::mat4 finalRot = additiveRotMat * rotMat;
		//glm::vec3 finalEulerRot;
		//glm::extractEulerAngleXYZ(finalRot, finalEulerRot.x, finalEulerRot.y, finalEulerRot.z);
		//finalEulerRot = glm::degrees(finalEulerRot);
		///*glm::quat quaternion = { glm::radians(trans.rotation.ConvertToGlmVec3Type()) };
		//glm::vec3 additiveEulerRot{ glm::radians(Rx), glm::radians(Ry), glm::radians(Rz) };
		//glm::quat additiveQuatRot{ additiveEulerRot };

		//glm::quat finalRot =  additiveQuatRot * quaternion;
		//glm::vec3 finalEulerRot = glm::eulerAngles(finalRot);*/
		//std::cout << finalEulerRot.x << " " << finalEulerRot.y << " " << finalEulerRot.z << std::endl;
		//
		//trans.rotation.setX(finalEulerRot.x);
		//trans.rotation.setY(finalEulerRot.y);
		//trans.rotation.setZ(finalEulerRot.z);
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
		CameraComponent& camera = engine->world.GetComponent<CameraComponent>(engine->gCamera.GetGameCameraID());
		glm::vec3 direction = camera.eyeFront;
		/*glm::quat quaternion{ glm::radians(trans.rotation.ConvertToGlmVec3Type()) };
		glm::vec4 forwardVec = glm::vec4{ 0 , 0, -1 , 0 };
		glm::vec4 direction = glm::rotate(quaternion, forwardVec);*/

		return engine->mono.CreateVector3Class(direction.x, direction.y, direction.z);
	}

	static MonoObject* GetBack(Entity entity)
	{
		/*glm::vec3 direction = engine->world.GetComponent<CameraComponent>(engine->gCamera.GetGameCameraID()).eyeFront;
		direction = -direction;
		return engine->mono.CreateVector3Class(direction.x, direction.y, direction.z);*/
		TransformComponent& trans = engine->world.GetComponent<TransformComponent>(entity);
		//glm::quat quad{ glm::radians(trans.rotation.ConvertToGlmVec3Type()) };
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::rotate(model, glm::radians(trans.rotation.getX()), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(trans.rotation.getY()), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(trans.rotation.getZ()), glm::vec3(0.0f, 0.0f, 1.0f));

		glm::vec3 direction = model[2];

		//glm::vec3 direction{ 2 * (quad.x * quad.z + quad.w * quad.y), 2 * (quad.y * quad.z - quad.w * quad.x), 1 - 2 * (quad.x * quad.x + quad.y * quad.y) };
		/*direction.x = cos(glm::radians(trans.rotation.y)) * cos(glm::radians(trans.rotation.x));
		direction.y = sin(glm::radians(trans.rotation.x));
		direction.z = sin(glm::radians(trans.rotation.y)) * cos(glm::radians(trans.rotation.x));
		direction = glm::normalize(direction);

		direction = glm::normalize(glm::cross(direction, glm::vec3(0, 1, 0)));*/

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