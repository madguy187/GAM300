#include "pch.h"
#include "CameraSystem.h"

void Eclipse::CameraSystem::Init()
{
	engine->gCamera.CreateEditorCamera();
	engine->gCamera.SetCameraSpeed(5.0f);
}

void Eclipse::CameraSystem::Update()
{	
	for (auto& it : mEntities)
	{
		auto& _camera = engine->world.GetComponent<CameraComponent>(it);
		auto& _transform = engine->world.GetComponent<TransformComponent>(it);

		engine->gCamera.CheckCameraInput();
		engine->gCamera.UpdateEditorCamera(_transform);

		engine->gCamera.ComputeViewDirection(_camera, _transform);
		engine->gCamera.ComputeViewMtx(_camera, _transform);
		engine->gCamera.ComputePerspectiveMtx(_camera);
	}
}
