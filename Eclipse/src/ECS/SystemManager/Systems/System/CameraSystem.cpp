#include "pch.h"
#include "CameraSystem.h"

void Eclipse::CameraSystem::Init()
{
	engine->gCamera.CreateEditorCamera();
	engine->gCamera.SetCameraSpeed(5.0f);
}

void Eclipse::CameraSystem::Update()
{	
	/*loop through camera entities*/
	//for (auto entities : mEntities)
	//{
	//	auto& transform = engine->world.GetComponent<Transform>(entities);
	//}
	//engine->gCamera.UpdateCameraInput();
	//engine->gCamera.UpdateEditorCamera();

	/*Shift later*/
	engine->gCamera.UpdateCameraInput();
	engine->gCamera.UpdateEditorCamera();


	//gCamera.UpdateViewMtx()
	//gCamera.UpdateProjectionMtx()
}
