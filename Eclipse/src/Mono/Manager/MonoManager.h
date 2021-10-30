#pragma once
#include "mono/jit/jit.h"

namespace Eclipse
{
	struct MonoScript
	{
		std::string scriptName{};
		MonoObject* obj = nullptr;

		bool operator==(const MonoScript& rhs) const
		{
			if (scriptName == rhs.scriptName)
				return true;

			return false;
		}
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

	public:
		void Init();
		void Start(MonoScript* obj);
		void Update(MonoScript* obj);
		void StartMono();
		void StopMono();
		void Terminate();

		MonoObject* CreateMonoObject(std::string scriptName, Entity entity);
		MonoClass* GetMonoClass(std::string className);
		MonoObject* CreateObjectFromClass(MonoClass* klass);
		MonoMethod* GetMethodFromClass(MonoClass* klass, std::string funcName);
		bool ExecuteMethod(MonoObject* obj, MonoMethod* method, std::vector<void*> args);

		// Gets image containing all API Scripts
		MonoImage* GetAPIImage();
		// Gets image containing all User scripts
		MonoImage* GetScriptImage();
		void DumpInfoFromImage(MonoImage* _image);
		void DumpInfoFromClass(MonoClass* _class);
	};
}