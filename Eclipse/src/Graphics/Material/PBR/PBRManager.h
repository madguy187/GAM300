#pragma once

namespace Eclipse
{
    class PBRManager
    {
    public:
        unsigned int sphereVAO = 0;
        unsigned int indexCount;

        void SetShader();
        void RenderSphere();
        void UpdateLoop();
    };
}