#pragma once

namespace Eclipse
{
    class EachFace
    {
    private:
        glm::vec3 Normal = { 0.f, 1.f, 0.f };
        float Distance = 0.f;

    public:
        EachFace();
        EachFace(const glm::vec3& p1, const glm::vec3& norm);
        float getSignedDistanceToPlan(const glm::vec3& point);
    };

    struct FrustrumFaces
    {
        EachFace TopFace;
        EachFace BottomFace;
        EachFace RightFace;
        EachFace LeftFace;
        EachFace FarFace;
        EachFace NearFace;
    };

    class CullingManager
    {
    private:
        glm::vec3 Center{ 0.f, 0.f, 0.f };
        float Radius = 0.f;

    public:
        CullingManager();
        CullingManager(const glm::vec3& inCenter, float inRadius);
        FrustrumFaces FrustrumFaceInfo(CameraComponent::CameraType CameraType);
        bool CheckOnFace(EachFace& plan);
        bool CheckOnFrustum(const TransformComponent& transform , CameraComponent::CameraType CameraType);
        bool ToRenderOrNot(unsigned int ID);
    };
}