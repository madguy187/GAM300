#pragma once
#include "Library/Math/Vector.h"
#include "Reflection/Meta.h"
#include "Reflection/RemTypeQual.h"

namespace Eclipse
{
	struct TransformComponent
	{
		META_DATA(TransformComponent);
		ECVec3 position{ 0.0f, 0.0f, 0.0f };
		ECVec3 rotation{ 0.0f, 0.0f, 0.0f }; //rotation around x,y,z-axis respectively
		ECVec3 scale{ 5.0f, 5.0f, 5.0f };
	};
}