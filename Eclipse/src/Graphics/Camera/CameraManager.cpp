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

        Entity newCam = engine->editorManager->CreateDefaultEntity(EntityType::ENT_GAMECAMERA);

        //engine->world.AddComponent(newCam, EntityComponent{EntityType::ENT_CAMERA,
        // lexical_cast_toStr<EntityType>(EntityType::ENT_CAMERA)});
        engine->world.AddComponent(newCam, CameraComponent{});
        // engine->world.AddComponent(newCam, TransformComponent{});

        editorCamID = newCam;

        auto& editorCam = engine->world.GetComponent<CameraComponent>(newCam);
        editorCam.camType = CameraComponent::CameraType::Editor_Camera;
        editorCam.projType = CameraComponent::ProjectionType::Perspective;

        auto& _transform = engine->world.GetComponent<TransformComponent>(newCam);
        _transform.position = EDITORCAM_INITPOS;
        _transform.rotation = EDITORCAM_INITROT;

        cameraList.emplace(CameraComponent::CameraType::Editor_Camera, newCam);
    }

    unsigned int CameraManager::GetEditorCameraID()
    {
        return editorCamID;
    }

    void CameraManager::CreateGameCamera()
    {
        if (gameCamID != MAX_ENTITY)
        {
            return;
        }

        Entity newCam = engine->world.CreateEntity();
        engine->world.AddComponent(newCam, CameraComponent{});
        engine->world.AddComponent(newCam, TransformComponent{});

        gameCamID = newCam;

        auto& gameCam = engine->world.GetComponent<CameraComponent>(newCam);
        gameCam.camType = CameraComponent::CameraType::Game_Camera;
        gameCam.projType = CameraComponent::ProjectionType::Perspective;

        auto& _transform = engine->world.GetComponent<TransformComponent>(newCam);
        _transform.position = GAMECAM_INITPOS;
        _transform.rotation = GAMECAM_INITROT;

        cameraList.emplace(CameraComponent::CameraType::Game_Camera, newCam);
    }

    unsigned int CameraManager::GetGameCameraID()
    {
        return gameCamID;
    }

    void CameraManager::CreateViewCamera(CameraComponent::CameraType _camType)
    {
        Entity newCam = engine->world.CreateEntity();
        engine->world.AddComponent(newCam, CameraComponent{});
        engine->world.AddComponent(newCam, TransformComponent{});

        auto& viewCam = engine->world.GetComponent<CameraComponent>(newCam);
        viewCam.camType = _camType;
        viewCam.projType = CameraComponent::ProjectionType::Orthographic;

        auto& _transform = engine->world.GetComponent<TransformComponent>(newCam);
        SetViewCameraValues(_camType, _transform);

        cameraList.emplace(_camType, newCam);
    }


    void CameraManager::SetViewCameraValues(CameraComponent::CameraType _camType, TransformComponent& _transform)
    {
        switch (_camType)
        {
        case CameraComponent::CameraType::TopView_Camera:
        {
            _transform.position = TOPCAM_INITPOS;
            _transform.rotation = TOPCAM_INITROT;
            _transform.scale = TOPCAM_INITSCALE;
        }
        break;
        case CameraComponent::CameraType::BottomView_Camera:
        {
            _transform.position = BOTTOMCAM_INITPOS;
            _transform.rotation = BOTTOMCAM_INITROT;
            _transform.scale = BOTTOMCAM_INITSCALE;
        }
        break;
        case CameraComponent::CameraType::LeftView_Camera:
        {
            _transform.position = LEFTCAM_INITPOS;
            _transform.rotation = LEFTCAM_INITROT;
            _transform.scale = LEFTCAM_INITSCALE;
        }
        break;
        case CameraComponent::CameraType::RightView_camera:
        {
            _transform.position = RIGHTCAM_INITPOS;
            _transform.rotation = RIGHTCAM_INITROT;
            _transform.scale = RIGHTCAM_INITSCALE;
        }
        break;
        }
    }

    void CameraManager::SetViewCameraProjectionType(CameraComponent& _camera, CameraComponent::ProjectionType _projType)
    {
        _camera.projType = _projType;
    }

    void CameraManager::SetViewCameraProjectionType(CameraComponent::CameraType _camType, CameraComponent::ProjectionType _projType)
    {
        auto& camera = engine->world.GetComponent<CameraComponent>(GetCameraID(_camType));
        camera.projType = _projType;
    }

    unsigned int CameraManager::GetCameraID(CameraComponent::CameraType _camType)
    {
        return cameraList[_camType];
    }

    void CameraManager::ComputeViewDirection(CameraComponent& _camera, TransformComponent& _transform)
    {
        glm::vec3 direction;
        direction.x = cos(glm::radians(_transform.rotation.y)) * cos(glm::radians(_transform.rotation.x));
        direction.y = sin(glm::radians(_transform.rotation.x));
        direction.z = sin(glm::radians(_transform.rotation.y)) * cos(glm::radians(_transform.rotation.x));
        _camera.eyeFront = glm::normalize(direction);

        _camera.rightVec = glm::normalize(glm::cross(_camera.eyeFront, _camera.worldUp));
        _camera.upVec = glm::normalize(glm::cross(_camera.rightVec, _camera.eyeFront));
    }

    void CameraManager::ComputeViewMtx(CameraComponent& _camera, TransformComponent& _transform)
    {
        glm::vec3 eyePos = _transform.position.ConvertToGlmVec3Type();
        _camera.eyePos = eyePos;
        _camera.viewMtx = glm::lookAt(eyePos, eyePos + _camera.eyeFront, _camera.upVec);
    }

    void CameraManager::ComputePerspectiveMtx(CameraComponent& _camera)
    {
        auto& editorCam = engine->world.GetComponent<CameraComponent>(GetEditorCameraID());
        auto& transform = engine->world.GetComponent<TransformComponent>(GetCameraID(_camera.camType));

        //_camera.aspect = static_cast<float>((OpenGL_Context::GetWindowRatioX() * OpenGL_Context::GetWidth()) / (OpenGL_Context::GetWindowRatioY() * OpenGL_Context::GetHeight()));

        if (_camera.projType == CameraComponent::ProjectionType::Orthographic)
        {
            // Darren was here . Actually wanna check only scene using ortho? if yes for now i hardcode ortho.
            _camera.aspect = engine->gFrameBufferManager->GetAspectRatio(FrameBufferMode::FBM_GAME);

            _camera.projMtx = glm::ortho(static_cast<float>(-(OpenGL_Context::GetWidth()) * _camera.aspect) / transform.scale.x,
                static_cast<float>((OpenGL_Context::GetWidth()) * _camera.aspect) / transform.scale.x,
                static_cast<float>(-(OpenGL_Context::GetHeight()) * _camera.aspect) / transform.scale.x,
                static_cast<float>((OpenGL_Context::GetHeight()) * _camera.aspect) / transform.scale.x,
                editorCam.nearPlane, editorCam.farPlane);
        }
        else
        {
            // Darren was here . Actually wanna check only scene using perspective? if yes for now i hardcode scene.
            _camera.aspect = engine->gFrameBufferManager->GetAspectRatio(FrameBufferMode::FBM_SCENE);

            if ((OpenGL_Context::GetWidth() != 0) && (OpenGL_Context::GetHeight() != 0))
            {
                _camera.projMtx = glm::perspective(glm::radians(_camera.fov),
                    _camera.aspect, _camera.nearPlane, _camera.farPlane);
            }
        }

    }

    void CameraManager::UpdateEditorCamera(TransformComponent& _transform)
    {
        unsigned int editorID = GetEditorCameraID();
        auto& camera = engine->world.GetComponent<CameraComponent>(editorID);

        float cameraSpd = engine->Game_Clock.get_DeltaTime() * camera.cameraSpeed;

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

        if (input.test(10))
        {
            _transform.position += camera.upVec * cameraSpd;
        }

        if (input.test(11))
        {
            _transform.position -= camera.upVec * cameraSpd;
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

        int keyT = glfwGetKey(OpenGL_Context::GetWindow(), GLFW_KEY_T);
        int keyG = glfwGetKey(OpenGL_Context::GetWindow(), GLFW_KEY_G);

        //int keyP = glfwGetKey(OpenGL_Context::GetWindow(), GLFW_KEY_P);

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

        if (GLFW_PRESS == keyT)
        {
            input.set(10, 1);
        }
        else if (GLFW_RELEASE == keyT)
        {
            input.set(10, 0);
        }

        if (GLFW_PRESS == keyG)
        {
            input.set(11, 1);
        }
        else if (GLFW_RELEASE == keyG)
        {
            input.set(11, 0);
        }

    }

    void CameraManager::CheckViewCameraInput()
    {
        int keyA = glfwGetKey(OpenGL_Context::GetWindow(), GLFW_KEY_A);
        int keyW = glfwGetKey(OpenGL_Context::GetWindow(), GLFW_KEY_W);
        int keyS = glfwGetKey(OpenGL_Context::GetWindow(), GLFW_KEY_S);
        int keyD = glfwGetKey(OpenGL_Context::GetWindow(), GLFW_KEY_D);

        int keyZ = glfwGetKey(OpenGL_Context::GetWindow(), GLFW_KEY_Z);
        int keyX = glfwGetKey(OpenGL_Context::GetWindow(), GLFW_KEY_X);

        int keyR = glfwGetKey(OpenGL_Context::GetWindow(), GLFW_KEY_R);
        int keyF = glfwGetKey(OpenGL_Context::GetWindow(), GLFW_KEY_F);

        if (GLFW_PRESS == keyA)
        {
            viewInput.set(0, 1);
        }
        else if (GLFW_RELEASE == keyA)
        {
            viewInput.set(0, 0);
        }

        if (GLFW_PRESS == keyS)
        {
            viewInput.set(3, 1);
        }
        else if (GLFW_RELEASE == keyS)
        {
            viewInput.set(3, 0);
        }

        if (GLFW_PRESS == keyD)
        {
            viewInput.set(1, 1);
        }
        else if (GLFW_RELEASE == keyD)
        {
            viewInput.set(1, 0);
        }

        if (GLFW_PRESS == keyW)
        {
            viewInput.set(2, 1);
        }
        else if (GLFW_RELEASE == keyW)
        {
            viewInput.set(2, 0);
        }

        if (GLFW_PRESS == keyZ)
        {
            viewInput.set(4, 1);
        }
        else if (GLFW_RELEASE == keyZ)
        {
            viewInput.set(4, 0);
        }

        if (GLFW_PRESS == keyX)
        {
            viewInput.set(5, 1);
        }
        else if (GLFW_RELEASE == keyX)
        {
            viewInput.set(5, 0);
        }

        if (GLFW_PRESS == keyR)
        {
            viewInput.set(6, 1);
        }
        else if (GLFW_RELEASE == keyR)
        {
            viewInput.set(6, 0);
        }

        if (GLFW_PRESS == keyF)
        {
            viewInput.set(7, 1);
        }
        else if (GLFW_RELEASE == keyF)
        {
            viewInput.set(7, 0);
        }
    }

    void CameraManager::UpdateViewCamera(CameraComponent& _camera, TransformComponent& _transform)
    {
        float cameraSpd = engine->Game_Clock.get_DeltaTime() * _camera.cameraSpeed;

        if (viewInput.test(0))
        {
            _transform.position -= glm::normalize(glm::cross(_camera.eyeFront, _camera.upVec)) * cameraSpd;
        }

        if (viewInput.test(1))
        {
            _transform.position += glm::normalize(glm::cross(_camera.eyeFront, _camera.upVec)) * cameraSpd;
        }

        if (viewInput.test(2))
        {
            _transform.position += _camera.upVec * cameraSpd;
        }

        if (viewInput.test(3))
        {
            _transform.position -= _camera.upVec * cameraSpd;
        }

        if (_camera.projType == CameraComponent::ProjectionType::Orthographic)
        {
            if (viewInput.test(4))
            {
                _transform.scale.x += cameraSpd;
            }

            if (viewInput.test(5))
            {
                _transform.scale.x -= cameraSpd;
            }
        }
        else
        {
            if (viewInput.test(4))
            {
                _transform.position += _camera.eyeFront * cameraSpd;
            }

            if (viewInput.test(5))
            {
                _transform.position -= _camera.eyeFront * cameraSpd;
            }

            if (viewInput.test(6))
            {
                if (_camera.fov < 2.0f)
                {
                    _camera.fov = 1.0f;
                }
                else
                {
                    _camera.fov -= cameraSpd;
                }
            }

            if (viewInput.test(7))
            {
                if (_camera.fov > 179.0f)
                {
                    _camera.fov = 180.0f;
                }
                else
                {
                    _camera.fov += cameraSpd;
                }
            }
        }
    }

    void CameraManager::SetCameraSpeed(float newSpeed)
    {
        unsigned int editorID = GetEditorCameraID();
        auto& camera = engine->world.GetComponent<CameraComponent>(editorID);

        camera.cameraSpeed = newSpeed;
    }

    void CameraManager::SetNearPlane(CameraComponent& _camera, float _nearPlane)
    {
        _camera.nearPlane = _nearPlane;
    }

    void CameraManager::SetFarPlane(CameraComponent& _camera, float _farPlane)
    {
        _camera.farPlane = _farPlane;
    }

    std::bitset<12>& CameraManager::GetInput()
    {
        return input;
    }

    std::bitset<8>& CameraManager::GetViewInput()
    {
        return viewInput;
    }

    void CameraManager::ResetScene()
    {
        editorCamID = MAX_ENTITY;
        gameCamID = MAX_ENTITY;

        engine->gDebugManager.ClearDebugShapes();
        cameraList.clear();

        //Re-create and initialize all cameras
        engine->gCamera.CreateEditorCamera();
        engine->gCamera.SetCameraSpeed(50.0f);

        engine->gCamera.CreateViewCamera(CameraComponent::CameraType::TopView_Camera);
        engine->gCamera.CreateViewCamera(CameraComponent::CameraType::BottomView_Camera);
        engine->gCamera.CreateViewCamera(CameraComponent::CameraType::LeftView_Camera);
        engine->gCamera.CreateViewCamera(CameraComponent::CameraType::RightView_camera);

        engine->gCamera.CreateGameCamera();
        auto& _camera = engine->world.GetComponent<CameraComponent>(engine->gCamera.GetGameCameraID());
        engine->gDebugManager.AddCameraFrustum(engine->gCamera.GetGameCameraID());
        engine->gCamera.SetFarPlane(_camera, 100.0f);
    }

    void CameraManager::ReInitCameraList(CameraComponent::CameraType _camType, unsigned int ID)
    {
        if (_camType == CameraComponent::CameraType::Game_Camera)
        {
            engine->gDebugManager.DeleteDebugShape(GetGameCameraID());

            if (engine->GetEditorState())
            {
                engine->editorManager->DestroyEntity(gameCamID);
            }
            else
            {
                engine->world.DestroyEntity(gameCamID);
            }

            gameCamID = ID;
            engine->gDebugManager.AddCameraFrustum(ID);
        }
        else if (_camType == CameraComponent::CameraType::Editor_Camera)
        {
            if (engine->GetEditorState())
            {
                engine->editorManager->DestroyEntity(editorCamID);
            }
            else
            {
                engine->world.DestroyEntity(editorCamID);
            }
            editorCamID = ID;
        }

        cameraList[_camType] = ID;
    }
}