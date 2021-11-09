#include "pch.h"
#include "../CameraSystem/CameraSystem.h"

void Eclipse::CameraSystem::Init()
{
	engine->gCamera.CreateEditorCamera();
	engine->gCamera.SetCameraSpeed(50.0f);

	engine->gCamera.CreateViewCamera(CameraComponent::CameraType::TopView_Camera);
	engine->gCamera.CreateViewCamera(CameraComponent::CameraType::BottomView_Camera);
	engine->gCamera.CreateViewCamera(CameraComponent::CameraType::LeftView_Camera);
	engine->gCamera.CreateViewCamera(CameraComponent::CameraType::RightView_camera);

	engine->gCamera.CreatePerspectiveCamera(CameraComponent::CameraType::MeshEditor_Camera);
	engine->gCamera.CreatePerspectiveCamera(CameraComponent::CameraType::MaterialEditor_Camera);

	//Temporary test code to create an initial game camera, remove later
	engine->gCamera.CreateGameCamera();
	auto& _camera = engine->world.GetComponent<CameraComponent>(engine->gCamera.GetGameCameraID());
	engine->gDebugManager.AddCameraFrustum(engine->gCamera.GetGameCameraID());
	engine->gCamera.SetFarPlane(_camera, 100.0f);
}

void Eclipse::CameraSystem::Update()
{
	ZoneScopedN("Camera System")
	engine->Timer.SetName({ SystemName::CAMERA });
	engine->Timer.tracker.system_start = static_cast<float>(glfwGetTime());
	for (auto& it : mEntities)
	{
		auto& _camera = engine->world.GetComponent<CameraComponent>(it);
		auto& _transform = engine->world.GetComponent<TransformComponent>(it);

		if (_camera.camType == CameraComponent::CameraType::Editor_Camera)
		{
			//engine->gCamera.CheckCameraInput();
			engine->gCamera.SetFarPlane(_camera, 300.0f);
			engine->gCamera.UpdateEditorCamera(_transform);
		}
		else if (_camera.camType == CameraComponent::CameraType::MeshEditor_Camera)
		{
			//engine->gCamera.CheckMeshCameraInput();
			engine->gCamera.UpdateMeshCamera(_transform);
		}
		else if (_camera.camType == CameraComponent::CameraType::MaterialEditor_Camera)
		{
			//engine->gCamera.CheckMaterialCameraInput();
			engine->gCamera.UpdateMaterialCamera(_transform);
		}
		else if ((_camera.camType == CameraComponent::CameraType::LeftView_Camera) ||
				(_camera.camType == CameraComponent::CameraType::RightView_camera) ||
				(_camera.camType == CameraComponent::CameraType::TopView_Camera) ||
				(_camera.camType == CameraComponent::CameraType::BottomView_Camera))
		{
			//engine->gCamera.CheckViewCameraInput();
			engine->gCamera.UpdateViewCamera(_camera, _transform);
		}

		engine->gCamera.ComputeViewDirection(_camera, _transform);
		engine->gCamera.ComputeViewMtx(_camera, _transform);
		engine->gCamera.ComputePerspectiveMtx(_camera);
	}
	engine->Timer.tracker.system_end = static_cast<float>(glfwGetTime());
	engine->Timer.UpdateTimeContainer(engine->Timer.tracker);
	FrameMark
}
