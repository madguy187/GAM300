#include "pch.h"
#include "MonoManager.h"
#include "mono/metadata/assembly.h"

namespace Eclipse
{
	void MonoManager::Init()
	{
		ENGINE_CORE_INFO("Mono: Initialising");

		mono_set_dirs("../Dep/mono/lib", "../Dep/mono/etc");

		domain = mono_jit_init("Manager");
		assert(domain, "Domain could not be created");

		GenerateDLL();

		assembly = mono_domain_assembly_open(domain, "../EclipseScripts.dll");
		assert(assembly, "Assembly could not be opened");

		image = mono_assembly_get_image(assembly);
		assert(image, "Image failed");

		ENGINE_CORE_INFO("Mono: Successfully Initialise");
	}

	void MonoManager::Update()
	{

	}

	void MonoManager::GenerateDLL()
	{
		ENGINE_CORE_INFO("Mono: Generating DLLs");
		system("sh -c ../Dep/mono/bin/mcs_api.bat");
		system("sh -c ../Dep/mono/bin/mcs_scripts.bat");
		ENGINE_CORE_INFO("Mono: Successfully Generate DLLs");
	}
}