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

		float AgentRadius{2.5f};
		float AgentHeight{10.0f};
		float MaxSlope{45.0f};
		float JumpDistance{4.0f};
	};
}