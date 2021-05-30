#pragma once
#include "Engine/Engine.h"
#include "ECS/ComponentManager/Components/CameraComponent.h"

namespace Eclipse
{
	class CameraManager
	{
		// Keyboard input press flags
		bool zoomIn_flag{ false }; //button Z
		bool zoomOut_flag{ false }; //button X
		bool moveRight_flag{ false };  //button A
		bool moveLeft_flag{ false }; //button D
		bool moveFront_flag{ false }; //button W
		bool moveBack_flag{ false }; //button S

		/*DOUBLE CHECK THIS*/
		unsigned int editorCamID;
	public:
		static CameraManager& GetCameraManager();

		void InitEditorCamera();
		unsigned int GetEditorCameraID();
		void UpdateEditorCamera();

		void UpdateCameraInput();

		void SetCameraSpeed(float newSpeed);
	};
}