#include "pch.h"
#include "PickingManager.h"


void Eclipse::PickingManager::GenerateAabb(unsigned int ID, TransformComponent& _transform)
{
	engine->world.AddComponent(ID, AabbComponent{});
	auto& _aabb = engine->world.GetComponent<AabbComponent>(ID);

	glm::vec3 scale = _transform.scale.ConvertToGlmVec3Type();
	glm::vec3 position = _transform.position.ConvertToGlmVec3Type();

	glm::vec3 halfExt = scale / 2.0f;
	_aabb.center = ECVec3{ position.x, position.y, position.z };
	_aabb.min = ECVec3{ position.x - halfExt.x, position.y - halfExt.y, position.z - halfExt.z };
	_aabb.max = ECVec3{ position.x + halfExt.x, position.y + halfExt.y, position.z + halfExt.z };
}

void Eclipse::PickingManager::UpdateAabb(unsigned int ID)
{
	auto& _transform = engine->world.GetComponent<TransformComponent>(ID);
	auto& _aabb = engine->world.GetComponent<AabbComponent>(ID);

	glm::vec3 scale = _transform.scale.ConvertToGlmVec3Type();
	glm::vec3 position = _transform.position.ConvertToGlmVec3Type();

	glm::vec3 halfExt = scale / 2.0f;
	_aabb.center = ECVec3{ position.x, position.y, position.z };
	_aabb.min = ECVec3{ position.x - halfExt.x, position.y - halfExt.y, position.z - halfExt.z };
	_aabb.max = ECVec3{ position.x + halfExt.x, position.y + halfExt.y, position.z + halfExt.z };

}

glm::vec3 Eclipse::PickingManager::ComputeCursorRayDirection()
{
    auto& camera = engine->world.GetComponent<CameraComponent>(engine->gCamera.GetEditorCameraID());
    auto* scene = engine->editorManager->GetEditorWindow<SceneWindow>();

    float mouseX = OpenGL_Context::GetMouseCursorPos().x;
    float mouseY = OpenGL_Context::GetMouseCursorPos().y;

    float windowOffsetX = scene->GetSceneBufferPos().x - OpenGL_Context::GetContextPosition().x;
    float windowOffsetY = scene->GetSceneBufferPos().y - OpenGL_Context::GetContextPosition().y;

    float mouseXOffset = mouseX - windowOffsetX;
    float mouseYOffset = mouseY - windowOffsetY;


    glm::vec4 rayStartNDC{ (2.0f * mouseXOffset) / scene->GetSceneBufferSize().x - 1.0f,
                            1.0f - (2.0f * mouseYOffset) / scene->GetSceneBufferSize().y,
                            -1.0f,
                            1.0f };
 
    glm::vec4 rayEye = glm::inverse(camera.projMtx) * rayStartNDC;
    rayEye = glm::vec4(rayEye.x, rayEye.y, -1.0, 0.0);

    glm::vec4 ray = glm::inverse(camera.viewMtx) * rayEye;
    glm::vec3 rayWorld = glm::vec3{ ray.x, ray.y, ray.z };
    rayWorld = glm::normalize(rayWorld);

	return rayWorld;

}

bool Eclipse::PickingManager::RayAabb(glm::vec3& rayStart, glm::vec3& rayDir, glm::vec3& aabbMin, glm::vec3& aabbMax, float& t)
{
    t = 0.0f;
    float tMax = (std::numeric_limits<float>::max)();

    for (size_t i = 0; i < 3; ++i)
    {
        if (rayDir[static_cast<glm::length_t>(i)] == 0)
        {
            if ((rayStart[static_cast<glm::length_t>(i)] >= aabbMin[static_cast<glm::length_t>(i)]) && 
                (rayStart[static_cast<glm::length_t>(i)] <= aabbMax[static_cast<glm::length_t>(i)]))
            {
                continue;
            }
            else
            {
               // std::cout << "false" << std::endl;
                return false;
            }
        }

        float invDir = 1.0f / rayDir[static_cast<glm::length_t>(i)];
        float t1 = (aabbMin[static_cast<glm::length_t>(i)] - rayStart[static_cast<glm::length_t>(i)]) * invDir;
        float t2 = (aabbMax[static_cast<glm::length_t>(i)] - rayStart[static_cast<glm::length_t>(i)]) * invDir;

        if (t1 > t2)
        {
            std::swap(t1, t2);
        }

        if (t1 > t)
        {
            t = t1;
        }

        if (tMax > t2)
        {
            tMax = t2;
        }

        if (t > tMax)
        {
            return false;
        }
    }

    return true;
}