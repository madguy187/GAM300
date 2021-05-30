#include "pch.h"
#include "CameraSystem.h"

void Eclipse::CameraSystem::Init()
{
	auto& gCamera = CameraManager::GetCameraManager();

	gCamera.InitEditorCamera();
	gCamera.SetCameraSpeed(5.0f);
}

void Eclipse::CameraSystem::Update()
{
	auto& gCamera = CameraManager::GetCameraManager();
	
	gCamera.UpdateCameraInput();
	gCamera.UpdateEditorCamera();
}
