#include "pch.h"
#include "InputSystem.h"

namespace Eclipse
{
    void InputSystem::Update()
    {
        engine->InputManager->UpdateKeys();
    }
}