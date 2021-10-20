#pragma once

#include "Graphics/Material/PBR/PBRManager.h"
#include "../System.h"

namespace Eclipse
{
    class MaterialSystem : public System
    {
    public:
        static void Init();
    private:
        void Update() override;
    };
}