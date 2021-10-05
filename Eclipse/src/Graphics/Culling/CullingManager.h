#ifndef CULLING_MANAGER_H
#define CULLING_MANAGER_H
#include "Graphics/Grid/IAABB.h"
#include "Graphics/Grid/AABB.h"

namespace Eclipse
{
    struct DYN_AABB;

    class AABBCulling : public IAABB
    {
    public:
        DYN_AABB AABB;
        AABBComponent* Obj;
        DYN_AABB getAABB() const override;
    };

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
        bool GameFrustumCull = false;
        bool SceneFrustumCull = true;
        std::unordered_map<unsigned int, std::shared_ptr< AABBCulling>> CullContainer;

    public:
        AABBTree FrustrumCollisionTree{ 5000 };
        DYN_AABB CameraAABB;
        std::shared_ptr<FrustrumFaces> frustum;

        CullingManager();
        CullingManager(const glm::vec3& inCenter, float inRadius);
        FrustrumFaces FrustrumFaceInfo(CameraComponent::CameraType CameraType);
        bool CheckOnFace(EachFace& plan);
        bool CheckOnFace(EachFace& Face, glm::vec3 Center, float Radius);
        bool CheckOnFrustum(const TransformComponent& transform, CameraComponent::CameraType CameraType);
        bool ToRenderOrNot(unsigned int ID);
        void Insert(AABBComponent& In, unsigned int ID);
        void Remove(unsigned int ID);
        void Clear();
        std::vector<unsigned int> ReturnContacted();
        CameraComponent::CameraType GetCamera();
        void UpdateDYN_AABB(AABBComponent& in, unsigned int ID);
        AABBComponent SetFrustrumAABB(CameraComponent::CameraType CameraType);
    };
}
#endif // CULLING_MANAGER_H