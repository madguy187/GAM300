#pragma once
#include "Global.h"
#include "mono/jit/jit.h"
#include "Library/Math/Vector.h"

namespace Eclipse
{
	struct MonoVariable
	{
		m_Type type = m_Type::MONO_UNDEFINED;
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

	class MonoManager
	{
		MonoDomain* domain = nullptr;
		MonoAssembly* ScriptAssembly = nullptr;
		MonoAssembly* APIAssembly = nullptr;
		MonoImage* ScriptImage = nullptr;
		MonoImage* APIImage = nullptr;

		std::vector<MonoScript> UserImplementedScriptList;
		std::vector<InvokeFunc> InvokeContainer;

		// Generates all the scripts into a dll
		void GenerateDLL();
		bool LoadDLLImage(const char* filename, MonoImage*& image, MonoAssembly*& assembly);

		MonoDomain* LoadDomain();
		void UnloadDomain();

		int CheckIfScriptExist(std::string scriptName);
		bool CheckIfFieldExist(MonoScript* script, std::string& fieldName);
		void LoadAllFields(MonoScript* script);

	public:
		bool fixUpdate = false;

		// Manager Functions
		void Init();
		void StartMono();
		void StopMono();
		void Terminate();
		void UpdateInvokers();
		void AddInvoke(MonoScript* _script, float _timer, MonoMethod* _method);
		MonoScript* GetScriptPointerByName(const std::string& name);
		void LoadVariable(MonoScript* script);

		// API Functions
		void Awake(MonoScript* obj);
		void Start(MonoScript* obj);
		void Update(MonoScript* obj);
		void FixedUpdate(MonoScript* obj);
		void OnCollision(Entity ent);

		MonoObject* CreateMonoObject(std::string scriptName, Entity entity);
		MonoObject* CreateObjectFromClass(MonoClass* klass, bool defaultConstructor = true);
		MonoObject* CreateVector3Class(float x, float y, float z);
		MonoObject* CreateQuaternionClass(float x, float y, float z);
		MonoObject* CreateRayCastHit(Entity ent, float x, float y, float z);
		MonoObject* CreateLightClass(Entity ent);
		MonoObject* CreateAudioSourceClass(Entity ent);
		MonoObject* CreateGameObjectClass(Entity ent, std::string scriptName);
		MonoObject* CreateLayerMaskClass(std::string mask);
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
		bool CheckIfScriptCompiled();

		// Gets image containing all API Scripts
		MonoImage* GetAPIImage();
		// Gets image containing all User scripts
		MonoImage* GetScriptImage();

		void DumpInfoFromImage(MonoImage* _image);
		void DumpInfoFromClass(MonoClass* _class);
	};
}