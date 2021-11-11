#include "pch.h"
#include "InputSystem.h"

#include "GLM/glm/gtc/matrix_transform.hpp"
#include "GLM/glm/gtc/type_ptr.hpp"
#include "GLM/glm/gtx/rotate_vector.hpp"
#include "GLM/glm/gtx/vector_angle.hpp"
#include "ECS/SystemManager/Systems/System/EntityCompSystem/EntityCompSystem.h"

namespace Eclipse
{
    void InputSystem::Update()
    {
        if (engine->InputManager->GetKeyTriggered(InputKeycode::Key_L))
        {
            auto entsystem = engine->world.GetSystem<EntityCompSystem>();
            Entity ID = entsystem->FindEntity("Ray Direction");

            if (ID != MAX_ENTITY)
            {
                // if (engine->world.CheckComponent<CollisionComponent>(ID))
                {
                    auto& transCom = engine->world.GetComponent<TransformComponent>(ID);
                    PxOverlapBuffer hit;
                    if (engine->gPhysics.CheckSphere(transCom.position, transCom.scale.getX() / 2.0f, hit))
                    {
                        Entity hitID = *(Entity*)(hit.block.actor->userData);
                        auto& hittransCom = engine->world.GetComponent<TransformComponent>(hitID);
                        hittransCom.position = transCom.position;
                    }
                }
            }
        }

        auto& Cam = engine->world.GetComponent<CameraComponent>(engine->gCamera.GetEditorCameraID());
        auto& CamT = engine->world.GetComponent<TransformComponent>(engine->gCamera.GetEditorCameraID());

        if (glfwGetMouseButton(OpenGL_Context::ptr_window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
        {
            glfwSetInputMode(OpenGL_Context::ptr_window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
            ImGui::SetMouseCursor(ImGuiMouseCursor_None);

            auto* scene = engine->editorManager->GetEditorWindow<eGameViewWindow>();

            float GameWindowMinX = scene->vMin.x - OpenGL_Context::GetContextPosition().x;
            float GameWindowMaxX = scene->vMin.x + scene->GetViewPortSize().getX() - OpenGL_Context::GetContextPosition().x;
            float GameWindowCenterX = (GameWindowMaxX + GameWindowMinX) / 2;

            float GameWindowMinY = scene->vMin.y - OpenGL_Context::GetContextPosition().y;
            float GameWindowMaxY = scene->vMin.y + scene->GetViewPortSize().getY() - OpenGL_Context::GetContextPosition().y;
            float GameWindowCenterY = (GameWindowMinY + GameWindowMaxY) / 2;

            //glfwSetCursorPos(OpenGL_Context::ptr_window, GameWindowCenterX, GameWindowCenterY);
            if (firstClick)
            {
                glfwSetCursorPos(OpenGL_Context::ptr_window, GameWindowCenterX, GameWindowCenterY);
                firstClick = false;
            }

            double mouseX;
            double mouseY;
            glfwGetCursorPos(OpenGL_Context::ptr_window, &mouseX, &mouseY);

            float rotX = 100 * (float)(mouseY - (GameWindowCenterY)) / 1155;
            float rotY = 100 * (float)(mouseX - (GameWindowCenterX)) / 513;

            CamT.rotation.setY(CamT.rotation.getY() + rotY);
            CamT.rotation.setX(CamT.rotation.getX() + -rotX);

            glm::vec3 newOrientation = glm::rotate(Cam.eyeFront, glm::radians(-rotX), glm::normalize(glm::cross(Cam.eyeFront, Cam.upVec)));

            if (abs(glm::angle(newOrientation, Cam.upVec) - glm::radians(90.0f)) <= glm::radians(85.0f))
            {
                Cam.eyeFront = newOrientation;
            }

            Cam.eyeFront = glm::rotate(Cam.eyeFront, -rotY, Cam.upVec);
            glfwSetCursorPos(OpenGL_Context::ptr_window, (GameWindowCenterX), (GameWindowCenterY));
        }
        else if (glfwGetMouseButton(OpenGL_Context::ptr_window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE)
        {
            //glfwSetInputMode(OpenGL_Context::ptr_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            firstClick = true;
        }

        glm::mat4 view = glm::mat4(1.0f);
        glm::mat4 projection = glm::mat4(1.0f);

        Cam.rightVec = glm::normalize(glm::cross(Cam.eyeFront, Cam.worldUp));
        Cam.upVec = glm::normalize(glm::cross(Cam.rightVec, Cam.eyeFront));

        Cam.aspect = engine->gFrameBufferManager->GetAspectRatio(CameraComponent::CameraType::Editor_Camera);

        //Cam.viewMtx = glm::lookAt(CamT.position.ConvertToGlmVec3Type(), CamT.position.ConvertToGlmVec3Type() + Cam.eyeFront, Cam.upVec);
        Cam.projMtx = glm::perspective(glm::radians(Cam.fov), (float)Cam.aspect, 0.1f, Cam.farPlane);

        //auto& Lite = engine->world.GetComponent<SpotLightComponent>(engine->LightManager.Hand);
        //
        //if (engine->InputManager->GetKeyTriggered(InputKeycode::Key_TAB))
        //{
        //    if (Lite.IntensityStrength <= 10.0f)
        //    {
        //        Lite.IntensityStrength += 2.0f;
        //    }
        //}
        //
        //if (Lite.IntensityStrength > 0.0f)
        //{
        //    Lite.IntensityStrength -= (engine->Game_Clock.get_DeltaTime() * 2 );
        //}
        //else
        //{
        //    Lite.IntensityStrength = 0.0f;
        //}

        engine->InputManager->CursorUpdate();
        engine->InputManager->AxisUpdate();
        engine->InputManager->UpdateKeys();
    }
}