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
		  0: Move Right								: button A
		  1: Move Left								: button D
		  2: Move Front								: button W
		  3: Move Back								: button S
		  4: Pitch Up (Rotation around x-axis)		: button R
		  5: Pitch Down (Rotation around x-axis)	: button F
		  6: Yaw Left (Rotation around y-axis)		: button Q
		  7: Yaw Right (Rotation around y-axis)		: button E
		  8: "Zoom In" (FOV)						: button Z
		  9: "Zoom Out" (FOV)						: button X
		  10: Move Up								: button T
		  11: Move Down								: button G
		*************************************************************************/
		std::bitset<12> input;

		std::map<CameraComponent::CameraType, unsigned int> cameraList;

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

		std::bitset<12>& GetInput();

		void CreateGameCamera();
		unsigned int GetGameCameraID();

		void CreateViewCamera(CameraComponent::CameraType _camType);
		void SetViewCameraValues(CameraComponent::CameraType _camType, TransformComponent& _transform);

		unsigned int GetCameraID(CameraComponent::CameraType _camType);
	};
}