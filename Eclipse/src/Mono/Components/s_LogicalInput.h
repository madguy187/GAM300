#pragma once
#include "pch.h"
#include "mono/jit/jit.h"
// #include "Graphics/InputHandler/AllInputKeyCodes.h" Nico , if u ever need to reference my key codes

namespace Eclipse
{
	static bool GetKeyTriggered(uint32_t keycode)
	{
		return engine->InputManager->GetKeyTriggered(static_cast<InputKeycode>(keycode));
	}

	static bool GetKeyCurrentByKeyCode(uint32_t keycode)
	{
		return engine->InputManager->GetKeyCurrent(static_cast<InputKeycode>(keycode));
	}

	static bool GetKeyCurrentByName(MonoString* Mappedkeycode)
	{
		return engine->InputManager->GetKeyCurrent(mono_string_to_utf8(Mappedkeycode));
	}

	// I haven do UI FOR MOUSE YET
	static void GetMouseTriggered(uint32_t keycode)
	{
		engine->InputManager->GetMouseTriggered(static_cast<InputMouseKeycode>(keycode));
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

	static float GetMouseAxis(MonoString* Axis)
	{
		std::string axis = mono_string_to_utf8(Axis);
		return engine->InputManager->GetAxis(axis);
	}

	static float GetRawMouseAxis(MonoString* Axis)
	{
		std::string axis = mono_string_to_utf8(Axis);
		return engine->InputManager->GetRawAxis(axis);
	}
}