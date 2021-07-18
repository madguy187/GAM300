#pragma once

#include "System.h"
#include "Graphics/Material/MaterialManager.h"

namespace Eclipse
{
    class MaterialSystem : public System
    {
    public:
        MaterialManager MaterialManager_;
        static void Init();

    private:
        void Update() override;
    };
}