#include "pch.h"
#include "MonoManager.h"
#include "mono/metadata/assembly.h"
#include <mono/metadata/mono-gc.h>
#include <mono/metadata/debug-helpers.h>

#include "ECS/ComponentManager/Components/TransformComponent.h"

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

	static MonoObject* GetTranslate(Entity ent)
	{
		TransformComponent& trans2 = engine->world.GetComponent<TransformComponent>(ent);
		auto& trans = engine->world.GetComponent<TransformComponent>(ent);
		void* args[3];
		args[0] = &trans.position.x;
		args[1] = &trans.position.y;
		args[2] = &trans.position.z;

		MonoClass* klass = mono_class_from_name(engine->mono.GetAPIImage(), "Eclipse", "MonoVec3");

		if (klass == nullptr) {
			std::cout << "Failed loading class, MonoVec3" << std::endl;
			return nullptr;
		}

		MonoObject* obj = mono_object_new(mono_domain_get(), klass);
		if (obj == nullptr) {
			std::cout << "Failed loading class instance, MonoVec3" << std::endl;
			return nullptr;
		}

		engine->mono.DumpInfoFromClass(klass);

		MonoMethod* ctor_method = mono_class_get_method_from_name(klass, ".ctor", -1);
		if (!ctor_method)
		{
			std::cout << "Failed to get method" << std::endl;
			return nullptr;
		}

		//mono_runtime_object_init(obj);
		mono_runtime_invoke(ctor_method, obj, args, NULL);

		return obj;
	}

	void MonoManager::Init()
	{
		ENGINE_CORE_INFO("Mono: Initialising");

		mono_set_dirs("../Dep/mono/lib", "../Dep/mono/etc");

		domain = mono_jit_init("Manager");
		assert(domain, "Domain could not be created");

		GenerateDLL();

		// Load API
		APIAssembly = mono_domain_assembly_open(domain, "../EclipseScriptsAPI.dll");
		assert(APIAssembly, "API Assembly could not be opened");

		APIImage = mono_assembly_get_image(APIAssembly);
		assert(APIImage, "API Image failed");

		// Load Scripts
		ScriptAssembly = mono_domain_assembly_open(domain, "../EclipseScripts.dll");
		assert(ScriptAssembly, "Script Assembly could not be opened");

		ScriptImage = mono_assembly_get_image(ScriptAssembly);
		assert(ScriptImage, "Script Image failed");

		ENGINE_CORE_INFO("Mono: Successfully Initialise");

		// adding internal call
		mono_add_internal_call("Eclipse.TransformComponent::GetTranslate", GetTranslate);

		/*Entity ent = engine->world.CreateEntity();
		TransformComponent trans;
		trans.position.x = 1.0f;
		trans.position.y = 2.0f;
		trans.position.z = 3.0f;*/
		
		/*engine->world.AddComponent<TransformComponent>(ent, trans);*/

		/*TransformComponent& trans2 = engine->world.GetComponent<TransformComponent>(ent);
		std::cout << trans2.position.x << std::endl;*/

		CreateScriptObjects();
		std::cout << "Successfully pushed" << std::endl;
	}

	void MonoManager::Update(MonoScript* obj)
	{
		/*DumpInfoFromImage(engine->mono.GetAPIImage());
		DumpInfoFromImage(engine->mono.GetScriptImage());*/
		MonoClass* klass = mono_class_from_name(engine->mono.GetScriptImage(), "", obj->scriptName.c_str());

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

		void* args[1];
		mono_runtime_invoke(m_update, obj, args, NULL);
	}

	void MonoManager::RestartMono()
	{
		mono_image_close(ScriptImage);
		mono_image_close(APIImage);
		objects.clear();

		MonoDomain* old_domain = mono_domain_get();
		if (old_domain && old_domain != mono_get_root_domain()) {
			if (!mono_domain_set(mono_get_root_domain(), false))
				ENGINE_CORE_WARN("Error: Domain could not be set");

			mono_domain_unload(old_domain);
		}

		//unloading a domain is also a nice point in time to have the GC run.
		mono_gc_collect(mono_gc_max_generation());
	}

	void MonoManager::StopMono()
	{
		mono_jit_cleanup(domain);
		/*mono_image_close(ScriptImage);
		mono_image_close(APIImage);*/
		
		objects.clear();
	}

	MonoObject* MonoManager::CreateMonoObject(std::string scriptName, Entity entity)
	{
		MonoClass* base = mono_class_from_name(APIImage, "Eclipse", scriptName.c_str());
		if (!base)
		{
			std::cout << "Failed loading class" << std::endl;
			return nullptr;
		}

		MonoMethod* method = mono_class_get_method_from_name(base, "InitBehavior", -1);
		if (method == nullptr) {
			std::cout << "Failed loading class method" << std::endl;
			return nullptr;
		}

		MonoObject* obj = mono_object_new(mono_domain_get(), base);
		if (obj == nullptr) {
			std::cout << "Failed loading class instance " << mono_class_get_name(base) << std::endl;
			return nullptr;
		}

		void* args[2];
		uint32_t handle = mono_gchandle_new(obj, true);
		args[0] = &handle;
		Entity ent = entity;
		args[1] = &ent;
		mono_runtime_invoke(method, obj, args, NULL);

		return obj;
	}

	void MonoManager::GenerateDLL()
	{
		ENGINE_CORE_INFO("Mono: Generating DLLs");
		system("sh -c ../Dep/mono/bin/mcs_api.bat");
		system("sh -c ../Dep/mono/bin/mcs_scripts.bat");
		ENGINE_CORE_INFO("Mono: Successfully Generate DLLs");
	}

	void MonoManager::CreateScriptObjects()
	{
		std::list<MonoClass*> objs = GetAssemblyClassList(ScriptImage);
		objs.pop_front();

		MonoClass* base = mono_class_from_name(APIImage, "Eclipse", "EclipseBehavior");
		MonoMethod* method = mono_class_get_method_from_name(base, "InitBehavior", -1);
		if (method == nullptr) {
			std::cout << "Failed loading class method" << std::endl;
			return;
		}

		for (auto it = objs.begin(); it != objs.end(); it++)
		{
			DumpInfoFromClass(*it);
			MonoObject* obj = mono_object_new(mono_domain_get(), *it);

			if (obj == nullptr) {
				std::cout << "Failed loading class instance " << mono_class_get_name(*it) << std::endl;
				return;
			}

			objects.push_back(obj);

			void* args[2];
			uint32_t handle = mono_gchandle_new(obj, true);
			args[0] = &handle;
			Entity ent = 0;
			args[1] = &ent;
			
			mono_runtime_invoke(method, obj, args, NULL);
		}
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
		std::list<MonoClass*> objs = GetAssemblyClassList(_image);

		int index = 0;
		for (auto it = objs.begin(); it != objs.end(); it++)
		{
			std::cout << index << std::endl;
			void* iter = NULL;
			MonoMethod* method;
			while (method = mono_class_get_methods(*it, &iter))
			{
				std::cout << mono_method_get_name(method) << std::endl;
				std::cout << mono_method_full_name(method, 1) << std::endl;
			}
			index++;
			std::cout << std::endl;
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
			std::cout << mono_method_get_name(method) << std::endl;
			std::cout << mono_method_full_name(method, 1) << std::endl;
		}

		std::cout << std::endl;
	}
}