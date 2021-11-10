#pragma once
#include "Global.h"
#include "mono/jit/jit.h"
#include "Library/Math/Vector.h"

namespace Eclipse
{
	struct MonoVariable
	{
		m_Type type = m_Type::MONO_UNDEFINED;
		MonoClassField* field;
		std::string varName = "";
		std::string varValue = "";
	};

	struct MonoScript
	{
		std::string scriptName{};
		MonoObject* obj = nullptr;
		std::vector<MonoVariable> vars;
		bool enabled = true;

		bool operator==(const MonoScript& rhs) const
		{
			if (scriptName == rhs.scriptName)
				return true;

			return false;
		}
	};

	struct InvokeFunc
	{
		MonoScript* script = nullptr;
		float timer = 0.0f;
		MonoMethod* method = nullptr;
	};

	struct M_LIGHT
	{
		uint32_t ent;
	};

	class MonoManager
	{
		MonoDomain* domain;
		MonoAssembly* ScriptAssembly;
		MonoAssembly* APIAssembly;
		MonoImage* ScriptImage;
		MonoImage* APIImage;

		std::vector<MonoScript> UserImplementedScriptList;
		std::vector<InvokeFunc> InvokeContainer;

		// Generates all the scripts into a dll
		void GenerateDLL();
		bool LoadDLLImage(const char* filename, MonoImage*& image, MonoAssembly*& assembly);

		MonoDomain* LoadDomain();
		void UnloadDomain();

	public:
		bool fixUpdate = false;

		// Manager Functions
		void Init();
		void LoadVariable(MonoScript*& script);
		void StartMono();
		void StopMono();
		void Terminate();
		void UpdateInvokers();
		void AddInvoke(MonoScript* _script, float _timer, MonoMethod* _method);
		MonoScript* GetScriptPointerByName(const std::string& name);

		// API Functions
		void Awake(MonoScript* obj);
		void Start(MonoScript* obj);
		void Update(MonoScript* obj);
		void FixedUpdate(MonoScript* obj);

		MonoObject* CreateMonoObject(std::string scriptName, Entity entity);
		MonoObject* CreateObjectFromClass(MonoClass* klass, bool defaultConstructor = true);
		MonoObject* CreateVector3Class(float x, float y, float z);
		MonoObject* CreateQuaternionClass(float x, float y, float z);
		MonoObject* CreateRayCastHit(float x, float y, float z);
		MonoObject* CreateLightClass(Entity ent);
		ECVec3 ConvertVectorToECVec(MonoObject* vec);
		ECVec3 ConvertQuaternionToECVec3(MonoObject* vec);

		std::string GetStringFromField(MonoObject* obj, MonoClass* klass, const char* fieldName);

		void SetFloatFromField(MonoObject* obj, MonoClass* klass, const char* fieldName, float fieldValue);
		bool GetFloatFromField(MonoObject* obj, MonoClass* klass, const char* fieldName, float& value);

		MonoClass* GetAPIMonoClass(std::string className);
		MonoClass* GetScriptMonoClass(std::string className);
		MonoMethod* GetMethodFromClass(MonoClass* klass, std::string funcName, int param_count = -1);
		MonoObject* ExecuteMethod(MonoObject* obj, MonoMethod* method, std::vector<void*> args);

		void LoadAllScripts();
		bool CheckIfScriptExist(std::string scriptName);
		void LoadAllFields(MonoScript* script);
		bool CheckIfFieldExist(MonoScript* script, std::string& fieldName, size_t index);

		// Gets image containing all API Scripts
		MonoImage* GetAPIImage();
		// Gets image containing all User scripts
		MonoImage* GetScriptImage();

		void DumpInfoFromImage(MonoImage* _image);
		void DumpInfoFromClass(MonoClass* _class);
	};
}