#pragma once
#include "Global.h"
#include "pch.h"

namespace Eclipse
{
	static void setState(int state)
	{
		engine->InputManager->LockCursor(static_cast<CursorLockMode>(state));
	}
}