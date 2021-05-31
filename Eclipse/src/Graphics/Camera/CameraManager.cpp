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

        auto& _transform = engine->world.GetComponent<TransformComponent>(newCam);
        _transform.position = ECVec3{ 0.0f, 0.0f, 40.0f };
        _transform.rotation = ECVec3{ 0.0f, -90.0f, 0.0f };
    }

    unsigned int CameraManager::GetEditorCameraID()
    {
        return editorCamID;
    }

    void CameraManager::ComputeViewDirection(CameraComponent& _camera, TransformComponent& _transform)
    {
        glm::vec3 direction;
        direction.x = cos(glm::radians(_transform.rotation.y)) * cos(glm::radians(_transform.rotation.x));
        direction.y = sin(glm::radians(_transform.rotation.x));
        direction.z = sin(glm::radians(_transform.rotation.y)) * cos(glm::radians(_transform.rotation.x));
        _camera.eyeFront = glm::normalize(direction);
    }

    void CameraManager::ComputeViewMtx(CameraComponent& _camera, TransformComponent& _transform)
    {
        glm::vec3 eyePos = _transform.position.ConvertToGlmVec3Type();
        _camera.viewMtx = glm::lookAt(eyePos, eyePos + _camera.eyeFront, _camera.upVec);
    }

    void CameraManager::ComputePerspectiveMtx(CameraComponent& _camera)
    {
        _camera.projMtx = glm::perspective(glm::radians(_camera.fov),
            static_cast<float>((OpenGL_Context::GetWindowRatioX() * OpenGL_Context::GetWidth()) /
                (OpenGL_Context::GetWindowRatioY() * OpenGL_Context::GetHeight() )),
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
            if (_transform.rotation.y < -90.0f)
            {            
                _transform.rotation.y = 270.0f;
            }
            else
            {
                _transform.rotation.y -= cameraSpd;
            }
        }

        if (input.test(7))
        {
            if (_transform.rotation.y > 270.0f)
            {
                _transform.rotation.y = -90.0f;
            }
            else
            {
                _transform.rotation.y += cameraSpd;
            }
        }

        if (input.test(4))
        {
            if (_transform.rotation.x > 89.0f)
            {
                _transform.rotation.x = 89.0f;
            }
            else
            {
                _transform.rotation.x += cameraSpd;
            }
        }

        if (input.test(5))
        {
            if (_transform.rotation.x < -89.0f)
            {
                _transform.rotation.x = -89.0f;
            }
            else
            {
                _transform.rotation.x -= cameraSpd;
            }
        }
    }

    void CameraManager::CheckCameraInput()
    {
        InputHandler.SetIsPrint(false);

        //Camera movement keys
        //int keyA = glfwGetKey(OpenGL_Context::GetWindow(), GLFW_KEY_A);
        //int keyW = glfwGetKey(OpenGL_Context::GetWindow(), GLFW_KEY_W);
        //int keyS = glfwGetKey(OpenGL_Context::GetWindow(), GLFW_KEY_S);
        //int keyD = glfwGetKey(OpenGL_Context::GetWindow(), GLFW_KEY_D);

        /*Camera "zoom" keys
        NOTE: Changing the FOV causes some level of distortion, similar to the fisheye effect.
        Recommended FOV value for a realistic view is usually about 45.*/
        int keyZ = glfwGetKey(OpenGL_Context::GetWindow(), GLFW_KEY_Z);
        int keyX = glfwGetKey(OpenGL_Context::GetWindow(), GLFW_KEY_X);
        
        int keyQ = glfwGetKey(OpenGL_Context::GetWindow(), GLFW_KEY_Q);
        int keyE = glfwGetKey(OpenGL_Context::GetWindow(), GLFW_KEY_E);
        int keyR = glfwGetKey(OpenGL_Context::GetWindow(), GLFW_KEY_R);
        int keyF = glfwGetKey(OpenGL_Context::GetWindow(), GLFW_KEY_F);

        if (InputHandler.GetKeyCurrent(InputKeycode::Key_A))
        {
            input.set(1, 1);
        }
        else
        {
            input.set(1, 0);
        }

        if (InputHandler.GetKeyCurrent(InputKeycode::Key_S))
        {
            input.set(3, 1);
        }
        else
        {
            input.set(3, 0);
        }

        if (InputHandler.GetKeyCurrent(InputKeycode::Key_D))
        {
            input.set(0, 1);
        }
        else 
        {
            input.set(0, 0);
        }

        if (InputHandler.GetKeyCurrent(InputKeycode::Key_W))
        {
            input.set(2, 1);
        }
        else 
        {
            input.set(2, 0);
        }

        //if (GLFW_PRESS == keyA)
        //{
        //    input.set(1, 1);
        //}
        //else if (GLFW_RELEASE == keyA)
        //{
        //    input.set(1, 0);
        //}

        //if (GLFW_PRESS == keyS)
        //{
        //    input.set(3, 1);
        //}
        //else if (GLFW_RELEASE == keyS)
        //{
        //    input.set(3, 0);
        //}

        //if (GLFW_PRESS == keyD)
        //{
        //    input.set(0, 1);
        //}
        //else if (GLFW_RELEASE == keyD)
        //{
        //    input.set(0, 0);
        //}

        //if (GLFW_PRESS == keyW)
        //{
        //    input.set(2, 1);
        //}
        //else if (GLFW_RELEASE == keyW)
        //{
        //    input.set(2, 0);
        //}

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

        if (GLFW_PRESS == keyR)
        {
            input.set(4, 1);
        }
        else if (GLFW_RELEASE == keyR)
        {
            input.set(4, 0);
        }

        if (GLFW_PRESS == keyF)
        {
            input.set(5, 1);
        }
        else if (GLFW_RELEASE == keyF)
        {
            input.set(5, 0);
        }
    }

    void CameraManager::SetCameraSpeed(float newSpeed)
    {
        unsigned int editorID = GetEditorCameraID();
        auto& camera = engine->world.GetComponent<CameraComponent>(editorID);

        camera.cameraSpeed = newSpeed;
    }
}