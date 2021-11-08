#include "pch.h"
#include "MeshEditor.h"
#include "../Inspector/Inspector.h"

namespace Eclipse
{
    void MeshEditorWindow::Update()
    {
        if (IsVisible)
        {
            ECGui::DrawMainWindow<void()>(WindowName, std::bind(&MeshEditorWindow::RunMainWindow, this));
            ECGui::DrawMainWindow<void()>("Mesh Settings", std::bind(&MeshEditorWindow::RunMeshSettings, this));
        }
    }

    void MeshEditorWindow::Init()
    {
        Type = EditorWindowType::EWT_MESHEDITOR;
        WindowName = "Mesh Editor";
        m_frameBuffer = engine->gFrameBufferManager->GetFramebuffer(FrameBufferMode::FBM_MESHEDITOR);
        IsVisible = false;
    }

    void MeshEditorWindow::Unload()
    {
        if (MeshID != MAX_ENTITY)
        {
            IsVisible = false;
            //engine->world.DestroyEntity(MeshID);
            RecursiveDestroy(MeshID);
            MeshFamily.clear();
            MeshID = MAX_ENTITY;
            OldestParentID = MAX_ENTITY;
            MeshIndex = 0;
            engine->editorManager->SetMeshEditorActive(false);

            auto* insp = engine->editorManager->GetEditorWindow<InspectorWindow>();
            auto& entCom = engine->world.GetComponent<EntityComponent>(engine->editorManager->GetSelectedEntity());
            insp->SetCurrentEntityName(entCom.Name.c_str());
        }
    }

    void MeshEditorWindow::RunMainWindow()
    {
        ImVec2 viewportPanelSize = ECGui::GetWindowSize();

        if (mViewportSize.getX() != viewportPanelSize.x ||
            mViewportSize.getY() != viewportPanelSize.y)
        {
            m_frameBuffer->Resize(static_cast<unsigned>(viewportPanelSize.x),
                static_cast<unsigned>(viewportPanelSize.y), FrameBufferMode::FBM_MESHEDITOR);
            mViewportSize = { viewportPanelSize.x, viewportPanelSize.y };
            engine->gFrameBufferManager->UpdateAspectRatio(FrameBufferMode::FBM_MESHEDITOR, mViewportSize);
        }

        ChildSettings settings;
        settings.Name = "MeshEditorFrameBuffer";
        settings.Size = ImVec2{ mViewportSize.x, mViewportSize.y };
        ECGui::DrawChildWindow<void()>(settings, std::bind(&MeshEditorWindow::RunFrameBuffer, this));
    }

