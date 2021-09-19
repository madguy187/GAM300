#ifndef GRAPHICS_MANAGER_H
#define GRAPHICS_MANAGER_H

#include "ECS/ComponentManager/Components/MeshComponent.h"
#include "AssimpModel/AssimpModel.h"
#include "Graphics/InputHandler/InputWrapper.h"
#include "Graphics/ModelFactory/Sky/CubeMap.h"
#include "Graphics/Grid/Grid.h"
#include "Graphics/Grid/Box.h"
#include "BasicPrimitives/Frustum.h"

namespace Eclipse
{
    struct Plan
    {
        glm::vec3 normal = { 0.f, 1.f, 0.f }; // unit vector
        float     distance = 0.f;        // Distance with origin

        Plan() {};

        Plan(const glm::vec3& p1, const glm::vec3& norm)
            : normal(glm::normalize(norm)),
            distance(glm::dot(normal, p1))
        {}

        float getSignedDistanceToPlan(const glm::vec3& point) const
        {
            return glm::dot(normal, point) - distance;
        }
    };

    struct FrustumCull
    {
        Plan topFace;
        Plan bottomFace;

        Plan rightFace;
        Plan leftFace;

        Plan farFace;
        Plan nearFace;
    };

    struct BoundingVolume
    {
        virtual bool isOnFrustum(const FrustumCull& camFrustum, const TransformComponent& transform) const = 0;

        virtual bool isOnOrForwardPlan(const Plan& plan) const = 0;

        bool isOnFrustum(const FrustumCull& camFrustum) const
        {
            return (isOnOrForwardPlan(camFrustum.leftFace) &&
                isOnOrForwardPlan(camFrustum.rightFace) &&
                isOnOrForwardPlan(camFrustum.topFace) &&
                isOnOrForwardPlan(camFrustum.bottomFace) &&
                isOnOrForwardPlan(camFrustum.nearFace) &&
                isOnOrForwardPlan(camFrustum.farFace));
        };
    };

    struct SphereCull : public BoundingVolume
    {
        glm::vec3 center{ 0.f, 0.f, 0.f };
        float radius{ 0.f };

        SphereCull() {};

        SphereCull(const glm::vec3& inCenter, float inRadius)
            : BoundingVolume{}, center{ inCenter }, radius{ inRadius }
        {}

        bool isOnOrForwardPlan(const Plan& plan) const;

        bool isOnFrustum(const FrustumCull& camFrustum, const TransformComponent& Transform) const;
    };

    class GraphicsManager
    {
    public:
        OpenGL_Context mRenderContext;
        std::vector<AssimpModel*> ModelContainer;
        InputWrapper InputHandler;
        GLenum GlobalMode = GL_FILL;
        std::unique_ptr<CubeMap> Sky;
        AABB_ AllAABBs;

        unsigned int createdID;
        unsigned int SkyCount = 0;
        bool CheckRender = true;
        bool EnableGammaCorrection = true;
        bool DrawSky = true;
        std::unique_ptr<SphereCull> uo;

    public:
        std::map<std::string, std::vector<std::string>> ShaderMap;

        FrustumCull CreateFrustum();
        void Pre_Render();
        void Post_Render();
        void End();
        void Unload();
        void GlobalFrmeBufferDraw();
        void GlobalFrameBufferBind();
        void Draw(unsigned int FrameBufferID, MeshComponent* _spritecomponent, GLenum mode, unsigned int ID, CameraComponent::CameraType _camType);
        void DrawIndexed(MeshComponent* in, GLenum mode);
        void CheckTexture(unsigned int ID);
        void CreatePrimitives(Entity ID, int ModelType);
        void CreateSky(std::string _Dir);
        void RenderSky(unsigned int FrameBufferID);
        void DebugPrintFrameBuffers();
        float GetGammaCorrection();
        void SetGammaCorrection(float in);
        void UploadGlobalUniforms();
        void CheckUniformLoc(Shader* _shdrpgm, MeshComponent& sprite, unsigned int id, unsigned int framebufferID, CameraComponent& camera);
        // Reset Boxes so we can prepare to add
        void ResetInstancedDebugBoxes();
        // Draw Debug Boxes
        void DrawDebugBoxes();
        std::string GetModelName(unsigned int modelname);
        unsigned int GetFrameBufferID(FrameBufferMode mode);
        FrameBuffer::RenderMode GetRenderMode(FrameBufferMode mode);
    private:
        float GammaCorrection = 1.0f;
        void UpdateFrameBuffer();
        void FrameBufferDraw();
    };
}

#endif