#pragma once
#include "pch.h"
#include "mono/jit/jit.h"

namespace Eclipse
{
	static void GetKeyTriggered(InputKeycode KeyIn)
	{
		engine->InputManager->GetKeyTriggered(KeyIn);
	}

	static void GetKeyTriggered(std::string Mappedkeycode)
	{
		engine->InputManager->GetKeyTriggered(Mappedkeycode);
	}

	static void GetKeyCurrent(InputKeycode keycode)
	{
		engine->InputManager->GetKeyCurrent(keycode);
	}

	static void GetKeyCurrent(std::string Mappedkeycode)
	{
		engine->InputManager->GetKeyCurrent(Mappedkeycode);
	}

	// I haven do UI FOR MOUSE YET
	static void GetMouseTriggered(InputMouseKeycode KeyIn)
	{
		engine->InputManager->GetMouseTriggered(KeyIn);
	}

	static void GetMouseTriggered(std::string Mappedkeycode)
	{
		engine->InputManager->GetMouseTriggered(Mappedkeycode);
	}

	static void GetMouseCurrent(InputMouseKeycode keycode)
	{
		engine->InputManager->GetMouseCurrent(keycode);
	}

	static void GetMouseCurrent(std::string Mappedkeycode)
	{
		engine->InputManager->GetMouseCurrent(Mappedkeycode);
	}

}