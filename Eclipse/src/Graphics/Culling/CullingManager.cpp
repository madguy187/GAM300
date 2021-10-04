#include "pch.h"
#include "CullingManager.h"

namespace Eclipse
{
    EachFace::EachFace()
    {

    }

    EachFace::EachFace(const glm::vec3& p1, const glm::vec3& norm) :
        Normal(glm::normalize(norm)),
        Distance(glm::dot(Normal, p1))
    {

    }

    float EachFace::getSignedDistanceToPlan(const glm::vec3& point)
    {
        return (glm::dot(Normal, point) - Distance);
    }

    CullingManager::CullingManager()
    {
        frustum = std::make_shared<FrustrumFaces>();
    }

    CullingManager::CullingManager(const glm::vec3& inCenter, float inRadius) :
        Center{ inCenter },
        Radius{ inRadius }
    {

    }

    FrustrumFaces CullingManager::FrustrumFaceInfo(CameraComponent::CameraType CameraType)
    {
        auto& cam = engine->world.GetComponent<CameraComponent>(engine->gCamera.GetCameraID(CameraType));
        auto& trans = engine->world.GetComponent<TransformComponent>(engine->gCamera.GetCameraID(CameraType));

        const float halfVSide = 500.0f * tanf(glm::radians(cam.fov) * .5f);
        const float halfHSide = halfVSide * cam.aspect;;
        const glm::vec3 frontMultFar = 500.0f * cam.eyeFront;

        frustum->NearFace = { trans.position.ConvertToGlmVec3Type() + cam.nearPlane * cam.eyeFront, cam.eyeFront };
        frustum->FarFace = { trans.position.ConvertToGlmVec3Type() + frontMultFar, -cam.eyeFront };
        frustum->RightFace = { trans.position.ConvertToGlmVec3Type(), glm::cross(cam.upVec, frontMultFar + cam.rightVec * halfHSide) };
        frustum->LeftFace = { trans.position.ConvertToGlmVec3Type(), glm::cross(frontMultFar - cam.rightVec * halfHSide, cam.upVec) };
        frustum->TopFace = { trans.position.ConvertToGlmVec3Type(), glm::cross(cam.rightVec, frontMultFar - cam.upVec * halfVSide) };
        frustum->BottomFace = { trans.position.ConvertToGlmVec3Type(), glm::cross(frontMultFar + cam.upVec * halfVSide, cam.rightVec) };

        return *(frustum);
    }

    bool CullingManager::CheckOnFace(EachFace& Face)
    {
        return Face.getSignedDistanceToPlan(Center) > -Radius;
    }

    bool CullingManager::CheckOnFace(EachFace& Face, glm::vec3 Center, float Radius)
    {
        return Face.getSignedDistanceToPlan(Center) > -Radius;
    }

    bool CullingManager::CheckOnFrustum(const TransformComponent& Transform, CameraComponent::CameraType CameraType)
    {
        int CollisionFlag = 0;

        auto& camFrustum = engine->gCullingManager->FrustrumFaceInfo(CameraType);
        auto& _camera = engine->world.GetComponent<CameraComponent>(engine->gCamera.GetCameraID(CameraType));
        glm::vec3 globalScale = Transform.scale.ConvertToGlmVec3Type();
        float maxScale = max(max(globalScale.x, globalScale.y), globalScale.z);

        glm::vec3 Center = Transform.position.ConvertToGlmVec3Type();
        float Radius = maxScale * 0.5f;

        return (CheckOnFace(camFrustum.LeftFace, Center, Radius) &&
            CheckOnFace(camFrustum.RightFace, Center, Radius) &&
            CheckOnFace(camFrustum.FarFace, Center, Radius) &&
            CheckOnFace(camFrustum.NearFace, Center, Radius) &&
            CheckOnFace(camFrustum.TopFace, Center, Radius) &&
            CheckOnFace(camFrustum.BottomFace, Center, Radius));
    }

    bool CullingManager::ToRenderOrNot(unsigned int ID)
    {
        auto& Transform = engine->world.GetComponent<TransformComponent>(ID);
        return CheckOnFrustum(Transform, CameraComponent::CameraType::Editor_Camera);
    }

    void CullingManager::Insert(AABBComponent& In, unsigned int ID)
    {
        std::shared_ptr< AABBComponent> SameAABB = std::make_shared<AABBComponent>(In);
        std::shared_ptr< AABBCulling> hi = std::make_shared<AABBCulling>();
        hi->Obj = &In;
        CullContainer.emplace(ID, hi);
        CullContainer[ID]->AABB.SetMaxMin(In.Max, In.Min, ID);
        FrustrumCollisionTree.InsertObject(engine->gCullingManager->CullContainer[ID]);
    }

    void CullingManager::Remove(unsigned int ID)
    {
        FrustrumCollisionTree.RemoveObject(engine->gCullingManager->CullContainer[ID]);
        engine->gCullingManager->CullContainer.erase(ID);
    }

    void CullingManager::Clear()
    {
        CullContainer.clear();
        FrustrumCollisionTree.ClearTree();
    }

    std::vector<unsigned int> CullingManager::ReturnContacted()
    {
        return FrustrumCollisionTree.QueryAgainstTrustrum(SetFrustrumAABB(CameraComponent::CameraType::Editor_Camera));
    }

    CameraComponent::CameraType CullingManager::GetCamera()
    {
        if (GameFrustumCull)
        {
            return CameraComponent::CameraType::Game_Camera;
        }
        else if (SceneFrustumCull)
        {
            return CameraComponent::CameraType::Editor_Camera;
        }
        else
        {
            return CameraComponent::CameraType::Editor_Camera;
        }
    }

    void CullingManager::UpdateDYN_AABB(AABBComponent& _aabb, unsigned int ID)
    {
        CullContainer[ID]->AABB.SetMaxMin(_aabb.Max, _aabb.Min, ID);
    }

    DYN_AABB AABBCulling::getAABB() const
    {
        return AABB;
    }

    AABBComponent CullingManager::SetFrustrumAABB(CameraComponent::CameraType CameraType)
    {
        auto& _camera = engine->world.GetComponent<CameraComponent>(engine->gCamera.GetCameraID(CameraType));
        auto& Transform = engine->world.GetComponent<TransformComponent>(engine->gCamera.GetCameraID(CameraType));

        glm::vec3 Half{ 2,2,2 };
        glm::vec3 scale{ _camera.farPlane / Half.x , _camera.farPlane / Half.y,_camera.farPlane };

        glm::vec3 position = glm::vec3{ Transform.position.getX() , Transform.position.getY(),Transform.position.getZ() };
        position /= Half;
        glm::vec3 halfExt = scale / 2.0f;
        glm::vec3 min = glm::vec3{ position.x - halfExt.x, position.y - halfExt.y, position.z - halfExt.z };
        glm::vec3 max = glm::vec3{ position.x + halfExt.x, position.y + halfExt.y, position.z + halfExt.z };

        AABBComponent Frustrum;
        Frustrum.center = ECVec3{ position.x , position.y , position.z };
        Frustrum.Min = ECVec3{ min.x,min.y,min.z };
        Frustrum.Max = ECVec3{ max.x,max.y,max.z };

        return Frustrum;
    }
}