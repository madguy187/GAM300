#pragma once
#include "ECS/ComponentManager/Components/PointLightComponent.h"
#include "ECS/ComponentManager/Components/TransformComponent.h"

namespace Eclipse
{
    typedef std::map<int, PointLightComponent*> PointLightContainer;
    using PointL = std::map<int, PointLightComponent*>::iterator;

    class PointLight
    {
    private:
        // Global Counter for all pointlights to loop in shader
        inline static int counter = 0; 
        // Containr for all Pointlights
        inline static PointLightContainer _pointlights;

    public:
        // Returns the PointLight Container
        PointLightContainer GetContainer();
        // Returns the number of Pointlights
        unsigned int GetNumberOfPointLights();
        // Create PointLight
        static void CreatePointLight(unsigned int CreatedID);
        // Remove PointLight From Container
        static bool DeletePointLight(unsigned int EntityID);
        // Pointlight Old Draw
        void DrawPointLights(unsigned int framebufferID);
        // PointLight
        void Draw(PointLightComponent* in, unsigned int framebufferID, unsigned int indexID, GLenum mode);

    private:
        // Update Shader Variables
        void CheckUniformLoc(Shader* _shdrpgm, PointLightComponent& hi, int index, unsigned int containersize);
    };
}
