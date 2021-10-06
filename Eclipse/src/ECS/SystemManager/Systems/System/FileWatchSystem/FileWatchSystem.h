#ifndef FILEWATCHINGSYSTEM_H
#define FILEWATCHINGSYSTEM_H
#include "../System.h"
#include "Graphics/FileWatch/FileWatch.h"

namespace Eclipse
{
    class FileWatchSystem : public System
    {
    public:
        static void Init();
        void Update() override;
    };
}
#endif /* FILEWATCHINGSYSTEM_H */