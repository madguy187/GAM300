#pragma once
#include "mono/jit/jit.h"

namespace Eclipse
{
	class MonoManager
	{
		MonoDomain* domain;
		MonoAssembly* assembly;
		MonoImage* image;
	public:
		void Init();
		void Update();
		void GenerateDLL();
	};
}