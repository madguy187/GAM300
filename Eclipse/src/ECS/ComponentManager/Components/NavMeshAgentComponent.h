#pragma once
#include "Reflection/Meta.h"
#include "Reflection/RemTypeQual.h"
#include <Library\Math\Vector.h>
#define MAX_PATHVERT		512 // most verts in a path 
namespace Eclipse
{

	struct PATHDATA
	{
		float PosX[MAX_PATHVERT]{ 0.0f };
		float PosY[MAX_PATHVERT]{ 0.0f };
		float PosZ[MAX_PATHVERT]{ 0.0f };
		int MaxVertex{ 0 };
	};

	struct NavMeshAgentComponent
	{
		META_DATA(NavMeshAgentComponent);
		PATHDATA _path;
		ECVec3 _dest{0.0f,0.0f,0.0f};
		float _speed{ 0.0f };
		float _angularSpeed{ 0.0f };
		float _acceleration{ 0.0f };
		float _stoppingDistance{ 0.0f };
	};
}