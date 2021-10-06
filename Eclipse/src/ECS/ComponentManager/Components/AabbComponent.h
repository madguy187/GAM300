#pragma once
#include "Reflection/Meta.h"
#include "Reflection/RemTypeQual.h"
#include "Library/Math/Vector.h"

namespace Eclipse
{
	struct AABBComponent
	{
		META_DATA(AABBComponent);
		ECVec3 center{ 0.0f, 0.0f, 0.0f };
		ECVec3 Min{ 0.0f, 0.0f, 0.0f };
		ECVec3 Max{ 0.0f, 0.0f, 0.0f };
	};
}
