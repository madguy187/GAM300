#pragma once

#include "GLEW/include/GL/glew.h"
#include "GLM/glm/glm.hpp"
#include "Reflection/Meta.h"
#include "Reflection/RemTypeQual.h"

namespace Eclipse
{
	struct CameraComponent
	{
		META_DATA(CameraComponent);
		float eyeAlpha{ 0.0f }; //pitch
		float eyeBeta{ -90.0f }; //yaw

		float fov{ 45.0f };
		float nearPlane{ 0.1f };
		float farPlane{ 500.0f };
		float aspect{ 1.0f };

		glm::vec3 eyePos{ 0.0f, 0.0f, 0.0f }; //camera position
		glm::vec3 eyeFront{ 0.0f, 0.0f, -1.0f }; //look-at position
		glm::vec3 upVec{ 0.0f, 1.0f, 0.0f }; //camera's up vector
		glm::vec3 rightVec{ 1.0f, 0.0f,0.0f }; //camera's right vector

		glm::vec3 worldUp{ 0.0f, 1.0f, 0.0f }; //World's up vector

		glm::mat4 viewMtx{ 0.0f };
		glm::mat4 projMtx{ 0.0f };

		float cameraSpeed{ 50.f }; //camera movement speed

		enum class CameraType
		{
			Editor_Camera,
			Game_Camera,
			TopView_Camera,
			BottomView_Camera,
			LeftView_Camera,
			RightView_camera
		};

		CameraType camType;

		enum class ProjectionType
		{
			Orthographic,
			Perspective
		};

		ProjectionType projType;
	};
}