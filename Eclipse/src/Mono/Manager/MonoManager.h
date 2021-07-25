#pragma once
#include "mono/jit/jit.h"

namespace Eclipse
{
	class MonoManager
	{
		MonoDomain* domain;
		MonoAssembly* ScriptAssembly;
		MonoAssembly* APIAssembly;
		MonoImage* ScriptImage;
		MonoImage* APIImage;

		std::vector<MonoObject*> objects;
	public:
		void Init();
		void Update();
		void GenerateDLL();
		MonoImage* GetAPIImage();
		MonoImage* GetScriptImage();

		void DumpInfoFromImage(MonoImage* _image);
		void DumpInfoFromClass(MonoClass* _class);
	};
}