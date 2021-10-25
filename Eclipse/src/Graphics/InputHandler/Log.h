#pragma once
#include "Graphics/InputHandler/Key.h"

namespace Eclipse
{
    std::ostream& operator <<(std::ostream& stream, const Eclipse::InputKeycode& keycode);
    std::ostream& operator <<(std::ostream& stream, const Eclipse::InputMouseKeycode& keycode);
}