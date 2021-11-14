#pragma once
#include "Global.h"

namespace Eclipse
{
	struct ChildComponent
	{
		META_DATA(ChildComponent);
		Entity parentIndex = MAX_ENTITY;
		ECVec3 PosOffset;
		ECVec3 RotOffset;
		ECVec3 ScaleOffset;
		bool UpdateChildren{ false };

		//for imgui side
		bool hasParent = false;
		bool IsAChild = false;
	};
}