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

    }

    CullingManager::CullingManager(const glm::vec3& inCenter, float inRadius) :
        Center{ inCenter },
        Radius{ inRadius }
    {

    }

    FrustrumFaces CullingManager::FrustrumFaceInfo(CameraComponent::CameraType CameraType)
    {
        auto& cam = engine->world.GetComponent<CameraComponent>(engine->gCamera.GetCameraID(CameraComponent::CameraType::Game_Camera));
        auto& trans = engine->world.GetComponent<TransformComponent>(engine->gCamera.GetCameraID(CameraComponent::CameraType::Game_Camera));

        FrustrumFaces frustum;
        const float halfVSide = 500.0f * tanf(glm::radians(cam.fov) * .5f);
        const float halfHSide = halfVSide * cam.aspect;;
        const glm::vec3 frontMultFar = 500.0f * cam.eyeFront;

        frustum.NearFace = { trans.position.ConvertToGlmVec3Type() + cam.nearPlane * cam.eyeFront, cam.eyeFront };
        frustum.FarFace = { trans.position.ConvertToGlmVec3Type() + frontMultFar, -cam.eyeFront };
        frustum.RightFace = { trans.position.ConvertToGlmVec3Type(), glm::cross(cam.upVec, frontMultFar + cam.rightVec * halfHSide) };
        frustum.LeftFace = { trans.position.ConvertToGlmVec3Type(), glm::cross(frontMultFar - cam.rightVec * halfHSide, cam.upVec) };
        frustum.TopFace = { trans.position.ConvertToGlmVec3Type(), glm::cross(cam.rightVec, frontMultFar - cam.upVec * halfVSide) };
        frustum.BottomFace = { trans.position.ConvertToGlmVec3Type(), glm::cross(frontMultFar + cam.upVec * halfVSide, cam.rightVec) };

        return frustum;
    }

    bool CullingManager::CheckOnFace(EachFace& Face)
    {
        return Face.getSignedDistanceToPlan(Center) > -Radius;
    }

    bool CullingManager::CheckOnFrustum(FrustrumFaces& camFrustum, const TransformComponent& Transform)
    {
        auto& _camera = engine->world.GetComponent<CameraComponent>(engine->gCamera.GetCameraID(CameraComponent::CameraType::Game_Camera));

        glm::vec3 globalScale = Transform.scale.ConvertToGlmVec3Type();

        float maxScale = max(max(globalScale.x, globalScale.y), globalScale.z);

        CullingManager globalSphere(Transform.position.ConvertToGlmVec3Type(), (maxScale * 0.5f));

        return (globalSphere.CheckOnFace(camFrustum.LeftFace) &&
            globalSphere.CheckOnFace(camFrustum.RightFace) &&
            globalSphere.CheckOnFace(camFrustum.FarFace) &&
            globalSphere.CheckOnFace(camFrustum.NearFace) &&
            globalSphere.CheckOnFace(camFrustum.TopFace) &&
            globalSphere.CheckOnFace(camFrustum.BottomFace));
    }

    bool CullingManager::ToRenderOrNot(unsigned int ID)
    {
        auto& Transform = engine->world.GetComponent<TransformComponent>(ID);

        return (CheckOnFrustum(FrustrumFaceInfo(CameraComponent::CameraType::Editor_Camera), Transform));
    }
}