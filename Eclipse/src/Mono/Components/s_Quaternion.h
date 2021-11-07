#pragma once
#include "pch.h"
#include "Global.h"

namespace Eclipse
{
	MonoObject* Euler(float x, float y, float z)
	{
		return engine->mono.CreateQuaternionClass(x, y, z);
	}
}