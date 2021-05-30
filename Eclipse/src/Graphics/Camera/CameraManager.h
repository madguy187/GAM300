#pragma once
#include "ECS/ComponentManager/Components/CameraComponent.h"
#include "ECS/ComponentManager/Components/TransformComponent.h"
#include "ECS/SystemManager/Systems/System/RenderSystem.h"

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
		bool pitchUp_flag{ false }; //button R
		bool pitchDown_flag{ false }; //button F
		bool yawLeft_flag{ false }; //button Q
		bool yawRight_flag{ false }; //button E

		/*DOUBLE CHECK THIS*/
		unsigned int editorCamID = MAX_ENTITY;
		
	public:
		void CreateEditorCamera();
		unsigned int GetEditorCameraID();

		void ComputeViewMtx(TransformComponent& _transform);
		void UpdateEditorCamera();

		void UpdateCameraInput();

		void SetCameraSpeed(float newSpeed);
	};
}