#pragma once
#include "pch.h"
#include "mono/jit/jit.h"
#include "Global.h"

namespace Eclipse
{
	static float GetDeltaTime()
	{
		return engine->Game_Clock.get_DeltaTime();
	}

	static float GetFixedDeltaTime()
	{
		return engine->Game_Clock.get_fixedDeltaTime();
	}
}