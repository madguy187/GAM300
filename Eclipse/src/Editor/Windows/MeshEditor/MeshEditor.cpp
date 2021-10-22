#include "pch.h"
#include "MeshEditor.h"

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
        IsVisible = true;
    }

    void MeshEditorWindow::Unload()
    {}

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
    }

    void MeshEditorWindow::RunMeshSettings()
    {
        static ImGuiTextFilter CompFilter;
        CompFilter.Draw();

        ShowMaterialProperty("Material", CompFilter);
        ShowTransformProperty("Transform", CompFilter);
        Buttons();
    }

    void MeshEditorWindow::Buttons()
    {
        if (ECGui::ButtonBool("Create Material", { ImGui::GetColumnWidth(), 25 }))
        {
            std::string scriptName;
            scriptName.reserve(256);
        }

        ECGui::NextColumn();

        if (ECGui::ButtonBool("Remove Material", { ImGui::GetColumnWidth(), 25 }))
        {
        }
    }

    bool MeshEditorWindow::ShowTransformProperty(const char* name, ImGuiTextFilter& filter)
    {
        if (filter.PassFilter(name) && ECGui::CreateCollapsingHeader(name))
        {
            ECGui::DrawTextWidget<const char*>("Rotation", EMPTY_STRING);
            ECGui::NextColumn();
            ECGui::PushItemWidth(ECGui::GetWindowSize().x);
            ImGuiAPI::SliderFloat3("Rotation", (float*)&engine->gPBRManager->gMaterialEditorSettings->Rotation, true, -360.f, 360.f);
        }

        return false;
    }

    bool MeshEditorWindow::ShowMaterialProperty(const char* name, ImGuiTextFilter& filter)
    {
        if (filter.PassFilter(name) && ECGui::CreateCollapsingHeader(name))
        {
            ECGui::DrawTextWidget<const char*>("HasTexture", EMPTY_STRING);
            ECGui::InsertSameLine();
            ECGui::CheckBoxBool("HasTexture", &engine->gPBRManager->gMaterialEditorSettings->CurrentMaterial->HasTexture);
            ECGui::NextColumn();

            ECGui::DrawInputTextHintWidget(my_strcat("Material Name", 1).c_str(), "Material Name", const_cast<char*>(engine->gPBRManager->gMaterialEditorSettings->MaterialName.c_str()), 256, true, ImGuiInputTextFlags_None);
            ECGui::NextColumn();

            // If We going to assign texture to his material
            if (engine->gPBRManager->gMaterialEditorSettings->CurrentMaterial->HasTexture )
            {
                ECGui::DrawInputTextHintWidget(my_strcat("Albdeo Name", 1).c_str(), "Drag Albdeo Texture here", const_cast<char*>(engine->gPBRManager->gMaterialEditorSettings->AlbedoTexture.c_str()), 256, true, ImGuiInputTextFlags_None);
                engine->editorManager->DragAndDropInst_.StringPayloadTarget("png", engine->gPBRManager->gMaterialEditorSettings->AlbedoTexture, "Albdeo Texture Inserted.", PayloadTargetType::PTT_ASSETS);
                engine->gPBRManager->Clear(engine->gPBRManager->gMaterialEditorSettings->AlbedoTexture, MaterialType::MT_ALBEDO);

                ECGui::NextColumn();
                ECGui::DrawInputTextHintWidget(my_strcat("Normal Name", 1).c_str(), "Drag Normal Texture here", const_cast<char*>(engine->gPBRManager->gMaterialEditorSettings->NormalTexture.c_str()), 256, true, ImGuiInputTextFlags_None);
                engine->editorManager->DragAndDropInst_.StringPayloadTarget("png", engine->gPBRManager->gMaterialEditorSettings->NormalTexture, "Normal Texture Inserted.", PayloadTargetType::PTT_ASSETS);
                engine->gPBRManager->Clear(engine->gPBRManager->gMaterialEditorSettings->NormalTexture, MaterialType::MT_NORMAL);

                ECGui::NextColumn();
                ECGui::DrawInputTextHintWidget(my_strcat("Metallic Name", 1).c_str(), "Drag Metallic Texture here", const_cast<char*>(engine->gPBRManager->gMaterialEditorSettings->MetallicTexture.c_str()), 256, true, ImGuiInputTextFlags_None);
                engine->editorManager->DragAndDropInst_.StringPayloadTarget("png", engine->gPBRManager->gMaterialEditorSettings->MetallicTexture, "Metallic Texture Inserted.", PayloadTargetType::PTT_ASSETS);
                engine->gPBRManager->Clear(engine->gPBRManager->gMaterialEditorSettings->MetallicTexture, MaterialType::MT_METALLIC);


                ECGui::NextColumn();
                ECGui::DrawInputTextHintWidget(my_strcat("Roughness Name", 1).c_str(), "Drag Roughness Texture here", const_cast<char*>(engine->gPBRManager->gMaterialEditorSettings->RoughnessTexture.c_str()), 256, true, ImGuiInputTextFlags_None);
                engine->editorManager->DragAndDropInst_.StringPayloadTarget("png", engine->gPBRManager->gMaterialEditorSettings->RoughnessTexture, "Roughness Texture Inserted.", PayloadTargetType::PTT_ASSETS);
                engine->gPBRManager->Clear(engine->gPBRManager->gMaterialEditorSettings->RoughnessTexture, MaterialType::MT_ROUGHNESS);

                ECGui::NextColumn();
                ECGui::DrawInputTextHintWidget(my_strcat("AO Name", 1).c_str(), "Drag AO Texture here", const_cast<char*>(engine->gPBRManager->gMaterialEditorSettings->AoTexture.c_str()), 256, true, ImGuiInputTextFlags_None);
                engine->editorManager->DragAndDropInst_.StringPayloadTarget("png", engine->gPBRManager->gMaterialEditorSettings->AoTexture, "AO Texture Inserted.", PayloadTargetType::PTT_ASSETS);
                engine->gPBRManager->Clear(engine->gPBRManager->gMaterialEditorSettings->AoTexture, MaterialType::MT_AO);
                ECGui::NextColumn();
            }
            else
            {
                ECGui::DrawTextWidget<const char*>("AlbedoConstant", EMPTY_STRING);
                ECGui::NextColumn();
                ECGui::ColorPicker3("AlbedoConstant", (float*)&engine->gPBRManager->gMaterialEditorSettings->CurrentMaterial->AlbedoConstant,
                    ImGuiColorEditFlags_PickerHueWheel | ImGuiColorEditFlags_DisplayRGB);
                ECGui::DrawSliderFloat3Widget("AlbedoConstant", &engine->gPBRManager->gMaterialEditorSettings->CurrentMaterial->AlbedoConstant, true, 0.0f, 1.0f);

                ECGui::DrawTextWidget<const char*>("MetallicConstant", EMPTY_STRING);
                ECGui::NextColumn();
                ECGui::PushItemWidth(ECGui::GetWindowSize().x);
                ECGui::DrawSliderFloatWidget("MetallicConstant", &engine->gPBRManager->gMaterialEditorSettings->CurrentMaterial->MetallicConstant, true, 0.0f, 1.0f);

                ECGui::NextColumn();

                ECGui::DrawTextWidget<const char*>("RoughnessConstant", EMPTY_STRING);
                ECGui::NextColumn();
                ECGui::PushItemWidth(ECGui::GetWindowSize().x);
                ECGui::DrawSliderFloatWidget("RoughnessConstant", &engine->gPBRManager->gMaterialEditorSettings->CurrentMaterial->RoughnessConstant, true, 0.0f, 1.0f);

                ECGui::NextColumn();

                ECGui::DrawTextWidget<const char*>("AoConstant", EMPTY_STRING);
                ECGui::NextColumn();
                ECGui::PushItemWidth(ECGui::GetWindowSize().x);
                ECGui::DrawSliderFloatWidget("AoConstant", &engine->gPBRManager->gMaterialEditorSettings->CurrentMaterial->AoConstant, true, 0.0f, 1.0f);
            }
        }

        return false;
    }
}