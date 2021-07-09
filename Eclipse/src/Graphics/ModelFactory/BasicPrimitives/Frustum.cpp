#include "pch.h"
#include "Frustum.h"

Frustum::Frustum(glm::vec3 _startPos, glm::vec3 _endPos) :
	vaoID{ 1 }, vboID{ 1 }, eboID{ 0 },
	primitiveType{ GL_LINES },
	primitiveCount{ 1 },
	drawCount{ 2 }
{
	startPos = _startPos;
	endPos = _endPos;

	initModel();
}

void Frustum::initModel()
{
	InsertModelData();
	CreateBuffers();
}

void Frustum::InsertPosVtx()
{
	GLfloat lineSeg[] =
	{
		startPos.x, startPos.y, startPos.z,
		endPos.x, endPos.y, endPos.z
	};

	PosVec.push_back({ lineSeg[0], lineSeg[1], lineSeg[2] });
	PosVec.push_back({ lineSeg[3], lineSeg[4], lineSeg[5] });
}

void Frustum::InsertIdxVtx()
{

}

void Frustum::InsertModelData()
{
	InsertPosVtx();
}

GLuint Frustum::GetVaoID()
{
	return vaoID;
}

GLuint Frustum::GetVboID()
{
	return vboID;
}

GLuint Frustum::GetEboID()
{
	return eboID;
}

GLenum Frustum::GetPrimitiveType()
{
	return primitiveType;
}

GLuint Frustum::GetPrimitiveCount()
{
	return primitiveCount;
}

GLuint Frustum::GetDrawCount()
{
	return drawCount;
}

void Frustum::SetVaoID(GLuint id)
{
	this->vaoID = id;
}

void Frustum::SetVboID(GLuint id)
{
	this->vboID = id;
}

void Frustum::SetEboID(GLuint id)
{
	this->eboID = id;
}

void Frustum::SetPrimitiveType(GLenum type)
{
	this->primitiveType = type;
}

void Frustum::SetPrimitiveCount(GLuint count)
{
	this->primitiveCount = count;
}

void Frustum::SetDrawCount(GLuint count)
{
	this->drawCount = count;
}

void Frustum::CreateVAO()
{
	//Define the VAO handle for position attributes
	glCreateVertexArrays(1, &vaoID);
	glEnableVertexArrayAttrib(vaoID, 0);
	glVertexArrayVertexBuffer(vaoID, 0, vboID, 0, sizeof(glm::vec3));
	glVertexArrayAttribFormat(vaoID, 0, 3, GL_FLOAT, GL_FALSE, 0);
	glVertexArrayAttribBinding(vaoID, 0, 0);
}

void Frustum::CreateVBO()
{
	glCreateBuffers(1, &vboID);
	glNamedBufferStorage(vboID,
		sizeof(glm::vec3) * PosVec.size(), nullptr, GL_DYNAMIC_STORAGE_BIT);
	glNamedBufferSubData(vboID, 0, sizeof(glm::vec3) * PosVec.size(), PosVec.data());
}

void Frustum::CreateEBO()
{

}

void Frustum::CreateBuffers()
{
	CreateVBO();
	CreateVAO();
	CreateEBO();
}

void Frustum::DeleteModel()
{
	glDeleteVertexArrays(1, &vaoID);
	glDeleteBuffers(1, &vboID);
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

	FrustumLines newLine{ p0, p1 };
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



