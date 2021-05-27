#pragma once
#include "mono/jit/jit.h"

namespace Eclipse
{
	class MonoManager
	{
		MonoDomain* domain;
		std::vector<MonoObject*> objects;
		std::vector<MonoImage*> images;
	public:
		void Init();
		void Update();
	};
}