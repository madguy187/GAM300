#pragma once
#include "Library/Math/Vector.h"

namespace Eclipse
{
	struct AABBComponent
	{
		ECVec3 center{ 0.0f, 0.0f, 0.0f };
		ECVec3 min{ 0.0f, 0.0f, 0.0f };
		ECVec3 max{ 0.0f, 0.0f, 0.0f };
	};
}