#ifndef SPOTLIGHT_H
#define SPOTLIGHT_H
#include "ECS/ComponentManager/Components/SpotLightComponent.h"

namespace Eclipse
{
    typedef std::unordered_map<int, SpotLightComponent*> SpotLightContainer;
    using SLIT = std::unordered_map<int, SpotLightComponent*>::iterator;

    class SpotLight
    {
    public:
        inline static unsigned int SpotLightCounter = 0;

    public:
        unsigned int GetNumberOfSpotLights();
        static void CreateSpotLight(unsigned int CreatedID);
        void Draw(unsigned int EntityId,SpotLightComponent* in, FrameBufferMode, unsigned int indexID, GLenum mode);
        void Destroy();

    private:
        void CheckUniformLoc(Shader* _shdrpgm, SpotLightComponent& hi, int index, unsigned int containersize, unsigned int EntityId);

        void CheckUniformPBR(int index, unsigned int EntityId);

    private:
        inline static SpotLightContainer _spotlights;
    };
}
#endif /* SPOTLIGHT_H */