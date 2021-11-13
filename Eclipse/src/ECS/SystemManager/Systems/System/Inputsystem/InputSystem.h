#pragma once

#include "../System.h"

namespace Eclipse
{
    class InputSystem : public System
    {
        bool firstClick = false;
    private:
        void Update() override;
    };
}