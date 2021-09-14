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

		// Generates all the scripts into a dll
		void GenerateDLL();
		void CreateScriptObjects();

	public:
		void Init();
		void Update();
		void StopMono();

		// Gets image containing all API Scripts
		MonoImage* GetAPIImage();
		// Gets image containing all User scripts
		MonoImage* GetScriptImage();
		void DumpInfoFromImage(MonoImage* _image);
		void DumpInfoFromClass(MonoClass* _class);
	};
}