#pragma once
#include "Global.h"
#include "ECS/SystemManager/Systems/System/System.h"

namespace Eclipse
{
	class MonoSystem : public System
	{

	public:
		void Init();
		void Update() override;
		void Terminate();
	};
}