#include "pch.h"
#include "CameraManager.h"

namespace Eclipse
{
    CameraManager& CameraManager::GetCameraManager()
    {
        static CameraManager _gCameraManager;
        return _gCameraManager;
    }

    void CameraManager::InitEditorCamera()
    {
        Entity newCam = engine->world.CreateEntity();
        engine->world.AddComponent(newCam, CameraComponent{});

        editorCamID = newCam;

        auto& editorCam = engine->world.GetComponent<CameraComponent>(newCam);
        editorCam.camType = CameraComponent::CameraType::Editor_Camera;
    }

    unsigned int CameraManager::GetEditorCameraID()
    {
        return editorCamID;
    }

    void CameraManager::UpdateEditorCamera()
    {
        unsigned int editorID = GetEditorCameraID();
        auto& camera = engine->world.GetComponent<CameraComponent>(editorID);

        float cameraSpd = GLHelper::deltaTime * camera.cameraSpeed;

        if (moveLeft_flag)
        {
            camera.eyePos -= glm::normalize(glm::cross(camera.eyeFront, camera.upVec)) * cameraSpd;
        }

        if (moveRight_flag)
        { 
            camera.eyePos += glm::normalize(glm::cross(camera.eyeFront, camera.upVec)) * cameraSpd;
        }

        if (moveFront_flag)
        {
            camera.eyePos += camera.eyeFront * cameraSpd;
        }

        if (moveBack_flag)
        {
            camera.eyePos -= camera.eyeFront * cameraSpd;
        }

        if (zoomIn_flag)
        {
            if (camera.fov < 2.0f)
            {
                camera.fov = 1.0f;
            }
            else
            {
                camera.fov -= cameraSpd;
            }
        }

        if (zoomOut_flag)
        {
            if (camera.fov > 179.0f)
            {
                camera.fov = 180.0f;
            }
            else
            {
                camera.fov += cameraSpd;
            }     
        }

        camera.viewMtx = glm::lookAt(camera.eyePos, camera.eyePos + camera.eyeFront, camera.upVec);
        camera.projMtx = glm::perspective(glm::radians(camera.fov),
                                            static_cast<float>((GLHelper::windowRatioX * GLHelper::width) / 
                                                (GLHelper::windowRatioY * GLHelper::height)),
                                            camera.nearPlane, camera.farPlane);
    }

    void CameraManager::UpdateCameraInput()
    {
        int keyA = glfwGetKey(GLHelper::ptr_window, GLFW_KEY_A);
        int keyW = glfwGetKey(GLHelper::ptr_window, GLFW_KEY_W);
        int keyS = glfwGetKey(GLHelper::ptr_window, GLFW_KEY_S);
        int keyD = glfwGetKey(GLHelper::ptr_window, GLFW_KEY_D);

        int keyZ = glfwGetKey(GLHelper::ptr_window, GLFW_KEY_Z);
        int keyX = glfwGetKey(GLHelper::ptr_window, GLFW_KEY_X);

        if (GLFW_PRESS == keyA)
        {
            moveLeft_flag = true;
        }
        else if (GLFW_RELEASE == keyA)
        {
            moveLeft_flag = false;
        }

        if (GLFW_PRESS == keyS)
        {
            moveBack_flag = true;
        }
        else if (GLFW_RELEASE == keyS)
        {
            moveBack_flag = false;
        }

        if (GLFW_PRESS == keyD)
        {
            moveRight_flag = true;
        }
        else if (GLFW_RELEASE == keyD)
        {
            moveRight_flag = false;
        }

        if (GLFW_PRESS == keyW)
        {
            moveFront_flag = true;
        }
        else if (GLFW_RELEASE == keyW)
        {
            moveFront_flag = false;
        }

        if (GLFW_PRESS == keyZ)
        {
            zoomIn_flag = true;
        }
        else if (GLFW_RELEASE == keyZ)
        {
            zoomIn_flag = false;
        }

        if (GLFW_PRESS == keyX)
        {
            zoomOut_flag = true;
        }
        else if (GLFW_RELEASE == keyX)
        {
            zoomOut_flag = false;
        }
    }

    void CameraManager::SetCameraSpeed(float newSpeed)
    {
        unsigned int editorID = GetEditorCameraID();
        auto& camera = engine->world.GetComponent<CameraComponent>(editorID);

        camera.cameraSpeed = newSpeed;
    }
}