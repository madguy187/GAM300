#pragma once
#include "Library/Math/Vector.h"

#define GLM_ENABLE_EXPERIMENTAL
#include "../Dep/GLM/glm/gtx/quaternion.hpp"

namespace Eclipse
{
	struct TransformComponent
	{
		ECVec3 position{ 0.0f, 0.0f, 0.0f };
		ECVec3 rotation{ 0.0f, 0.0f, 0.0f }; //rotation around x,y,z-axis respectively
		ECVec3 scale{ 5.0f, 5.0f, 5.0f };

		// For ImGuizmo to work - Subjected to Change
		glm::mat4 GetTransform() const
		{
			glm::mat4 Rotation = glm::toMat4(glm::quat(rotation.ConvertToGlmVec3Type()));

			return glm::translate(glm::mat4(1.0f), position.ConvertToGlmVec3Type())
				* Rotation
				* glm::scale(glm::mat4(1.0f), scale.ConvertToGlmVec3Type());
		}
	};
}