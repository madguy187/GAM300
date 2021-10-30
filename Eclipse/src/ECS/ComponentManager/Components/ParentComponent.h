#pragma once
#include "Global.h"

namespace Eclipse
{
	struct ParentComponent
	{
		META_DATA(ParentComponent);
		std::vector<Entity> child;
		glm::mat4 model;
	};
}