#pragma once

namespace Eclipse
{
    class Irradiance
    {
    public:
        unsigned int hdrTexture;
        unsigned int envCubemap;
        glm::mat4 captureProjection;
        glm::mat4 captureViews;
        unsigned int cubeVAO = 0;
        unsigned int cubeVBO = 0;
        unsigned int captureFBO;
        unsigned int captureRBO;
        unsigned int irradianceMap;
        unsigned int brdfLUTTexture;
        unsigned int prefilterMap;
        unsigned int quadVAO = 0;
        unsigned int quadVBO = 0;

        void Init();
        void SetupCubeMap();
        void renderQuad();
        void renderCube();
    };
}