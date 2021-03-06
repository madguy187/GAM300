#include "pch.h"
#include "BaseSwitchView.h"

namespace Eclipse
{
    void BaseSwitchViewWindow::Update()
    {
        if (IsVisible)
            ECGui::DrawMainWindow<void()>(WindowName, std::bind(&BaseSwitchViewWindow::RunMainWindow, this));
    }

    void BaseSwitchViewWindow::Init()
    {
        FBO_Mode = FrameBufferMode::FBM_SCENE;
        mCamType = CameraComponent::CameraType::TopView_Camera;
        mViewportSize = glm::vec2{ 0.f, 0.f };
        Type = EditorWindowType::EWT_SWITCHVIEW_RIGHT;
        WindowName = "Right Switch View";
        IsVisible = false;
        m_frameBuffer = engine->gFrameBufferManager->GetFramebuffer(FrameBufferMode::FBM_RIGHT);
        mProjectionView_List.push_back("Orthographic");
    }

    void BaseSwitchViewWindow::Unload()
    {
    }

    void BaseSwitchViewWindow::RunMainWindow()
    {
        ImVec2 viewportPanelSize = ECGui::GetWindowSize();
        int GizmoType = (engine->editorManager->GetEditorWindow<SceneWindow>())->GetGizmoType();

        if (mViewportSize.getX() != viewportPanelSize.x ||
            mViewportSize.getY() != viewportPanelSize.y)
        {
            m_frameBuffer->Resize(static_cast<unsigned>(viewportPanelSize.x), static_cast<unsigned>(viewportPanelSize.y), FBO_Mode);
            mViewportSize = { viewportPanelSize.x, viewportPanelSize.y };
            engine->gFrameBufferManager->UpdateAspectRatio(FBO_Mode, mViewportSize);
        }

        ChildSettings settings;
        settings.Name = "RightSwitchViewFrameBuffers";
        settings.Size = ImVec2{ mViewportSize.x, mViewportSize.y };
        ECGui::DrawChildWindow<void(int)>(settings,
            std::bind(&BaseSwitchViewWindow::RunFrameBuffer, this, std::placeholders::_1),
            GizmoType);
    }

