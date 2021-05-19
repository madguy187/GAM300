#pragma once
#include "pch.h"
#include "Global.h"

namespace Eclipse
{
	class System
	{
	public:
		virtual void Update() = 0;
		std::set<Entity> mEntities;
	};
}