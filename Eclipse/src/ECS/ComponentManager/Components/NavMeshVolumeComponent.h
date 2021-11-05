#pragma once
#include "Reflection/Meta.h"
#include "Reflection/RemTypeQual.h"
#include <Library\Math\Vector.h>
namespace Eclipse
{
	struct NavMeshVolumeComponent
	{
		META_DATA(NavMeshVolumeComponent);
		ECVec3 NavMeshMin;
		ECVec3 NavMeshMax;

		float AgentRadius;
		float AgentHeight;
		float MaxSlope;
		float JumpDistance;
	};
}