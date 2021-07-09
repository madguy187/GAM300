#pragma once
#include "pch.h"
#include "ECS/ComponentManager/Components/CameraComponent.h"
#include "ECS/ComponentManager/Components/TransformComponent.h"
#include "ECS/SystemManager/Systems/System/RenderSystem.h"

namespace Eclipse
{
	class CameraManager
	{
		/*************************************************************************
		  Camera bitset layout
		  0: Move Right								: buttton A
		  1: Move Left								: buttton D
		  2: Move Front								: buttton W
		  3: Move Back								: buttton S
		  4: Pitch Up (Rotation around x-axis)		: buttton R
		  5: Pitch Down (Rotation around x-axis)	: buttton F
		  6: Yaw Left (Rotation around y-axis)		: buttton Q
		  7: Yaw Right (Rotation around y-axis)		: buttton E
		  8: "Zoom In" (FOV)						: buttton Z
		  9: "Zoom Out" (FOV)						: buttton X
		*************************************************************************/
		std::bitset<10> input;

		unsigned int editorCamID = MAX_ENTITY;	
		unsigned int gameCamID = MAX_ENTITY;
	public:
		InputWrapper InputHandler;

		void CreateEditorCamera();
		unsigned int GetEditorCameraID();

		void ComputeViewDirection(CameraComponent& _camera, TransformComponent& _transform);
		void ComputeViewMtx(CameraComponent& _camera, TransformComponent& _transform);
		void ComputePerspectiveMtx(CameraComponent& _camera);
		void UpdateEditorCamera(TransformComponent& _transform);

		void CheckCameraInput();

		void SetCameraSpeed(float newSpeed);
		void SetNearPlane(CameraComponent& _camera, float _nearPlane);
		void SetFarPlane(CameraComponent& _camera, float _farPlane);

		std::bitset<10>& GetInput();

		void CreateGameCamera();
		unsigned int GetGameCameraID();
	};
}