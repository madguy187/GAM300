#include "pch.h"
#include "Graphics/InputHandler/test.h"

namespace Eclipse
{
    void test::hi()
    {
        if (engine->helloInput->GetKeyPressed(InputKeycode::KeyCode_A))
        {
            std::cout << "Hi" << std::endl;
        }
    }
}