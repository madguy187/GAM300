#pragma once
#include "mono/jit/jit.h"

namespace Eclipse
{
	struct MonoScript
	{
		std::string scriptName;
		MonoObject* obj;
	};

	class MonoManager
	{
		MonoDomain* domain;
		MonoAssembly* ScriptAssembly;
		MonoAssembly* APIAssembly;
		MonoImage* ScriptImage;
		MonoImage* APIImage;

		// Generates all the scripts into a dll
		void GenerateDLL();
		bool LoadDLLImage(const char* filename, MonoImage*& image, MonoAssembly*& assembly);

		MonoDomain* LoadDomain();
		void UnloadDomain();
		void test();

	public:
		void Init();
		void Update(MonoScript* obj);
		void StopMono();
		void StartMono();
		void Terminate();
		MonoObject* CreateMonoObject(std::string scriptName, Entity entity);

		// Gets image containing all API Scripts
		MonoImage* GetAPIImage();
		// Gets image containing all User scripts
		MonoImage* GetScriptImage();
		void DumpInfoFromImage(MonoImage* _image);
		void DumpInfoFromClass(MonoClass* _class);
	};
}