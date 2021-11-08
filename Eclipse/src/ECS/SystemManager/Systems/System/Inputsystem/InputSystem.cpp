#include "pch.h"
#include "InputSystem.h"

namespace Eclipse
{
    void InputSystem::Update()
    {
        if (engine->InputManager->GetKeyTriggered("Test"))
        {
            engine->InputManager->LockCursor(CursorLockMode::Locked);
        }

        if (engine->InputManager->GetKeyTriggered(InputKeycode::Key_RIGHT))
        {
            engine->InputManager->LockCursor(CursorLockMode::None);
        }

        engine->InputManager->CursorUpdate();
        engine->InputManager->AxisUpdate();
        engine->InputManager->UpdateKeys();
    }
}