#pragma once
#include "Global.h"

namespace Eclipse
{
	struct ParentComponent
	{
		META_DATA(ParentComponent);
		std::vector<Entity> child;
		bool UpdateChildren{ false };
	};
}