#pragma once
#include "Global.h"

namespace Eclipse
{
	struct ChildComponent
	{
		META_DATA(ChildComponent);
		Entity parentIndex;
		ECVec3 offset;
	};
}