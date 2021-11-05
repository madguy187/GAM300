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
		uint32_t handle = mono_gchandle_new(obj, true);

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

		mono_add_internal_call("Eclipse.RigidBody::Add_Force", SetForce);
		mono_add_internal_call("Eclipse.Input::GetButtonDown", GetKeyCurrentByName);
		mono_add_internal_call("Eclipse.Input::GetKey", GetKeyCurrentByKeyCode);
		mono_add_internal_call("Eclipse.Input::GetAxis", GetMouseAxis);
		mono_add_internal_call("Eclipse.Time::getDeltaTime", GetDeltaTime);
		mono_add_internal_call("Eclipse.Time::getFixedDeltaTime", GetFixedDeltaTime);
	}

	void MonoManager::Awake(MonoScript* obj)
	{
		MonoClass* klass = mono_class_from_name(ScriptImage, "", obj->scriptName.c_str());

		if (klass == nullptr) {
			std::cout << "Failed loading class, MonoVec3" << std::endl;
			return;
		}

		MonoMethod* m_update = mono_class_get_method_from_name(klass, "Awake", -1);
		if (!m_update)
		{
			std::cout << "Failed to get method" << std::endl;
			return;
		}

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
		if (!m_update)
		{
			std::cout << "Failed to get method" << std::endl;
			return;
		}

		mono_runtime_invoke(m_update, obj->obj, nullptr, NULL);
	}

	std::unordered_map<std::string, std::string> MonoManager::GetAllFields(MonoClass* klass)
	{
		MonoClassField* field;
		void* iter = NULL;
		std::unordered_map<std::string, std::string> fields;
		while ((field = mono_class_get_fields(klass, &iter)))
		{
			// check for attributes
			MonoCustomAttrInfo* attrInfo = mono_custom_attrs_from_field(klass, field);

			if ((mono_field_get_flags(field) & MONO_FIELD_ATTR_FIELD_ACCESS_MASK) == MONO_FIELD_ATTR_PRIVATE) continue;

			if (attrInfo != nullptr)
			{
				MonoClass* attrKlass = mono_class_from_name(engine->mono.GetAPIImage(), "", "Header");
				
				if (attrKlass != nullptr)
				{
					if (mono_custom_attrs_has_attr(attrInfo, attrKlass))
					{
						MonoObject* attrObj = mono_custom_attrs_get_attr(attrInfo, attrKlass);
						fields.insert(std::make_pair(GetStringFromField(attrObj, attrKlass, "name"), "Header"));
					}
				}
			}
			
			mono_custom_attrs_free(attrInfo);
			fields.insert(std::make_pair(mono_field_get_name(field), "Variable"));
		}

		return fields;
	}

	void MonoManager::Update(MonoScript* obj)
	{
		MonoClass* klass = mono_class_from_name(ScriptImage, "", obj->scriptName.c_str());

		if (klass == nullptr) {
			std::cout << "Failed loading class, MonoVec3" << std::endl;
			return;
		}

		MonoMethod* m_update = mono_class_get_method_from_name(klass, "Update", -1);
		if (!m_update)
		{
			std::cout << "Failed to get method" << std::endl;
			return;
		}

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
		if (!m_update)
		{
			std::cout << "Failed to get method" << std::endl;
			return;
		}

		mono_runtime_invoke(m_update, obj->obj, nullptr, NULL);
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
		GenerateDLL();

		LoadDomain();
		LoadDLLImage("../EclipseScriptsAPI.dll", APIImage, APIAssembly);
		LoadDLLImage("../EclipseScripts.dll", ScriptImage, ScriptAssembly);
	}

	void MonoManager::Terminate()
	{
		if (ScriptImage)
			mono_image_close(ScriptImage);

		if (APIImage)
			mono_image_close(APIImage);
			
		mono_jit_cleanup(domain);
	}

	MonoObject* MonoManager::CreateMonoObject(std::string scriptName, Entity entity)
	{
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

		MonoClass* base = mono_class_from_name(APIImage, "Eclipse", "EclipseBehavior");
		if (!base)
		{
			std::cout << "Failed loading class" << std::endl;
			return nullptr;
		}

		MonoMethod* method = mono_class_get_method_from_name(base, "InitBehavior", 2);
		if (method == nullptr) {
			std::cout << "Failed loading class method" << std::endl;
			return nullptr;
		}

		mono_runtime_object_init(obj);

		void* args[2];
		uint32_t handle = mono_gchandle_new(obj, true);
		args[0] = &handle;
		Entity ent = entity;
		args[1] = &ent;
		mono_runtime_invoke(method, obj, args, NULL);

		return obj;
	}

	MonoClass* MonoManager::GetMonoClass(std::string className)
	{
		MonoClass* klass = mono_class_from_name(APIImage, "Eclipse", className.c_str());
		if (!klass)
		{
			std::cout << "Failed loading class: " << className << std::endl;
			return nullptr;
		}

		return klass;
	}

	MonoObject* MonoManager::CreateObjectFromClass(MonoClass* klass)
	{
		if (!klass) return nullptr;

		MonoObject* obj = mono_object_new(mono_domain_get(), klass);
		if (obj == nullptr) {
			std::cout << "Failed loading class instance " << mono_class_get_name(klass) << std::endl;
			return nullptr;
		}

		mono_runtime_object_init(obj);

		return obj;
	}

	std::string MonoManager::GetStringFromField(MonoObject* obj, MonoClass* klass, const char* fieldName)
	{
		MonoString* stringField;

		MonoClassField* field = mono_class_get_field_from_name(klass, fieldName);
		if (!field) {
			fprintf(stderr, "Can't find field val in MyType\n");
			return std::string{};
		}

		if (mono_type_get_type(mono_field_get_type(field)) != MONO_TYPE_STRING) {
			fprintf(stderr, "Field val is not a string\n");
			return std::string{};
		}

		mono_field_get_value(obj, field, &stringField);

		return mono_string_to_utf8(stringField);
	}

	MonoMethod* MonoManager::GetMethodFromClass(MonoClass* klass, std::string funcName)
	{
		MonoMethod* method = mono_class_get_method_from_name(klass, "funcName", -1);
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
		system("sh -c ../Dep/mono/bin/mcs_api.bat");
		system("sh -c ../Dep/mono/bin/mcs_scripts.bat");
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
		/*std::list<MonoClass*> objs = GetAssemblyClassList(_image);

		int index = 0;
		for (auto it = objs.begin(); it != objs.end(); it++)
		{
			std::cout << index << std::endl;
			void* iter = NULL;
			MonoMethod* method = nullptr;
			while (method == mono_class_get_methods(*it, &iter))
			{
				if (!method) continue;
				std::cout << mono_method_get_name(method) << std::endl;
				std::cout << mono_method_full_name(method, 1) << std::endl;
			}
			index++;
			std::cout << std::endl;
		}*/

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
			while (method = mono_class_get_methods(_class, &iter))
			{
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
		while (method = mono_class_get_methods(_class, &iter))
		{
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