    void MeshEditorWindow::RunFrameBuffer()
    {
        ECGui::Image((void*)(static_cast<size_t>(m_frameBuffer->GetTextureColourBufferID())),
            ImVec2{ mViewportSize.x, mViewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

        if (ECGui::IsItemHovered())
        {
            OnCameraMoveEvent();
            OnCameraZoomEvent();
        }

        if (ImGui::IsWindowFocused())
            IsActive = true;
        else
            IsActive = false;
    }

    void MeshEditorWindow::RunMeshSettings()
    {
        std::string combo_label = my_strcat("Prefab ", MeshFamily[MeshIndex]);

        ECGui::DrawTextWidget<const char*>("Prefab IDs", EMPTY_STRING);
        ECGui::InsertSameLine();

        if (ECGui::BeginComboList("MeshEditorFamily", combo_label.c_str()))
        {
            for (size_t n = 0; n < MeshFamily.size(); n++)
            {
                const bool is_selected = (MeshIndex == n);

                if (ImGui::Selectable(my_strcat("Prefab ", MeshFamily[n]).c_str(), is_selected))
                {
                    auto* insp = engine->editorManager->GetEditorWindow<InspectorWindow>();
                    MeshIndex = n;
                    MeshID = MeshFamily[MeshIndex];

                    auto& entCom = engine->world.GetComponent<EntityComponent>(MeshID);
                    insp->SetCurrentEntityName(entCom.Name.c_str());
                }

                if (is_selected)
                    ImGui::SetItemDefaultFocus();
            }

            ECGui::EndComboList();
        }

        if (ECGui::ButtonBool("Close Mesh Editor"))
            Unload();
    }

    void MeshEditorWindow::OnCameraMoveEvent()
    {
        ImGuiIO& io = ImGui::GetIO();
        ImVec2 value_with_lock_threshold = ECGui::GetMouseDragDelta(1);
        const float benchmarkValue = 0.0f;

        // ImGui Right Click Detection
        if (ECGui::IsMouseDragging(1))
        {
            // Camera Yaw Right
            if (value_with_lock_threshold.x > benchmarkValue && io.MouseDelta.x > 0.0f)
                engine->gCamera.GetMeshInput().set(7, 1);
            else
                engine->gCamera.GetMeshInput().set(7, 0);

            // Camera Yaw Left
            if (value_with_lock_threshold.x < benchmarkValue && io.MouseDelta.x < 0.0f)
                engine->gCamera.GetMeshInput().set(6, 1);
            else
                engine->gCamera.GetMeshInput().set(6, 0);

            // Camera Pitch Down
            if (value_with_lock_threshold.y > benchmarkValue && io.MouseDelta.y > 0.0f)
                engine->gCamera.GetMeshInput().set(5, 1);
            else
                engine->gCamera.GetMeshInput().set(5, 0);

            // Camera Pitch Up
            if (value_with_lock_threshold.y < benchmarkValue && io.MouseDelta.y < 0.0f)
                engine->gCamera.GetMeshInput().set(4, 1);
            else
                engine->gCamera.GetMeshInput().set(4, 0);

            // Camera Move Front
            if (ECGui::IsKeyDown(ECGui::GetKeyIndex(ImGuiKey_W)))
                engine->gCamera.GetMeshInput().set(2, 1);
            else
                engine->gCamera.GetMeshInput().set(2, 0);

            // Camera Move Left
            if (ECGui::IsKeyDown(ECGui::GetKeyIndex(ImGuiKey_A)))
                engine->gCamera.GetMeshInput().set(1, 1);
            else
                engine->gCamera.GetMeshInput().set(1, 0);

            // Camera Move Back
            if (ECGui::IsKeyDown(ECGui::GetKeyIndex(ImGuiKey_S)))
                engine->gCamera.GetMeshInput().set(3, 1);
            else
                engine->gCamera.GetMeshInput().set(3, 0);

            // Camera Move Right
            if (ECGui::IsKeyDown(ECGui::GetKeyIndex(ImGuiKey_D)))
                engine->gCamera.GetMeshInput().set(0, 1);
            else
                engine->gCamera.GetMeshInput().set(0, 0);

            // Camera Move Up
            if (ECGui::IsKeyDown(ECGui::GetKeyIndex(ImGuiKey_Q)))
                engine->gCamera.GetMeshInput().set(10, 1);
            else
                engine->gCamera.GetMeshInput().set(10, 0);

            // Camera Move Down
            if (ECGui::IsKeyDown(ECGui::GetKeyIndex(ImGuiKey_E)))
                engine->gCamera.GetMeshInput().set(11, 1);
            else
                engine->gCamera.GetMeshInput().set(11, 0);
        }
        else
        {
            engine->gCamera.GetMeshInput().set(0, 0);
            engine->gCamera.GetMeshInput().set(1, 0);
            engine->gCamera.GetMeshInput().set(2, 0);
            engine->gCamera.GetMeshInput().set(3, 0);
            engine->gCamera.GetMeshInput().set(4, 0);
            engine->gCamera.GetMeshInput().set(5, 0);
            engine->gCamera.GetMeshInput().set(6, 0);
            engine->gCamera.GetMeshInput().set(7, 0);
            engine->gCamera.GetMeshInput().set(10, 0);
            engine->gCamera.GetMeshInput().set(11, 0);
        }
    }

    void MeshEditorWindow::OnCameraZoomEvent()
    {
        ImGuiIO& io = ImGui::GetIO();

        if (io.MouseWheel != 0.0f)
        {
            // ImGui Scroll Up Detection
            if (io.MouseWheel > 0.0f)
                engine->gCamera.GetMeshInput().set(8, 1);
            else
                engine->gCamera.GetMeshInput().set(8, 0);

            // ImGui Scroll Down Detection
            if (io.MouseWheel < 0.0f)
                engine->gCamera.GetMeshInput().set(9, 1);
            else
                engine->gCamera.GetMeshInput().set(9, 0);
        }
        else
        {
            engine->gCamera.GetMeshInput().set(8, 0);
            engine->gCamera.GetMeshInput().set(9, 0);
        }
    }

    void MeshEditorWindow::SetMeshID(Entity ID)
    {
        MeshID = ID;
        OldestParentID = ID;

        MeshFamily.push_back(ID);

        if (engine->world.CheckComponent<ParentComponent>(ID))
        {
            auto& parentCom = engine->world.GetComponent<ParentComponent>(ID);
            
            for (const auto& kid : parentCom.child)
            {
                MeshFamily.push_back(kid);
            }
        }
    }

    Entity MeshEditorWindow::GetMeshID() const
    {
        return MeshID;
    }

    Entity MeshEditorWindow::GetOldestParentID() const
    {
        return OldestParentID;
    }

    bool MeshEditorWindow::GetActiveState()
    {
        return IsActive;
    }

    void MeshEditorWindow::RecursiveDestroy(const Entity& ent)
    {
        if (engine->world.CheckComponent<ParentComponent>(ent))
        {
            auto& parentComp = engine->world.GetComponent<ParentComponent>(ent);

            for (auto& child : parentComp.child)
            {
                RecursiveDestroy(child);
            }

            engine->world.DestroyEntity(ent);
        }
        else 
        {
            engine->world.DestroyEntity(ent);
        }
    }
}