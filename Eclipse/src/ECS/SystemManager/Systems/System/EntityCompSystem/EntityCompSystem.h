#pragma once
#include "Global.h"
#include "ECS/SystemManager/Systems/System/System.h"

namespace Eclipse
{
	class EntityCompSystem : public System
	{
	public:
		void Update() override;
		Entity FindEntity(const std::string& entName);
	};
}