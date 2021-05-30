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
        engine->world.AddComponent(newCam, TransformComponent{});

        editorCamID = newCam;

        auto& editorCam = engine->world.GetComponent<CameraComponent>(newCam);
        editorCam.camType = CameraComponent::CameraType::Editor_Camera;
    }

    unsigned int CameraManager::GetEditorCameraID()
    {
        return editorCamID;
    }

    void CameraManager::ComputeViewMtx(CameraComponent& _camera, TransformComponent& _transform)
    {
        glm::vec3 eyePos = _transform.position.ConvertToGlmVec3Type();
        _camera.viewMtx = glm::lookAt(eyePos, eyePos + _camera.eyeFront, _camera.upVec);
    }

    void CameraManager::ComputePerspectiveMtx(CameraComponent& _camera)
    {
        _camera.projMtx = glm::perspective(glm::radians(_camera.fov),
            static_cast<float>((OpenGL_Context::windowRatioX * OpenGL_Context::width) /
                (OpenGL_Context::windowRatioY * OpenGL_Context::height)),
            _camera.nearPlane, _camera.farPlane);
    }

    void CameraManager::UpdateEditorCamera(TransformComponent& _transform)
    {
        unsigned int editorID = GetEditorCameraID();
        auto& camera = engine->world.GetComponent<CameraComponent>(editorID);

        float cameraSpd = OpenGL_Context::deltaTime * camera.cameraSpeed;

        if (input.test(0))
        {
            _transform.position += glm::normalize(glm::cross(camera.eyeFront, camera.upVec)) * cameraSpd;
        }

        if (input.test(1))
        {
            _transform.position -= glm::normalize(glm::cross(camera.eyeFront, camera.upVec)) * cameraSpd;
        }

        if (input.test(2))
        {
            _transform.position += camera.eyeFront * cameraSpd;
        }

        if (input.test(3))
        {
            _transform.position -= camera.eyeFront * cameraSpd;
        }

        if (input.test(8))
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

        if (input.test(9))
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

        if (input.test(6))
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

        if (input.test(7))
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
    }

    void CameraManager::CheckCameraInput()
    {
        //Camera movement keys
        int keyA = glfwGetKey(OpenGL_Context::ptr_window, GLFW_KEY_A);
        int keyW = glfwGetKey(OpenGL_Context::ptr_window, GLFW_KEY_W);
        int keyS = glfwGetKey(OpenGL_Context::ptr_window, GLFW_KEY_S);
        int keyD = glfwGetKey(OpenGL_Context::ptr_window, GLFW_KEY_D);

        /*Camera "zoom" keys
        NOTE: Changing the FOV causes some level of distortion, similar to the fisheye effect.
        Recommended FOV value for a realistic view is usually about 45.*/
        int keyZ = glfwGetKey(OpenGL_Context::ptr_window, GLFW_KEY_Z);
        int keyX = glfwGetKey(OpenGL_Context::ptr_window, GLFW_KEY_X);
        
        int keyQ = glfwGetKey(OpenGL_Context::ptr_window, GLFW_KEY_Q);
        int keyE = glfwGetKey(OpenGL_Context::ptr_window, GLFW_KEY_E);
        int keyR = glfwGetKey(OpenGL_Context::ptr_window, GLFW_KEY_R);
        int keyF = glfwGetKey(OpenGL_Context::ptr_window, GLFW_KEY_F);

        if (GLFW_PRESS == keyA)
        {
            input.set(1, 1);
        }
        else if (GLFW_RELEASE == keyA)
        {
            input.set(1, 0);
        }

        if (GLFW_PRESS == keyS)
        {
            input.set(3, 1);
        }
        else if (GLFW_RELEASE == keyS)
        {
            input.set(3, 0);
        }

        if (GLFW_PRESS == keyD)
        {
            input.set(0, 1);
        }
        else if (GLFW_RELEASE == keyD)
        {
            input.set(0, 0);
        }

        if (GLFW_PRESS == keyW)
        {
            input.set(2, 1);
        }
        else if (GLFW_RELEASE == keyW)
        {
            input.set(2, 0);
        }

        if (GLFW_PRESS == keyZ)
        {
            input.set(8, 1);
        }
        else if (GLFW_RELEASE == keyZ)
        {
            input.set(8, 0);
        }

        if (GLFW_PRESS == keyX)
        {
            input.set(9, 1);
        }
        else if (GLFW_RELEASE == keyX)
        {
            input.set(9, 0);
        }

        if (GLFW_PRESS == keyQ)
        {
            input.set(6, 1);
        }
        else if (GLFW_RELEASE == keyQ)
        {
            input.set(6, 0);
        }

        if (GLFW_PRESS == keyE)
        {
            input.set(7, 1);
        }
        else if (GLFW_RELEASE == keyE)
        {
            input.set(7, 0);
        }
    }

    void CameraManager::SetCameraSpeed(float newSpeed)
    {
        unsigned int editorID = GetEditorCameraID();
        auto& camera = engine->world.GetComponent<CameraComponent>(editorID);

        camera.cameraSpeed = newSpeed;
    }
}