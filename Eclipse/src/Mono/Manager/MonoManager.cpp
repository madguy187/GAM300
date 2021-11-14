#include "pch.h"
#include "MonoManager.h"

#include <mono/metadata/mono-config.h>
#include <mono/metadata/threads.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/mono-gc.h>
#include <mono/metadata/environment.h>
#include <mono/metadata/debug-helpers.h>
#include <mono/metadata/attrdefs.h>

#include "ECS/ComponentManager/Components/TransformComponent.h"

#include "../Components/s_RigidBodyComponent.h"
#include "../Components/s_LogicalInput.h"
#include "../Components/s_Time.h"
#include "../Components/s_Transform.h"
#include "../Components/s_Quaternion.h"
#include "../Components/s_GameObject.h"
#include "../Components/s_EclipseBehavior.h"
#include "../Components/s_Physics.h"
#include "../Components/s_Cursor.h"
#include "../Components/s_Light.h"
#include "../Components/s_AudioSource.h"


namespace Eclipse
{
	std::list<MonoClass*> GetAssemblyClassList(MonoImage* image)
	{
		std::list<MonoClass*> class_list;

		const MonoTableInfo* table_info = mono_image_get_table_info(image, MONO_TABLE_TYPEDEF);

		int rows = mono_table_info_get_rows(table_info);

		/* For each row, get some of its values */
		for (int i = 0; i < rows; i++)
		{
			MonoClass* _class = nullptr;
			uint32_t cols[MONO_TYPEDEF_SIZE];
			mono_metadata_decode_row(table_info, i, cols, MONO_TYPEDEF_SIZE);
			const char* name = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAME]);
			const char* name_space = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAMESPACE]);
			_class = mono_class_from_name(image, name_space, name);
			class_list.push_back(_class);
		}
		return class_list;
	}

	static MonoObject* CreateGameObject(Entity ent)
	{
		MonoClass* gameobjKlass = mono_class_from_name(engine->mono.GetAPIImage(), "Eclipse", "GameObject");
		if (!gameobjKlass)
		{
			std::cout << "Failed loading class" << std::endl;
			return nullptr;
		}

		MonoObject* obj = mono_object_new(mono_domain_get(), gameobjKlass);
		if (obj == nullptr) {
			std::cout << "Failed loading class instance " << mono_class_get_name(gameobjKlass) << std::endl;
			return nullptr;
		}

		MonoMethod* method = mono_class_get_method_from_name(gameobjKlass, ".ctor", -1);
		if (method == nullptr) {
			std::cout << "Failed loading class method" << std::endl;
			return nullptr;
		}

		void* args[1];
		args[0] = &ent;
		mono_runtime_invoke(method, obj, args, NULL);

		return obj;
	}

	void MonoManager::Init()
	{
		ENGINE_CORE_INFO("Mono: Initialising");

		mono_config_parse(NULL);
		mono_set_dirs("../Dep/mono/lib", "../Dep/mono/etc");

		domain = mono_jit_init_version("Manager", "v4.0.30319");
		mono_thread_set_main(mono_thread_current());

		ENGINE_LOG_ASSERT(domain, "Domain could not be created");

		// RigidBody Internal Calls
		mono_add_internal_call("Eclipse.Rigidbody::Add_Force", AddForce);
		mono_add_internal_call("Eclipse.Rigidbody::getMass", getMass);
		mono_add_internal_call("Eclipse.Rigidbody::getX", getX);
		mono_add_internal_call("Eclipse.Rigidbody::getY", getY);
		mono_add_internal_call("Eclipse.Rigidbody::getZ", getZ);
		mono_add_internal_call("Eclipse.Rigidbody::setGravBool", setGravBool);

		// Input Internal Calls
		mono_add_internal_call("Eclipse.Input::GetKeyDown", GetKeyTriggered);
		mono_add_internal_call("Eclipse.Input::GetKey", GetKeyCurrentByKeyCode);
		mono_add_internal_call("Eclipse.Input::GetAxis", GetMouseAxis);
		mono_add_internal_call("Eclipse.Input::GetAxisRaw", GetRawMouseAxis);

		// Time Internal Calls
		mono_add_internal_call("Eclipse.Time::getDeltaTime", GetDeltaTime);
		mono_add_internal_call("Eclipse.Time::getFixedDeltaTime", GetFixedDeltaTime);

		// GameObject Internal Calls
		mono_add_internal_call("Eclipse.GameObject::setEnabled", setEnabled);
		mono_add_internal_call("Eclipse.GameObject::getBehavior", getBehavior);

		// Transform Internal Calls
		mono_add_internal_call("Eclipse.Transform::RotateEuler", RotateEuler);
		mono_add_internal_call("Eclipse.Transform::GetTransform", GetTransform);
		mono_add_internal_call("Eclipse.Transform::SetTransform", SetTransform);
		mono_add_internal_call("Eclipse.Transform::GetRotation", GetRotation);
		mono_add_internal_call("Eclipse.Transform::SetRotation", SetRotation);
		mono_add_internal_call("Eclipse.Transform::GetRight", GetRight);
		mono_add_internal_call("Eclipse.Transform::GetForward", GetForward);
		mono_add_internal_call("Eclipse.Transform::GetBack", GetBack);
		mono_add_internal_call("Eclipse.Transform::GetLeft", GetLeft);
		mono_add_internal_call("Eclipse.Transform::GetUp", GetUp);
		mono_add_internal_call("Eclipse.Transform::GetDown", GetDown);

		// Quaternion Internal Calls
		mono_add_internal_call("Eclipse.Quaternion::GetEuler", Euler);

		// InvokeFunc Internal Calls
		mono_add_internal_call("Eclipse.EclipseBehavior::InvokeFunc", Invoke);
		mono_add_internal_call("Eclipse.EclipseBehavior::Find", Find);
		mono_add_internal_call("Eclipse.EclipseBehavior::CreateSpotLight", CreateSpotLight);
		mono_add_internal_call("Eclipse.EclipseBehavior::CreatePrefab", CreatePrefab);

		// Physics Internal Calls
		mono_add_internal_call("Eclipse.Physics::RaycastCheck", RaycastFunc);

		// Cursor Internal Calls
		mono_add_internal_call("Eclipse.Cursor::setState", setState);

		// Light Internal Calls
		mono_add_internal_call("Eclipse.Light::SetLightEnabled", SetLightEnabled);
		mono_add_internal_call("Eclipse.Light::SetIntensity", SetIntensity);
		mono_add_internal_call("Eclipse.Light::SetDirection", SetDirection);
		mono_add_internal_call("Eclipse.Light::IncreaseIntensity", IncreaseIntensity);
		mono_add_internal_call("Eclipse.Light::DecreaseIntensity", DecreaseIntensity);
		mono_add_internal_call("Eclipse.Light::GetIntensity", GetIntensity);

		// AudioSource Internal Calls
		mono_add_internal_call("Eclipse.AudioSource::PlayAudio", Play);

		StartMono();
	}

	void MonoManager::LoadVariable(MonoScript* script)
	{
		MonoClass* klass = GetScriptMonoClass(script->scriptName);

		for (auto& var : script->vars)
		{
			if (var.varValue.empty()) continue;

			if (var.type == m_Type::MONO_UNDEFINED || var.type == m_Type::MONO_HEADER) continue;
			else if (var.type == m_Type::MONO_LIGHT)
			{
				mono_field_set_value(
					script->obj,
					mono_class_get_field_from_name(klass, var.varName.c_str()),
					CreateLightClass(std::strtoul(var.varValue.c_str(), 0, 10))
				);
			}
			else if (var.type == m_Type::MONO_AUDIO)
			{
				mono_field_set_value(
					script->obj,
					mono_class_get_field_from_name(klass, var.varName.c_str()),
					CreateAudioSourceClass(std::strtoul(var.varValue.c_str(), 0, 10))
				);
			}
			else if (var.type == m_Type::MONO_FLOAT)
			{
				float temp = std::stof(var.varValue);
				mono_field_set_value(
					script->obj,
					mono_class_get_field_from_name(klass, var.varName.c_str()),
					&temp);
			}
			else if (var.type == m_Type::MONO_GAMEOBJECT)
			{
				mono_field_set_value(
					script->obj,
					mono_class_get_field_from_name(klass, var.varName.c_str()),
					CreateGameObjectClass(std::strtoul(var.varValue.c_str(), 0, 10), "")
				);
			}
			else
			{
				ENGINE_CORE_INFO("Variable type cannot be added because it is not recognised.");
			}
		}
	}

	void MonoManager::Awake(MonoScript* obj)
	{
		MonoClass* klass = mono_class_from_name(ScriptImage, "", obj->scriptName.c_str());

		if (klass == nullptr) {
			std::cout << "Failed loading class, MonoVec3" << std::endl;
			return;
		}

		MonoMethod* m_update = mono_class_get_method_from_name(klass, "Awake", -1);
		if (!m_update) return;

		mono_runtime_invoke(m_update, obj->obj, nullptr, NULL);
	}

	void MonoManager::Start(MonoScript* obj)
	{
		MonoClass* klass = mono_class_from_name(ScriptImage, "", obj->scriptName.c_str());

		if (klass == nullptr) {
			std::cout << "Failed loading class, MonoVec3" << std::endl;
			return;
		}

		MonoMethod* m_update = mono_class_get_method_from_name(klass, "Start", -1);
		if (!m_update) return;

		mono_runtime_invoke(m_update, obj->obj, nullptr, NULL);
	}

	void MonoManager::LoadAllScripts()
	{
		if (!CheckIfScriptCompiled()) return;
		const MonoTableInfo* table_info = mono_image_get_table_info(ScriptImage, MONO_TABLE_TYPEDEF);

		int rows = mono_table_info_get_rows(table_info);

		/* For each row, get some of its values */
		for (int i = 1; i < rows; i++)
		{
			uint32_t cols[MONO_TYPEDEF_SIZE];
			mono_metadata_decode_row(table_info, i, cols, MONO_TYPEDEF_SIZE);
			const char* name = mono_metadata_string_heap(ScriptImage, cols[MONO_TYPEDEF_NAME]);

			int index = CheckIfScriptExist(name);
			if (index == -1)
			{
				UserImplementedScriptList.push_back(MonoScript{});
				UserImplementedScriptList.back().scriptName = name;
				LoadAllFields(&UserImplementedScriptList.back());
			}
			else
				LoadAllFields(&UserImplementedScriptList[index]);
		}
	}

	bool MonoManager::CheckIfScriptCompiled()
	{
		TCHAR buffer[MAX_PATH] = { 0 };
		GetModuleFileName(NULL, buffer, MAX_PATH); // get exe buff
		std::wstring wBuffer{ buffer }; // convert buffer into wstring
		std::string exePath{ wBuffer.begin(), wBuffer.end() }; // convert wstring into string

		// removes path from bin onwards
		size_t index = exePath.find("bin");
		if (index == exePath.npos) return false;
		exePath = exePath.substr(0, index);

		// combine strings to make api and script path
		std::string outputTxtFile = exePath + "Eclipse//mcs_script_output.txt";

		const std::ifstream input_stream(outputTxtFile, std::ios_base::binary);

		if (input_stream.fail()) {
			return true;
		}

		std::stringstream streambuffer;
		streambuffer << input_stream.rdbuf();
		std::string output{ streambuffer.str() };

		if (output.find("succeed") != output.npos) return true;

		return false;
	}

	int MonoManager::CheckIfScriptExist(std::string scriptName)
	{
		for (int i = 0; i < UserImplementedScriptList.size(); i++)
			if (UserImplementedScriptList[i].scriptName == scriptName) return i;
		
		return -1;
	}

	void MonoManager::LoadAllFields(MonoScript* script)
	{
		if (script->scriptName.empty()) return;
		size_t i = 0;
		MonoClass* klass = GetScriptMonoClass(script->scriptName);
		MonoClass* attrKlass = mono_class_from_name(engine->mono.GetAPIImage(), "", "Header");
		MonoClassField* field;
		void* iter = NULL;

		while (true)
		{
			field = mono_class_get_fields(klass, &iter);
			if (!field) break;
			// check for attributes
			MonoCustomAttrInfo* attrInfo = mono_custom_attrs_from_field(klass, field);

			if (attrInfo != nullptr)
			{
				if (attrKlass != nullptr)
				{
					if (mono_custom_attrs_has_attr(attrInfo, attrKlass))
					{
						MonoObject* attrObj = mono_custom_attrs_get_attr(attrInfo, attrKlass);
						MonoVariable var;
						var.type = m_Type::MONO_HEADER;
						var.varName = GetStringFromField(attrObj, attrKlass, "name");

						if (CheckIfFieldExist(script, var.varName, i))
							i++;
						else
							script->vars.insert(script->vars.begin() + i++, var);
					}
				}
			}

			if ((mono_field_get_flags(field) & MONO_FIELD_ATTR_FIELD_ACCESS_MASK) == MONO_FIELD_ATTR_PRIVATE) continue;
			
			mono_custom_attrs_free(attrInfo);

			MonoVariable var;

			int typeInt = mono_type_get_type(mono_field_get_type(field));
			var.varName = mono_field_get_name(field);

			MonoClass* fieldklass = mono_type_get_class(mono_field_get_type(field));

			switch (typeInt)
			{
			case MONO_TYPE_R4:
				var.type = m_Type::MONO_FLOAT;
				break;
			case MONO_TYPE_VALUETYPE:
				
				if (fieldklass == GetAPIMonoClass("Light"))
					var.type = m_Type::MONO_LIGHT;
				else if (fieldklass == GetAPIMonoClass("AudioSource"))
					var.type = m_Type::MONO_AUDIO;
				else
					var.type = m_Type::MONO_UNDEFINED;				
				break;
			case MONO_TYPE_CLASS:
				if (fieldklass == GetAPIMonoClass("GameObject"))
					var.type = m_Type::MONO_GAMEOBJECT;
				break;
			}

			if (!CheckIfFieldExist(script, var.varName, i))
				script->vars.insert(script->vars.begin() + i, var);

			i++;
			
			
		}
	}

	bool MonoManager::CheckIfFieldExist(MonoScript* script, std::string& fieldName, size_t index)
	{
		if (index >= script->vars.size()) return false;

		if (script->vars[index].varName == fieldName) return true;
		return false;
	}

	void MonoManager::Update(MonoScript* obj)
	{
		MonoClass* klass = mono_class_from_name(ScriptImage, "", obj->scriptName.c_str());

		if (klass == nullptr) {
			std::cout << "Failed loading class, MonoVec3" << std::endl;
			return;
		}

		MonoMethod* m_update = mono_class_get_method_from_name(klass, "Update", -1);
		if (!m_update) return;

		mono_runtime_invoke(m_update, obj->obj, nullptr, NULL);
	}

	void MonoManager::FixedUpdate(MonoScript* obj)
	{
		MonoClass* klass = mono_class_from_name(ScriptImage, "", obj->scriptName.c_str());

		if (klass == nullptr) {
			std::cout << "Failed loading class, MonoVec3" << std::endl;
			return;
		}

		MonoMethod* m_update = mono_class_get_method_from_name(klass, "FixedUpdate", -1);
		if (!m_update) return;

		mono_runtime_invoke(m_update, obj->obj, nullptr, NULL);
	}

	void MonoManager::OnCollision(Entity ent)
	{
		auto& scriptComp = engine->world.GetComponent<ScriptComponent>(ent);

		for (auto& script : scriptComp.scriptList)
		{
			MonoClass* klass = mono_class_from_name(ScriptImage, "", script->scriptName.c_str());

			if (klass == nullptr) {
				std::cout << "Failed loading class, MonoVec3" << std::endl;
				continue;
			}

			MonoMethod* m_update = mono_class_get_method_from_name(klass, "OnCollision", -1);
			if (!m_update) continue;

			mono_runtime_invoke(m_update, script->obj, nullptr, NULL);
		}
	}

	void MonoManager::StopMono()
	{
		mono_image_close(ScriptImage);
		ScriptImage = nullptr;

		mono_image_close(APIImage);
		APIImage = nullptr;
		
		UnloadDomain();
	}

	void MonoManager::StartMono()
	{
		if (ScriptImage || APIImage)
			StopMono();

		GenerateDLL();

		LoadDomain();
		LoadDLLImage("../EclipseScriptsAPI.dll", APIImage, APIAssembly);
		LoadDLLImage("../EclipseScripts.dll", ScriptImage, ScriptAssembly);

		LoadAllScripts();
	}

	void MonoManager::Terminate()
	{
		if (ScriptImage)
		{
			mono_image_close(ScriptImage);
			ScriptImage = nullptr;
		}

		if (APIImage)
		{
			mono_image_close(APIImage);
			APIImage = nullptr;
		}
			
		mono_jit_cleanup(domain);
	}

	void MonoManager::UpdateInvokers()
	{
		InvokeContainer.erase(std::remove_if(InvokeContainer.begin(),
			InvokeContainer.end(),
			[&](auto& invokee) -> bool
			{ 
				invokee.timer -= engine->Game_Clock.get_DeltaTime();
				if (invokee.timer < 0.0f)
				{
					ExecuteMethod(invokee.script->obj, invokee.method, std::vector<void*>{});
					return true;
				}

				return false;
			}),
			InvokeContainer.end());
	}

	void MonoManager::AddInvoke(MonoScript* _script, float _timer, MonoMethod* _method)
	{
		InvokeContainer.push_back({ _script, _timer, _method });
	}

	MonoScript* MonoManager::GetScriptPointerByName(const std::string& name)
	{
		for (auto& script : UserImplementedScriptList)
		{
			if (script.scriptName == name) return &script;
		}

		return nullptr;
	}

	MonoObject* MonoManager::CreateMonoObject(std::string scriptName, Entity entity)
	{
		if (scriptName == "") return nullptr;
		MonoClass* script = mono_class_from_name(ScriptImage, "", scriptName.c_str());
		if (!script)
		{
			std::cout << "Failed loading class" << std::endl;
			return nullptr;
		}

		MonoObject* obj = mono_object_new(mono_domain_get(), script);
		if (obj == nullptr) {
			std::cout << "Failed loading class instance " << mono_class_get_name(script) << std::endl;
			return nullptr;
		}

		mono_runtime_object_init(obj);

		MonoClass* base = mono_class_from_name(APIImage, "Eclipse", "EclipseBehavior");
		if (!base)
		{
			std::cout << "Failed loading class" << std::endl;
			return nullptr;
		}

		MonoMethod* method = mono_class_get_method_from_name(base, "InitBehavior", 3);
		if (method == nullptr) {
			std::cout << "Failed loading class method" << std::endl;
			return nullptr;
		}

		void* args[3];
		uint32_t handle = mono_gchandle_new(obj, true);
		args[0] = &handle;
		Entity ent = entity;
		args[1] = &ent;
		args[2] = mono_string_new(mono_domain_get(), scriptName.c_str());
		mono_runtime_invoke(method, obj, args, NULL);

		return obj;
	}

	MonoClass* MonoManager::GetAPIMonoClass(std::string className)
	{
		MonoClass* klass = mono_class_from_name(APIImage, "Eclipse", className.c_str());
		if (!klass)
		{
			std::cout << "Failed loading class: " << className << std::endl;
			return nullptr;
		}

		return klass;
	}
	
	MonoClass* MonoManager::GetScriptMonoClass(std::string className)
	{
		MonoClass* klass = mono_class_from_name(ScriptImage, "", className.c_str());
		if (!klass)
		{
			std::cout << "Failed loading class: " << className << std::endl;
			return nullptr;
		}

		return klass;
	}

	MonoObject* MonoManager::CreateObjectFromClass(MonoClass* klass, bool defaultConstructor)
	{
		if (!klass) return nullptr;

		MonoObject* obj = mono_object_new(mono_domain_get(), klass);
		if (obj == nullptr) {
			std::cout << "Failed loading class instance " << mono_class_get_name(klass) << std::endl;
			return nullptr;
		}

		if (defaultConstructor)
			mono_runtime_object_init(obj);

		return obj;
	}

	MonoObject* MonoManager::CreateVector3Class(float x, float y, float z)
	{
		MonoClass* klass = GetAPIMonoClass("Vector3");
		MonoObject* obj = CreateObjectFromClass(klass, false);
		if (!obj) return nullptr;

		MonoMethod* method = GetMethodFromClass(klass, ".ctor", 3);
		if (!method) return nullptr;

		std::vector<void*> args;
		args.push_back(&x);
		args.push_back(&y);
		args.push_back(&z);

		ExecuteMethod(obj, method, args);

		return obj;
	}

	ECVec3 MonoManager::ConvertVectorToECVec(MonoObject* vec)
	{
		MonoClass* klass = GetAPIMonoClass("Vector3");
		float x;
		float y;
		float z;
		GetFloatFromField(vec, klass, "x", x);
		GetFloatFromField(vec, klass, "y", y);
		GetFloatFromField(vec, klass, "z", z);
		return ECVec3{ x, y, z };
	}

	ECVec3 MonoManager::ConvertQuaternionToECVec3(MonoObject* vec)
	{
		float w;
		GetFloatFromField(vec, GetAPIMonoClass("Quaternion"), "w", w);
		float x;
		GetFloatFromField(vec, GetAPIMonoClass("Quaternion"), "x", x);
		float y;
		GetFloatFromField(vec, GetAPIMonoClass("Quaternion"), "y", y);
		float z;
		GetFloatFromField(vec, GetAPIMonoClass("Quaternion"), "z", z);

		glm::quat quad{ w, x, y, z };
		PxQuat tempquat{ x,y,z,w };
		//auto rot = glm::degrees(glm::eulerAngles(quad));
		ECVec3 VecRot = engine->gPhysics.QuattoAngles(tempquat);
		return VecRot;
	}

	MonoObject* MonoManager::CreateQuaternionClass(float x, float y, float z)
	{
		MonoClass* klass = GetAPIMonoClass("Quaternion");
		MonoObject* obj = CreateObjectFromClass(klass, false);
		if (!obj) return nullptr;

		MonoMethod* method = GetMethodFromClass(klass, ".ctor", 4);
		if (!method) return nullptr;

		PxQuat quad = engine->gPhysics.AnglestoQuat(x,y,z);
		//glm::vec3 vec{ x, y, z };
		//glm::quat quad{ glm::radians(vec) };

		std::vector<void*> args;
		args.push_back(&quad.w);
		args.push_back(&quad.x);
		args.push_back(&quad.y);
		args.push_back(&quad.z);

		ExecuteMethod(obj, method, args);

		return obj;
	}

	MonoObject* MonoManager::CreateRayCastHit(Entity ent, float x, float y, float z)
	{
		MonoClass* klass = GetAPIMonoClass("RaycastHit");
		MonoObject* obj = CreateObjectFromClass(klass, false);
		std::vector<void*> args;
		args.push_back(&ent);
		args.push_back(&x);
		args.push_back(&y);
		args.push_back(&z);
		ExecuteMethod(obj, GetMethodFromClass(klass, ".ctor", 4), args);
		
		return obj;
	}

	MonoObject* MonoManager::CreateLightClass(Entity ent)
	{
		MonoClass* klass = GetAPIMonoClass("Light");
		MonoObject* obj = CreateObjectFromClass(klass, false);
		std::vector<void*> args;
		args.push_back(&ent);
		ExecuteMethod(obj, GetMethodFromClass(klass, ".ctor", 1), args);

		return obj;
	}

	MonoObject* MonoManager::CreateAudioSourceClass(Entity ent)
	{
		MonoClass* klass = GetAPIMonoClass("AudioSource");
		MonoObject* obj = CreateObjectFromClass(klass, false);
		std::vector<void*> args;
		args.push_back(&ent);
		ExecuteMethod(obj, GetMethodFromClass(klass, ".ctor", 1), args);

		return obj;
	}

	MonoObject* MonoManager::CreateGameObjectClass(Entity ent, std::string scriptName)
	{
		MonoClass* klass = GetAPIMonoClass("GameObject");
		MonoObject* obj = CreateObjectFromClass(klass, false);


		MonoString* str = mono_string_new(mono_domain_get(), scriptName.c_str());

		std::vector<void*> args;
		args.push_back(&ent);
		args.push_back(str);

		ExecuteMethod(obj, GetMethodFromClass(klass, ".ctor", 2), args);

		return obj;
	}

	std::string MonoManager::GetStringFromField(MonoObject* obj, MonoClass* klass, const char* fieldName)
	{
		MonoString* stringField;

		MonoClassField* field = mono_class_get_field_from_name(klass, fieldName);
		if (!field) {
			ENGINE_CORE_WARN("Can't find field in klass");
			return std::string{};
		}

		if (mono_type_get_type(mono_field_get_type(field)) != MONO_TYPE_STRING) {
			ENGINE_CORE_WARN("Trying to set a field that is not a string field");
			return std::string{};
		}

		mono_field_get_value(obj, field, &stringField);

		return mono_string_to_utf8(stringField);
	}

	void MonoManager::SetFloatFromField(MonoObject* obj, MonoClass* klass, const char* fieldName, float fieldValue)
	{
		MonoClassField* field = mono_class_get_field_from_name(klass, fieldName);
		if (!field) {
			ENGINE_CORE_WARN("Can't find field in klass");
			return;
		}

		mono_field_set_value(obj, field, &fieldValue);
	}

	bool MonoManager::GetFloatFromField(MonoObject* obj, MonoClass* klass, const char* fieldName, float& value)
	{
		MonoClassField* field = mono_class_get_field_from_name(klass, fieldName);
		if (!field) {
			ENGINE_CORE_WARN("Can't find field in klass");
			return false;
		}

		mono_field_get_value(obj, field, (void*)&value);

		return true;
	}

	MonoMethod* MonoManager::GetMethodFromClass(MonoClass* klass, std::string funcName, int param_count)
	{
		MonoMethod* method = mono_class_get_method_from_name(klass, funcName.c_str(), param_count);
		if (method == nullptr) {
			std::cout << "Failed loading class method" << std::endl;
			return nullptr;
		}

		return method;
	}

	MonoObject* MonoManager::ExecuteMethod(MonoObject* obj, MonoMethod* method, std::vector<void*> args)
	{
		if (!obj && !method) return nullptr;
		return mono_runtime_invoke(method, obj, args.data(), NULL);
	}

	MonoDomain* MonoManager::LoadDomain()
	{
		MonoDomain* newDomain = mono_domain_create_appdomain("Child Domain", NULL);

		if (!newDomain) {
			printf("Error creating domain\n");
			return nullptr;
		}

		if (!mono_domain_set(newDomain, false)) {
			printf("Error setting domain\n");
			return nullptr;
		}

		return mono_domain_get();
	}

	void MonoManager::UnloadDomain()
	{
		MonoDomain* old_domain = mono_domain_get();
		MonoDomain* root_domain = mono_get_root_domain();
		if (old_domain && old_domain != root_domain) {

			if (!mono_domain_set(mono_get_root_domain(), false))
				printf("Error setting domain\n");

			mono_domain_unload(old_domain);
		}

		mono_gc_collect(mono_gc_max_generation());
	}

	void MonoManager::GenerateDLL()
	{
		ENGINE_CORE_INFO("Mono: Generating DLLs");
		//system("sh -c ../Dep/mono/bin/mcs_api.bat");

		TCHAR buffer[MAX_PATH] = { 0 };
		GetModuleFileName(NULL, buffer, MAX_PATH); // get exe buff
		std::wstring wBuffer{ buffer }; // convert buffer into wstring
		std::string exePath{ wBuffer.begin(), wBuffer.end() }; // convert wstring into string
		std::string cmdCall = "cmd /C "; // string for command call

		// removes path from bin onwards
		size_t index = exePath.find("bin");
		if (index == exePath.npos) return;
		exePath = exePath.substr(0, index);

		// combine strings to make api and script path
		std::string apiPath = '"' + exePath + "Dep//mono//bin//mcs_api.bat" + '"';
		std::string scriptPath = '"' + exePath + "Dep//mono//bin//mcs_scripts.bat" + '"';

		// system call path
		system((cmdCall + apiPath).c_str());
		system((cmdCall + scriptPath).c_str());
		ENGINE_CORE_INFO("Mono: Successfully Generate DLLs");
	}

	bool MonoManager::LoadDLLImage(const char* filename, MonoImage*& image, MonoAssembly*& assembly)
	{
		/*std::ifstream t(filename);
		std::string str((std::istreambuf_iterator<char>(t)),
			std::istreambuf_iterator<char>());*/

		char* arr = nullptr;
		uint32_t len = 0;
		std::ifstream file(filename, std::ifstream::binary);
		if (file) {
			// get length of file:
			file.seekg(0, file.end);
			auto length = file.tellg();
			len = static_cast<uint32_t>(length);
			file.seekg(0, file.beg);

			arr = new char[length];
			file.read(arr, length);
			file.close();
		}

		MonoImageOpenStatus status;
		image = nullptr;
		image = mono_image_open_from_data_with_name(arr, len, true /* copy data */, &status, false /* ref only */, filename);
		bool result = true;
		if (image)
			assembly = mono_assembly_load_from_full(image, filename, &status, false);
		else
			result = false;

		delete[] arr;
		return result;
	}

	MonoImage* MonoManager::GetAPIImage()
	{
		return APIImage;
	}

	MonoImage* MonoManager::GetScriptImage()
	{
		return ScriptImage;
	}

	void MonoManager::DumpInfoFromImage(MonoImage* _image)
	{
		std::cout << "#####################################" << std::endl;
		std::cout << mono_image_get_name(_image) << std::endl;
		std::cout << "#####################################" << std::endl;

		const MonoTableInfo* table_info = mono_image_get_table_info(_image, MONO_TABLE_TYPEDEF);

		int rows = mono_table_info_get_rows(table_info);

		/* For each row, get some of its values */
		for (int i = 0; i < rows; i++)
		{
			MonoClass* _class = nullptr;
			uint32_t cols[MONO_TYPEDEF_SIZE];
			mono_metadata_decode_row(table_info, i, cols, MONO_TYPEDEF_SIZE);
			const char* name = mono_metadata_string_heap(_image, cols[MONO_TYPEDEF_NAME]);
			const char* name_space = mono_metadata_string_heap(_image, cols[MONO_TYPEDEF_NAMESPACE]);
			_class = mono_class_from_name(_image, name_space, name);

			void* iter = NULL;
			MonoMethod* method;
			while (true)
			{
				method = mono_class_get_methods(_class, &iter);
				if (!method) break;
				std::cout << mono_method_full_name(method, 1) << std::endl;
			}
		}
	}

	void MonoManager::DumpInfoFromClass(MonoClass* _class)
	{
		std::cout << "#####################################" << std::endl;
		std::cout << mono_class_get_name(_class) << std::endl;
		std::cout << "#####################################" << std::endl;

		void* iter = NULL;
		MonoMethod* method;
		while (true)
		{
			method = mono_class_get_methods(_class, &iter);
			if (!method) break;
			std::cout << mono_method_full_name(method, 1) << std::endl;
		}

		//void* iter = NULL;
		//MonoMethod* method = nullptr;
		//while (method == mono_class_get_methods(_class, &iter))
		//{
		//	std::cout << mono_method_get_name(method) << std::endl;
		//	std::cout << mono_method_full_name(method, 1) << std::endl;
		//}

		std::cout << std::endl;
	}
}