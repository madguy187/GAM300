#pragma once
#include "mono/jit/jit.h"

namespace Eclipse
{
	struct MonoScript
	{
		std::string scriptName{};
		MonoObject* obj = nullptr;
		std::vector<MonoVariable> vars;

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
		bool fixUpdate = false;

		// Manager Functions
		void Init();
		void StartMono();
		void StopMono();
		void Terminate();

		// API Functions
		void Awake(MonoScript* obj);
		void Start(MonoScript* obj);
		void Update(MonoScript* obj);
		void FixedUpdate(MonoScript* obj);

		MonoObject* CreateMonoObject(std::string scriptName, Entity entity);
		MonoObject* CreateObjectFromClass(MonoClass* klass, bool defaultConstructor = true);
		MonoObject* CreateVector3Class(float x, float y, float z);

		std::string GetStringFromField(MonoObject* obj, MonoClass* klass, const char* fieldName);

		void SetFloatFromField(MonoObject* obj, MonoClass* klass, const char* fieldName, float fieldValue);

		MonoClass* GetAPIMonoClass(std::string className);
		MonoClass* GetScriptMonoClass(std::string className);
		MonoMethod* GetMethodFromClass(MonoClass* klass, std::string funcName, int param_count = -1);
		void LoadAllFields(MonoScript* script);

		MonoObject* ExecuteMethod(MonoObject* obj, MonoMethod* method, std::vector<void*> args);

		// Gets image containing all API Scripts
		MonoImage* GetAPIImage();
		// Gets image containing all User scripts
		MonoImage* GetScriptImage();

		void DumpInfoFromImage(MonoImage* _image);
		void DumpInfoFromClass(MonoClass* _class);
	};
}