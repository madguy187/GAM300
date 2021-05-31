#pragma once

#include "GLEW/include/GL/glew.h"
#include "GLM/glm/glm.hpp"

namespace Eclipse
{
	struct CameraComponent
	{
		float eyeAlpha{ 0.0f }; //pitch
		float eyeBeta{ -90.0f }; //yaw

		float fov{ 45.0f };
		float nearPlane{ 1.0f };
		float farPlane{ 500.0f };

		glm::vec3 eyeFront{ 0.0f, 0.0f, -1.0f }; //look-at position
		glm::vec3 upVec{ 0.0f, 1.0f, 0.0f }; //camera's up vector

		glm::mat4 viewMtx{ 0.0f };
		glm::mat4 projMtx{ 0.0f };

		float cameraSpeed{ 2.5f }; //camera movement speed

		enum class CameraType
		{
			Editor_Camera
		};

		CameraType camType;
	};
}