#include "pch.h"
#include "InputSystem.h"

namespace Eclipse
{
    void InputSystem::Update()
    {
        //std::cout << "Mouse X " << OpenGL_Context::m_posX << std::endl;
        //std::cout << "Mouse Y " << OpenGL_Context::m_posY << std::endl;
        //std::cout << " Width " << OpenGL_Context::width << std::endl;
        //std::cout << "GetAxis " << engine->InputManager->GetAxis("Vertical") << std::endl;

        if (engine->InputManager->GetKeyTriggered(InputKeycode::Key_RIGHT))
        {
            engine->InputManager->LockCursor(CursorLockMode::Locked);
        }

        if (engine->InputManager->GetKeyTriggered(InputKeycode::Key_LEFT))
        {
            engine->InputManager->LockCursor(CursorLockMode::None);
        }

        engine->InputManager->CursorUpdate();
        engine->InputManager->AxisUpdate();
        engine->InputManager->UpdateKeys();
    }
}