#ifndef GRIDSYSTEM_H
#define GRIDSYSTEM_H
#include "../System.h"

namespace Eclipse
{
    class GridSystem : public System
    {
    public:
        static void Init();
        void Update() override;
    };
}
#endif /* GRIDSYSTEM_H */