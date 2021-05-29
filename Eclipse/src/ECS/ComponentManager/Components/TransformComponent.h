#pragma once

namespace Eclipse
{
	struct TransformComponent
	{
		float x = 0.0f;
		float y = 0.0f;
		float z = 0.0f;

		float ScaleX = 10.0f;
		float ScaleY = 10.0f;
		float ScaleZ = 10.0f;

		float rotX = 0.0f;
		float rotY = 0.0f;
		float rotZ = 0.0f;

		// to be replaced with new lib
		glm::vec3 pos{0,0,0};
		glm::vec3 scale{ 5.0f,5.0f,5.0f };
		glm::vec3 rot{ 0,0,0 };
	};
}