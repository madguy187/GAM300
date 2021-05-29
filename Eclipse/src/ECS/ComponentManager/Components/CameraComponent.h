#pragma once

#include "GLEW/include/GL/glew.h"
#include "GLM/glm/glm.hpp"

namespace Eclipse
{
	struct CameraComponent
	{
		float eyeAlpha{ 1 };
		float eyeBeta{ 18 };
		float eyeRadius{ 40 };

		// Keyboard input press flags
		bool zoomIn_flag{ false }; //button Z
		bool zoomOut_flag{ false }; //button X
		bool moveRight_flag{ false };  //button A
		bool moveLeft_flag{ false }; //button D
		bool moveUp_flag{ false }; //button W
		bool moveDown_flag{ false }; //button S

		enum class CameraType
		{
			Editor_Camera
		};

		CameraType camType;
	};
}