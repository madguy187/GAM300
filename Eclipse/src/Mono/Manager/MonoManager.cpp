#include "pch.h"
#include "MonoManager.h"
#include "mono/metadata/assembly.h"

namespace Eclipse
{
	void MonoManager::Init()
	{
		mono_set_dirs("../Dep/mono/lib", "../Dep/mono/etc");

		domain = mono_jit_init("Manager");
		assert(domain, "Domain could not be created");

		assembly = mono_domain_assembly_open(domain, "../EclipseScripts.dll");
		assert(assembly, "Assembly could not be opened");

		image = mono_assembly_get_image(assembly);
		assert(image, "Image failed");

		std::cout << "Mono Init Success" << std::endl;
	}

	void MonoManager::Update()
	{

	}

	void MonoManager::GenerateDLL()
	{
		//system("")
	}
}