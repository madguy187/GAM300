#pragma once
#include "ECS/ComponentManager/Components/TransformComponent.h"
#include "ECS/ComponentManager/Components/AabbComponent.h"
#include "Editor/Windows/Scene/SceneView.h"
#include <limits>

#ifndef PICKINGMANAGER_H
#define PICKINGMANAGER_H

namespace Eclipse
{
	class PickingManager
	{
		unsigned int currentCollisionID = MAX_ENTITY;
	public:
		void GenerateAabb(unsigned int ID, TransformComponent& _transform, EntityType _type);
		void UpdateAabb(unsigned int ID);
		glm::vec3 ComputeCursorRayDirection();
		static bool RayAabb(glm::vec3& rayStart, glm::vec3& rayDir, glm::vec3& aabbMin, glm::vec3& aabbMax, float& t);
		unsigned int GetCurrentCollisionID();
		void SetCurrentCollisionID(unsigned int ID);
	};
}

#endif  /* PICKINGMANAGER_H */