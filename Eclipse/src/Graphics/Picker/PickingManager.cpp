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

    //float winWidth = static_cast<float>(OpenGL_Context::GetWindowRatioX() * OpenGL_Context::GetWidth());
    //float winHeight = static_cast<float>(OpenGL_Context::GetWindowRatioY() * OpenGL_Context::GetHeight());
    //
    //float mouseX = OpenGL_Context::GetMouseCursorPos().x / (0.5f * winWidth) - 1.0f;
    //float mouseY = OpenGL_Context::GetMouseCursorPos().y / (0.5f * winHeight) - 1.0f;
    //
    //float mouseX = OpenGL_Context::GetMouseCursorPos().x / (0.5f * OpenGL_Context::GetWidth()) - 1.0f;
    //float mouseY = OpenGL_Context::GetMouseCursorPos().y / (0.5f * OpenGL_Context::GetHeight()) - 1.0f;
    //
    //float mouseX = (2.0f * OpenGL_Context::GetMouseCursorPos().x) / OpenGL_Context::GetWidth() - 1.0f;
    //float mouseY = 1.0f - (2.0f * OpenGL_Context::GetMouseCursorPos().y) / OpenGL_Context::GetHeight();

    float aspectRatio = ImGui::GetWindowWidth() / ImGui::GetWindowHeight();
    float camWidth = aspectRatio * camera.fov;

    float mouseX = OpenGL_Context::GetMouseCursorPos().x;
    float mouseY = OpenGL_Context::GetMouseCursorPos().y;

    float windowOffsetX = ImGui::GetCursorScreenPos().x - OpenGL_Context::GetContextPosition().x;
    //float windowOffsetY = ImGui::GetCursorScreenPos().y - OpenGL_Context::GetContextPosition().y;
    float windowOffsetY = (OpenGL_Context::GetContextPosition().y + OpenGL_Context::GetHeight()) - ImGui::GetCursorScreenPos().y;

    float windowX = (ImGui::GetWindowWidth() / 2) + windowOffsetX;
    float windowY = (ImGui::GetWindowHeight() / 2) + windowOffsetY;

    mouseX -= windowX + camera.eyePos.x;
    mouseY = windowY - mouseY + camera.eyePos.y;

    mouseX = mouseX * (camWidth / ImGui::GetWindowWidth());
    mouseY = mouseY * (camera.fov / ImGui::GetWindowHeight());

    //float mouseX = (2.0f * OpenGL_Context::GetMouseCursorPos().x) /  - 1.0f;
    //float mouseY = 1.0f - (2.0f * OpenGL_Context::GetMouseCursorPos().y) / ImGui::GetWindowHeight();

    //std::cout << "MousePosX: " << OpenGL_Context::GetMouseCursorPos().x << std::endl;
    //std::cout << "MousePosY: " << OpenGL_Context::GetMouseCursorPos().y << std::endl;

    //std::cout << "MousePosX: " << OpenGL_Context::m_posX << std::endl;
    //std::cout << "MousePosY: " << OpenGL_Context::m_posY << std::endl;

    //std::cout << "Width: " << OpenGL_Context::GetWidth() << std::endl;
    //std::cout << "Height: " << OpenGL_Context::GetHeight() << std::endl;

    //std::cout << "Mouse: { " << mouseX << " ," << mouseY << " }" << std::endl;

	glm::mat4 invVP = glm::inverse(camera.projMtx * camera.viewMtx);
	glm::vec4 clipCoords = glm::vec4{ mouseX, -mouseY, -1.0f, 1.0f };
	glm::vec4 worldPos = invVP * clipCoords;

	glm::vec3 dir = glm::normalize(glm::vec3(worldPos));

    int keyN = glfwGetKey(OpenGL_Context::GetWindow(), GLFW_KEY_N);

    if (GLFW_PRESS == keyN)
    {
        auto& io = ImGui::GetIO();
        //std::cout << "ImGui mousePosX: " << io.MousePos.x << std::endl;
        //std::cout << "ImGui mousePosY: " << io.MousePos.y << std::endl;     
        //
        //std::cout << "ImGui WindowPosX: " << ImGui::GetWindowPos().x << std::endl;
        //std::cout << "ImGui WindowPosY: " << ImGui::GetWindowPos().y << std::endl;

        //std::cout << "ImGui ContentRegionAvailX: " << ImGui::GetContentRegionAvail().x << std::endl;
        //std::cout << "ImGui ContentRegionAvailY: " << ImGui::GetContentRegionAvail().y << std::endl;
        //
        //std::cout << "ImGui ItemRectMinX: " << ImGui::GetItemRectMin().x << std::endl;
        //std::cout << "ImGui ItemRectMinY: " << ImGui::GetItemRectMin().y << std::endl;
        //
        //std::cout << "ImGui WindowPosX: " << ImGui::GetWindowPos().x << std::endl;
        //std::cout << "ImGui WindowPosY: " << ImGui::GetWindowPos().y << std::endl;
        //
        //std::cout << "ImGui CursorPosX: " << ImGui::GetCursorPos().x << std::endl;
        //std::cout << "ImGui CursorPosY: " << ImGui::GetCursorPos().y << std::endl;
        //
        //std::cout << "ImGui WindowContentRegionMinX: " << ImGui::GetWindowContentRegionMin().x << std::endl;
        //std::cout << "ImGui WindowContentRegionMinY: " << ImGui::GetWindowContentRegionMin().y << std::endl;
        //
        //std::cout << "ImGui WindowContentRegionMaxX: " << ImGui::GetWindowContentRegionMax().x << std::endl;
        //std::cout << "ImGui WindowContentRegionMaxY: " << ImGui::GetWindowContentRegionMax().y << std::endl;
        
        std::cout << "ContextPosX: " << OpenGL_Context::GetContextPosition().x << std::endl;
        std::cout << "ContextPosY: " << OpenGL_Context::GetContextPosition().y << std::endl;

        //std::cout << "ImGui WindowPosX: " << ImGui::GetWindowPos().x << std::endl;
        //std::cout << "ImGui WindowPosY: " << ImGui::GetWindowPos().y << std::endl;

        std::cout << "WindowPosX func: " << scene->GetSceneBufferPos().x << std::endl;
        std::cout << "WindowPosY func: " << scene->GetSceneBufferPos().y << std::endl;

        std::cout << "MousePosX: " << OpenGL_Context::GetMouseCursorPos().x << std::endl;
        std::cout << "MousePosY: " << OpenGL_Context::GetMouseCursorPos().y << std::endl;

        std::cout << "Camera Height: " << camera.fov << std::endl;
        std::cout << "Camera Pos: {" << camera.eyePos.x << ", " << camera.eyePos.y << ", " << camera.eyePos.z << " }" << std::endl;

        std::cout << "ImGui Width func: " << scene->GetSceneBufferSize().x << std::endl;
        std::cout << "ImGui Height func: " << scene->GetSceneBufferSize().y << std::endl;
        
        //std::cout << "ImGui Width: " << ImGui::GetWindowWidth() << std::endl;
        //std::cout << "ImGui Height: " << ImGui::GetWindowHeight() << std::endl;
        std::cout << "AR: " << aspectRatio << std::endl;
        
        std::cout << "CursorScreenPosX: " << ImGui::GetCursorScreenPos().x << std::endl;
        std::cout << "CursorScreenPosY: " << ImGui::GetCursorScreenPos().y << std::endl;

        std::cout << "WindowOffsetX: " << windowOffsetX << std::endl;
        std::cout << "WindowOffsetY: " << windowOffsetY << std::endl;

        std::cout << "WindowX: " << (ImGui::GetWindowWidth() / 2) + ImGui::GetCursorScreenPos().x << std::endl;
        std::cout << "WindowY: " << (ImGui::GetWindowHeight() / 2) + ImGui::GetCursorScreenPos().y << std::endl;

        std::cout << "Mouse: { " << mouseX << " ," << mouseY << " }" << std::endl;
        std::cout << "RayDir: { " << dir.x << ", " << dir.y << ", " << dir.z << " }" << std::endl;

        std::cout << std::endl;
    }

	return dir;
}

bool Eclipse::PickingManager::RayAabb(glm::vec3& rayStart, glm::vec3& rayDir, glm::vec3& aabbMin, glm::vec3& aabbMax, float& t)
{
    t = 0.0f;
    float tMax = (std::numeric_limits<float>::max)();

    for (size_t i = 0; i < 3; ++i)
    {
        if (rayDir[i] == 0)
        {
            if ((rayStart[i] >= aabbMin[i]) && (rayStart[i] <= aabbMax[i]))
            {
                continue;
            }
            else
            {
               // std::cout << "false" << std::endl;
                return false;
            }
        }

        float invDir = 1.0f / rayDir[i];
        float t1 = (aabbMin[i] - rayStart[i]) * invDir;
        float t2 = (aabbMax[i] - rayStart[i]) * invDir;

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
            //std::cout << "falseEnd" << std::endl;
            return false;
        }
    }

    //std::cout << "true" << std::endl;
    return true;
}
