#pragma once
#include "Library/Math/Vector.h"

namespace Eclipse
{
	struct TransformComponent
	{
		ECVec3 position{ 0.0f, 0.0f, 0.0f };
		ECVec3 rotation{ 0.0f, 0.0f, 0.0f }; //rotation around x,y,z-axis respectively
		ECVec3 scale{ 10.0f, 10.0f, 10.0f };
	};
}