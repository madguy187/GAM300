#pragma once
#include "pch.h"
#include "ECS/ComponentManager/Components/CameraComponent.h"
#include "BasicPrimitives/Frustum.h"
#include <any>

namespace Eclipse
{
	class DebugRenderingManager
	{
		std::map<unsigned int, std::any> debugShapes;
		void CheckUniformLoc(Shader*_shdrpgm, CameraComponent& _camera);
	public:
		void AddCameraFrustum(unsigned int ID, CameraComponent& _camera);
		void DrawFrustum(unsigned int ID, unsigned int framebufferID);
		void SetDebugShape(unsigned int ID, std::any newShape);

		void DrawDebugShapes(unsigned int framebufferID);

		void ClearDebugShapes();
	};
}