#pragma once
#include "pch.h"
#include "mono/jit/jit.h"
#include "Global.h"

namespace Eclipse
{
	static MonoObject* GetRigidComponent(Entity ent)
	{
		MonoClass* klass = engine->mono.GetMonoClass("RigidBodyComponent");
		MonoObject* obj = engine->mono.CreateObjectFromClass(klass);
		MonoMethod* method = engine->mono.GetMethodFromClass(klass, "Initcomponent");
		std::vector<void*> args;
		args.push_back(&ent);

		std::cout << "C++: " << ent << std::endl;
		engine->mono.ExecuteMethod(obj, method, args);

		return obj;
	}
}