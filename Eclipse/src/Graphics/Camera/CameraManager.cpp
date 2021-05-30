#include "pch.h"
#include "CameraManager.h"

namespace Eclipse
{
    void CameraManager::CreateEditorCamera()
    {
        if (editorCamID != MAX_ENTITY)
        {
            return;
        }

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

    void CameraManager::ComputeViewMtx(TransformComponent& _transform)
    {
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

        if (yawLeft_flag)
        {
            if (camera.eyeBeta < 1.0f)
            {
                camera.eyeBeta = 360.0f;
            }
            else
            {
                camera.eyeBeta -= cameraSpd;
            }
        }

        if (yawRight_flag)
        {
            if (camera.eyeBeta > 359.0f)
            {
                camera.eyeBeta = 0.0f;
            }
            else
            {
                camera.eyeBeta -= cameraSpd;
            }
        }

        //glm::vec3 direction;
        //direction.x = cos(glm::radians(camera.eyeBeta)) * cos(glm::radians(camera.eyeAlpha));
        //direction.y = cos(glm::radians(camera.eyeAlpha));
        //direction.z = sin(glm::radians(camera.eyeBeta)) * cos(glm::radians(camera.eyeAlpha));
        //camera.eyeFront = glm::normalize(direction);

        //std::cout << "Eye front: {" << camera.eyeFront.x << ", " << camera.eyeFront.y << ", " << camera.eyeFront.z << " }" << std::endl;

        camera.viewMtx = glm::lookAt(camera.eyePos, camera.eyePos + camera.eyeFront, camera.upVec);
        camera.projMtx = glm::perspective(glm::radians(camera.fov),
                                            static_cast<float>((GLHelper::windowRatioX * GLHelper::width) / 
                                                (GLHelper::windowRatioY * GLHelper::height)),
                                            camera.nearPlane, camera.farPlane);
    }

    void CameraManager::UpdateCameraInput()
    {
        //Camera movement keys
        int keyA = glfwGetKey(GLHelper::ptr_window, GLFW_KEY_A);
        int keyW = glfwGetKey(GLHelper::ptr_window, GLFW_KEY_W);
        int keyS = glfwGetKey(GLHelper::ptr_window, GLFW_KEY_S);
        int keyD = glfwGetKey(GLHelper::ptr_window, GLFW_KEY_D);

        /*Camera "zoom" keys
        NOTE: Changing the FOV causes some level of distortion, similar to the fisheye effect.
        Recommended FOV value for a realistic view is usually about 45.*/
        int keyZ = glfwGetKey(GLHelper::ptr_window, GLFW_KEY_Z);
        int keyX = glfwGetKey(GLHelper::ptr_window, GLFW_KEY_X);
        
        int keyQ = glfwGetKey(GLHelper::ptr_window, GLFW_KEY_Q);
        int keyE = glfwGetKey(GLHelper::ptr_window, GLFW_KEY_E);
        int keyR = glfwGetKey(GLHelper::ptr_window, GLFW_KEY_R);
        int keyF = glfwGetKey(GLHelper::ptr_window, GLFW_KEY_F);

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

        if (GLFW_PRESS == keyQ)
        {
            yawLeft_flag = true;
        }
        else if (GLFW_RELEASE == keyQ)
        {
            yawLeft_flag = false;
        }

        if (GLFW_PRESS == keyE)
        {
            yawRight_flag = true;
        }
        else if (GLFW_RELEASE == keyE)
        {
            yawRight_flag = false;
        }

    }

    void CameraManager::SetCameraSpeed(float newSpeed)
    {
        unsigned int editorID = GetEditorCameraID();
        auto& camera = engine->world.GetComponent<CameraComponent>(editorID);

        camera.cameraSpeed = newSpeed;
    }
}