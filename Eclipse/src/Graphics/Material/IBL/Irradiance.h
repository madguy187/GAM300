#pragma once

namespace Eclipse
{
    class Irradiance
    {
    public:
        unsigned int hdrTexture = 0;
        unsigned int envCubemap = 0;
        glm::mat4 captureProjection;
        unsigned int cubeVAO = 0;
        unsigned int cubeVBO = 0;
        unsigned int captureFBO = 0;
        unsigned int captureRBO = 0;
        unsigned int irradianceMap = 0;
        unsigned int brdfLUTTexture = 0;
        unsigned int prefilterMap;
        unsigned int quadVAO = 0;
        unsigned int quadVBO = 0;

        void Init();
        void SetupCubeMap();
        void renderQuad();
        void renderCube();
    };
}