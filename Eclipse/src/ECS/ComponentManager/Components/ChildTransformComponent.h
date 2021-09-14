#pragma once

#include "Global.h"

namespace Eclipse
{
	struct ChildTransformComponent
	{
		// Darren -
		// This will be child Movement

		ECVec3 ChildPosition{ 0.0f, 0.0f, 0.0f };
		ECVec3 ChildRotation{ 0.0f, 0.0f, 0.0f }; 
		ECVec3 ChildScale{ 5.0f, 5.0f, 5.0f };
	};
}
