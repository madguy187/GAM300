#include "pch.h"
#include "Frustum.h"

Frustum::Frustum()
{
}

glm::vec3 Frustum::ComputeNearCenter(CameraComponent& _camera)
{
	glm::vec3 center = _camera.eyePos + _camera.eyeFront * _camera.nearPlane;
	
	return center;
}

glm::vec3 Frustum::ComputeFarCenter(CameraComponent& _camera)
{
	glm::vec3 center = _camera.eyePos + _camera.eyeFront * _camera.farPlane;
	
	return center;
}

glm::vec2 Frustum::ComputeNearPlaneXY(CameraComponent& _camera)
{
	float nearHeight = 2.0f * glm::tan(glm::radians(_camera.fov) / 2.0f) * _camera.nearPlane;
	float nearWidth = nearHeight * _camera.aspect;

	return glm::vec2{ nearWidth, nearHeight };
}

glm::vec2 Frustum::ComputeFarPlaneXY(CameraComponent& _camera)
{
	float farHeight = 2.0f * glm::tan(glm::radians(_camera.fov) / 2.0f) * _camera.farPlane;
	float farWidth = farHeight * _camera.aspect;

	return glm::vec2{ farWidth, farHeight };
}

void Frustum::AddLineSegment(unsigned int index, glm::vec3 p0, glm::vec3 p1)
{
	ENGINE_LOG_ASSERT(index < 12, "Frustum index cannot be greater than 11!");

	Lines3D newLine{ p0, p1 };
	lineSegments[index] = newLine;
}

void Frustum::UpdateFrustum(CameraComponent& _camera)
{
	glm::vec3 farCenter = ComputeFarCenter(_camera);
	glm::vec2 farXY = ComputeFarPlaneXY(_camera);

	glm::vec3 nearCenter = ComputeNearCenter(_camera);
	glm::vec2 nearXY = ComputeNearPlaneXY(_camera);

	glm::vec3 farTopLeft = farCenter + _camera.upVec * (0.5f * farXY.y) - _camera.rightVec * (0.5f * farXY.x);
	glm::vec3 farTopRight = farCenter + _camera.upVec * (0.5f * farXY.y) + _camera.rightVec * (0.5f * farXY.x);
	glm::vec3 farBottomLeft = farCenter - _camera.upVec * (0.5f * farXY.y) - _camera.rightVec * (0.5f * farXY.x);
	glm::vec3 farBottomRight = farCenter - _camera.upVec * (0.5f * farXY.y) + _camera.rightVec * (0.5f * farXY.x);

	glm::vec3 nearTopLeft = nearCenter + _camera.upVec * (0.5f * nearXY.y) - _camera.rightVec * (0.5f * nearXY.x);
	glm::vec3 nearTopRight = nearCenter + _camera.upVec * (0.5f * nearXY.y) + _camera.rightVec * (0.5f * nearXY.x);
	glm::vec3 nearBottomLeft = nearCenter - _camera.upVec * (0.5f * nearXY.y) - _camera.rightVec * (0.5f * nearXY.x);
	glm::vec3 nearBottomRight = nearCenter - _camera.upVec * (0.5f * nearXY.y) + _camera.rightVec * (0.5f * nearXY.x);

	/*************************************************************************
	  Frustum line segments

	  Near Plane
	  Line 0: NearTopLeft - NearTopRight
	  Line 1: NearBottomLeft - NearBottomRight
	  Line 2: NearTopLeft - NearBottomLeft
	  Line 3: NearTopRight - NearBottomRight

	  Far Plane
	  Line 4: FarTopLeft - FarTopRight
	  Line 5: FarBottomLeft - FarBottomRight
	  Line 6: FarTopLeft - FarBottomLeft
	  Line 7: FarTopRight - FarBottomRight

	  Connecting
	  Line 8: NearTopLeft - FarTopLeft
	  Line 9: NearTopRight - FarTopRight
	  Line 10: NearBottomLeft - FarBottomLeft
	  Line 11: NearBottomRight - FarBottomRight
	*************************************************************************/

	//Add near plane line segments
	AddLineSegment(0, nearTopLeft, nearTopRight);
	AddLineSegment(1, nearBottomLeft, nearBottomRight);
	AddLineSegment(2, nearTopLeft, nearBottomLeft);
	AddLineSegment(3, nearTopRight, nearBottomRight);
	
	//Add far plane line segments
	AddLineSegment(4, farTopLeft, farTopRight);
	AddLineSegment(5, farBottomLeft, farBottomRight);
	AddLineSegment(6, farTopLeft, farBottomLeft);
	AddLineSegment(7, farTopRight, farBottomRight);
	
	////Connecting line segments
	AddLineSegment(8, nearTopLeft, farTopLeft);
	AddLineSegment(9, nearTopRight, farTopRight);
	AddLineSegment(10, nearBottomLeft, farBottomLeft);
	AddLineSegment(11, nearBottomRight, farBottomRight);
}

typename Frustum::LineArr& Frustum::GetLineSegments()
{
	return lineSegments;
}



