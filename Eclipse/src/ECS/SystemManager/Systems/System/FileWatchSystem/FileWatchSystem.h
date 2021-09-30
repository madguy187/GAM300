#pragma once
#include "../System.h"

namespace Eclipse
{
    class FileWatchSystem : public System
    {
    public:
        static void Init();
        void Update() override;
    };
}