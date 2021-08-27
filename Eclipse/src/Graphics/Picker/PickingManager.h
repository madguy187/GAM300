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
	public:
		void GenerateAabb(unsigned int ID, TransformComponent& _transform);
		void UpdateAabb(unsigned int ID);
		glm::vec3 ComputeCursorRayDirection();
		bool RayAabb(glm::vec3& rayStart, glm::vec3& rayDir, glm::vec3& aabbMin, glm::vec3& aabbMax, float& t);
	};
}

#endif  /* PICKINGMANAGER_H */