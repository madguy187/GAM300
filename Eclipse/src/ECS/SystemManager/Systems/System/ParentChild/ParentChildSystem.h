#pragma once

#include "Global.h"

namespace Transcend
{
	class ParentChildSystem : public System
	{
	public:
		void Update() override;
		void PostUpdate();
	};
}