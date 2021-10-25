#include "pch.h"
#include "MonoManager.h"

#include <mono/metadata/mono-config.h>
#include <mono/metadata/threads.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/mono-gc.h>
#include <mono/metadata/environment.h>
#include <mono/metadata/debug-helpers.h>

#include "ECS/ComponentManager/Components/TransformComponent.h"

#include "../Components/s_RigidBodyComponent.h"


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

	//static MonoObject* GetTranslate(Entity ent)
	//{
	//	//TransformComponent& trans2 = engine->world.GetComponent<TransformComponent>(ent);
	//	auto& trans = engine->world.GetComponent<TransformComponent>(ent);
	//	void* args[3];
	//	args[0] = &trans.position.x;
	//	args[1] = &trans.position.y;
	//	args[2] = &trans.position.z;

	//	MonoClass* klass = mono_class_from_name(engine->mono.GetAPIImage(), "Eclipse", "MonoVec3");

	//	if (klass == nullptr) {
	//		std::cout << "Failed loading class, MonoVec3" << std::endl;
	//		return nullptr;
	//	}

	//	MonoObject* obj = mono_object_new(mono_domain_get(), klass);
	//	if (obj == nullptr) {
	//		std::cout << "Failed loading class instance, MonoVec3" << std::endl;
	//		return nullptr;
	//	}

	//	engine->mono.DumpInfoFromClass(klass);

	//	MonoMethod* ctor_method = mono_class_get_method_from_name(klass, ".ctor", -1);
	//	if (!ctor_method)
	//	{
	//		std::cout << "Failed to get method" << std::endl;
	//		return nullptr;
	//	}

	//	//mono_runtime_object_init(obj);
	//	mono_runtime_invoke(ctor_method, obj, args, NULL);

	//	return obj;
	//}

	static void SetForce(Entity ent, float x, float y, float z)
	{
		engine->gPhysics.SetForce(ent, { x, y, z });
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

		//mono_runtime_object_init(obj);

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

		mono_add_internal_call("Eclipse.PhysicsObject::Add_Force", SetForce);
		mono_add_internal_call("Eclipse.GameObject::GetRigidComponent", GetRigidComponent);
		mono_add_internal_call("Eclipse.EclipseBehavior::GetGameObject", CreateGameObject);
	}

	void MonoManager::Update(MonoScript* obj)
	{
		//DumpInfoFromImage(APIImage);
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

		mono_runtime_invoke(m_update, obj, nullptr, NULL);
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
		MonoClass* klass = mono_class_from_name(APIImage, "", className.c_str());
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

	MonoMethod* MonoManager::GetMethodFromClass(MonoClass* klass, std::string funcName)
	{
		MonoMethod* method = mono_class_get_method_from_name(klass, "funcName", -1);
		if (method == nullptr) {
			std::cout << "Failed loading class method" << std::endl;
			return nullptr;
		}

		return method;
	}

	bool MonoManager::ExecuteMethod(MonoObject* obj, MonoMethod* method, std::vector<void*> args)
	{
		if (!obj && !method) return false;
		mono_runtime_invoke(method, obj, args.data(), NULL);

		return true;
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