    void BaseSwitchViewWindow::RunFrameBuffer(int GizmoType)
    {
        ECGui::Image((void*)(static_cast<size_t>(m_frameBuffer->GetTextureColourBufferID())),
            ImVec2{ mViewportSize.x, mViewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

        /*if (!engine->editorManager->IsEntityListEmpty() && GizmoType != -1)
            OnGizmoUpdateEvent(GizmoType);*/

        if (ECGui::IsItemHovered())
        {
            // Do all the future stuff here
            OnCameraZoomEvent();
            OnCameraMoveEvent();

            if (!engine->editorManager->IsEntityListEmpty() && GizmoType != -1)
                OnGizmoUpdateEvent(GizmoType);

            IsWindowHovering = true;
        }
        else
        {
            IsWindowHovering = false;
        }

        if (ECGui::IsItemActive())
            IsWindowActive = true;
        else
            IsWindowActive = false;
    }

    void BaseSwitchViewWindow::OnGizmoUpdateEvent(int GizmoType)
    {
        Entity selectedEntity = engine->editorManager->GetSelectedEntity();

        ImGuizmo::SetOrthographic(true);
        ImGuizmo::SetDrawlist();

        float windowWidth = (float)ECGui::GetWindowWidth();
        float windowHeight = (float)ECGui::GetWindowHeight();
        ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowWidth, windowHeight);

        // Camera
        Entity cameraEntity = static_cast<Entity>(engine->gCamera.GetCameraID(mCamType));
        const auto& camCom = engine->world.GetComponent<CameraComponent>(cameraEntity);
        // Selected Entity Transform
        auto& transCom = engine->world.GetComponent<TransformComponent>(selectedEntity);
        glm::mat4 transform{};

        ImGuizmo::RecomposeMatrixFromComponents(glm::value_ptr(transCom.position.ConvertToGlmVec3Type()),
            glm::value_ptr(transCom.rotation.ConvertToGlmVec3Type()),
            glm::value_ptr(transCom.scale.ConvertToGlmVec3Type()),
            glm::value_ptr(transform));

        // Snapping
        glm::vec3 snapValues{};

        switch (GizmoType)
        {
        case ImGuizmo::OPERATION::TRANSLATE:
            snapValues =
            { engine->editorManager->GetEditorWindow<SceneWindow>()->GetSnapSettings().mPosSnapValue,
              engine->editorManager->GetEditorWindow<SceneWindow>()->GetSnapSettings().mPosSnapValue,
              engine->editorManager->GetEditorWindow<SceneWindow>()->GetSnapSettings().mPosSnapValue };
            break;
        case ImGuizmo::OPERATION::ROTATE:
            snapValues =
            { engine->editorManager->GetEditorWindow<SceneWindow>()->GetSnapSettings().mRotSnapValue,
              engine->editorManager->GetEditorWindow<SceneWindow>()->GetSnapSettings().mRotSnapValue,
              engine->editorManager->GetEditorWindow<SceneWindow>()->GetSnapSettings().mRotSnapValue };
            break;
        case ImGuizmo::OPERATION::SCALE:
            snapValues =
            { engine->editorManager->GetEditorWindow<SceneWindow>()->GetSnapSettings().mScaleSnapValue,
              engine->editorManager->GetEditorWindow<SceneWindow>()->GetSnapSettings().mScaleSnapValue,
              engine->editorManager->GetEditorWindow<SceneWindow>()->GetSnapSettings().mScaleSnapValue };
            break;
        default:
            break;
        }

        // ImGuiIO& io = ImGui::GetIO();

        ImGuizmo::Manipulate(glm::value_ptr(camCom.viewMtx), glm::value_ptr(camCom.projMtx),
            (ImGuizmo::OPERATION)GizmoType, ImGuizmo::LOCAL, glm::value_ptr(transform),
            nullptr, engine->editorManager->GetEditorWindow<SceneWindow>()->GetSnapping() ? glm::value_ptr(snapValues) : nullptr);

        if (ImGuizmo::IsUsing() && ECGui::IsItemHovered())
        {
            glm::vec3 translation, rotation, scale;
            ImGuizmo::DecomposeMatrixToComponents(glm::value_ptr(transform), glm::value_ptr(translation),
                glm::value_ptr(rotation), glm::value_ptr(scale));

            // glm::vec3 deltaRotation = rotation - transCom.rotation.ConvertToGlmVec3Type();
            auto& ent = engine->world.GetComponent<EntityComponent>(selectedEntity);

            switch (GizmoType)
            {
            case ImGuizmo::OPERATION::TRANSLATE:
                transCom.position = translation;
                CommandHistory::RegisterCommand(new ECVec3DeltaCommand{ transCom.position, transCom.position });

                if (engine->world.CheckComponent<ChildComponent>(selectedEntity))
                {
                    auto& child = engine->world.GetComponent<ChildComponent>(selectedEntity);
                    child.UpdateChildren = true;

                    if (ent.Tag != EntityType::ENT_MESH)
                    {
                        engine->gPicker.UpdateAabb(selectedEntity);
                        engine->gDynamicAABBTree.UpdateData(selectedEntity);
                    }
                }
                break;
            case ImGuizmo::OPERATION::ROTATE:
                transCom.rotation = rotation;
                CommandHistory::RegisterCommand(new ECVec3DeltaCommand{ transCom.rotation, transCom.rotation });
                break;
            case ImGuizmo::OPERATION::SCALE:
                transCom.scale = scale;
                CommandHistory::RegisterCommand(new ECVec3DeltaCommand{ transCom.scale, transCom.scale });
                break;
            default:
                break;
            }

            //Update for DynamicAABB Tree -Rachel
            engine->gPicker.UpdateAabb(selectedEntity);
            engine->gDynamicAABBTree.UpdateData(selectedEntity);
            if ((engine->world.CheckComponent<ParentComponent>(selectedEntity)) && (ent.Tag != EntityType::ENT_MODEL))
            {
                auto& parent = engine->world.GetComponent<ParentComponent>(selectedEntity);

                for (auto& it : parent.child)
                {
                    // auto& transform = engine->world.GetComponent<TransformComponent>(it);

                    engine->gPicker.UpdateAabb(it);
                    engine->gDynamicAABBTree.UpdateData(it);
                }
            }
        }
        else if (!ImGuizmo::IsUsing() && ImGui::IsMouseReleased(0)
            && ECGui::IsItemHovered())
        {
            CommandHistory::DisableMergeForMostRecentCommand();

            if (engine->world.CheckComponent<ChildComponent>(selectedEntity))
            {
                auto& child = engine->world.GetComponent<ChildComponent>(selectedEntity);
                child.UpdateChildren = false;
            }
        }

    }

    void BaseSwitchViewWindow::OnCameraZoomEvent()
    {
        ImGuiIO& io = ImGui::GetIO();

        if (io.MouseWheel != 0.0f)
        {
            // ImGui Scroll Up Detection
            if (io.MouseWheel > 0.0f)
                engine->gCamera.GetViewInput().set(4, 1);
            else
                engine->gCamera.GetViewInput().set(4, 0);

            // ImGui Scroll Down Detection
            if (io.MouseWheel < 0.0f)
                engine->gCamera.GetViewInput().set(5, 1);
            else
                engine->gCamera.GetViewInput().set(5, 0);
        }
        else
        {
            engine->gCamera.GetViewInput().set(4, 0);
            engine->gCamera.GetViewInput().set(5, 0);
        }
    }

    void BaseSwitchViewWindow::OnCameraMoveEvent()
    {
        ImGuiIO& io = ImGui::GetIO();
        ImVec2 value_with_lock_threshold = ECGui::GetMouseDragDelta(1);
        const float benchmarkValue = 0.0f;

        // ImGui Right Click Detection
        if (ECGui::IsMouseDragging(1))
        {
            // Camera Move Right
            if (value_with_lock_threshold.x > benchmarkValue && io.MouseDelta.x > 0.0f)
                engine->gCamera.GetViewInput().set(0, 1);
            else
                engine->gCamera.GetViewInput().set(0, 0);

            // Camera Move Left
            if (value_with_lock_threshold.x < benchmarkValue && io.MouseDelta.x < 0.0f)
                engine->gCamera.GetViewInput().set(1, 1);
            else
                engine->gCamera.GetViewInput().set(1, 0);

            // Camera Move Down
            if (value_with_lock_threshold.y > benchmarkValue && io.MouseDelta.y > 0.0f)
                engine->gCamera.GetViewInput().set(2, 1);
            else
                engine->gCamera.GetViewInput().set(2, 0);

            // Camera Move Up
            if (value_with_lock_threshold.y < benchmarkValue && io.MouseDelta.y < 0.0f)
                engine->gCamera.GetViewInput().set(3, 1);
            else
                engine->gCamera.GetViewInput().set(3, 0);
        }
        else
        {
            engine->gCamera.GetViewInput().set(0, 0);
            engine->gCamera.GetViewInput().set(1, 0);
            engine->gCamera.GetViewInput().set(2, 0);
            engine->gCamera.GetViewInput().set(3, 0);
        }
    }

    bool BaseSwitchViewWindow::GetIsWindowActive() const
    {
        return IsWindowActive;
    }
    bool BaseSwitchViewWindow::GetIsWindowHovered() const
    {
        return IsWindowHovering;
    }
}
