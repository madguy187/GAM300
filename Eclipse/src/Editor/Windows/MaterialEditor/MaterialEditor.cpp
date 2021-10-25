#include "pch.h"
#include "MaterialEditor.h"

namespace Eclipse
{
    void MaterialEditorWindow::Update()
    {
        if (IsVisible == false)
        {
            ColorPicker = ECVec3{ 1.0f };
            engine->gPBRManager->gMaterialEditorSettings->ClearCurrentMaterial();
        }

        if (IsVisible)
        {
            ECGui::DrawMainWindow<void()>(WindowName, std::bind(&MaterialEditorWindow::RunMainWindow, this));
            ECGui::DrawMainWindow<void()>("Material Settings", std::bind(&MaterialEditorWindow::RunMaterialSettings, this));
        }
    }

    void MaterialEditorWindow::Init()
    {
        Type = EditorWindowType::EWT_MATERIALEDITOR;
        WindowName = "Material Editor";
        m_frameBuffer = engine->gFrameBufferManager->GetFramebuffer(FrameBufferMode::FBM_MATERIALEDITOR);
        IsVisible = false;
    }

    void MaterialEditorWindow::Unload()
    {
    }

    void MaterialEditorWindow::RunMainWindow()
    {
        ImVec2 viewportPanelSize = ECGui::GetWindowSize();

        if (mViewportSize.getX() != viewportPanelSize.x ||
            mViewportSize.getY() != viewportPanelSize.y)
        {
            m_frameBuffer->Resize(static_cast<unsigned>(viewportPanelSize.x),
                static_cast<unsigned>(viewportPanelSize.y), FrameBufferMode::FBM_MATERIALEDITOR);
            mViewportSize = { viewportPanelSize.x, viewportPanelSize.y };
            engine->gFrameBufferManager->UpdateAspectRatio(FrameBufferMode::FBM_MATERIALEDITOR, mViewportSize);
        }

        ChildSettings settings;
        settings.Name = "MaterialEditorFrameBuffer";
        settings.Size = ImVec2{ mViewportSize.x, mViewportSize.y };
        ECGui::DrawChildWindow<void()>(settings, std::bind(&MaterialEditorWindow::RunFrameBuffer, this));
    }

    void MaterialEditorWindow::RunFrameBuffer()
    {
        ECGui::Image((void*)(static_cast<size_t>(m_frameBuffer->GetTextureColourBufferID())),
            ImVec2{ mViewportSize.x, mViewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
    }

    void MaterialEditorWindow::RunMaterialSettings()
    {
        static ImGuiTextFilter CompFilter;
        CompFilter.Draw();

        ShowMaterialProperty("Material", CompFilter);
        ShowTransformProperty("Transform", CompFilter);
        Buttons();

    }

    void MaterialEditorWindow::CheckCurrentMaterial(size_t comboIndex)
    {
        if (engine->gPBRManager->gMaterialEditorSettings->SelectedIndex != comboIndex)
        {
            engine->gPBRManager->gMaterialEditorSettings->SelectedIndex = comboIndex;

            if (comboIndex == 0)
            {
                engine->gPBRManager->gMaterialEditorSettings->ClearCurrentMaterial();
            }
            else
            {
                engine->gPBRManager->gMaterialEditorSettings->CurrentMaterial =
                    *engine->gPBRManager->AllMaterialInstances[engine->gPBRManager->AllMaterialInstName[engine->gPBRManager->gMaterialEditorSettings->SelectedIndex]];
            }
        }
    }

    void MaterialEditorWindow::Buttons()
    {
        std::string MaterialName_ = engine->gPBRManager->gMaterialEditorSettings->CurrentMaterial.Name.data();

        ImGui::Dummy(ImVec2(1, 5));

        if (ECGui::ButtonBool("Update Material", { ImGui::GetColumnWidth(), 25 }))
        {
            if (engine->gPBRManager->AllMaterialInstances.find(MaterialName_) != engine->gPBRManager->AllMaterialInstances.end())
            {
                engine->gPBRManager->AllMaterialInstances[MaterialName_] = std::make_unique<MaterialInstance>(engine->gPBRManager->gMaterialEditorSettings->CurrentMaterial);
            }
        }

        if (engine->gPBRManager->AllMaterialInstances.find(MaterialName_) == engine->gPBRManager->AllMaterialInstances.end())
        {
            ImGui::Dummy(ImVec2(1, 5));

            if (ECGui::ButtonBool("Create Material", { ImGui::GetColumnWidth(), 25 }))
            {
                engine->gPBRManager->gMaterialEditorSettings->CreateMaterialInstance();
            }
        }

        ImGui::Dummy(ImVec2(1, 2));

        if (ECGui::ButtonBool("Clear Changes", { ImGui::GetColumnWidth(), 25 }))
        {
            ColorPicker = ECVec3{ 1.0f };
            engine->gPBRManager->gMaterialEditorSettings->ClearCurrentMaterial();
        }
    }

    bool MaterialEditorWindow::ShowTransformProperty(const char* name, ImGuiTextFilter& filter)
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

    bool MaterialEditorWindow::ShowMaterialProperty(const char* name, ImGuiTextFilter& filter)
    {
        const auto& MaterialNames = engine->gPBRManager->AllMaterialInstName;
        ComboListSettings settingsss = { "Current Material" };
        static size_t comboindex = 0;
        ECGui::DrawTextWidget<const char*>("Current Materials:", EMPTY_STRING);
        ECGui::CreateComboList(settingsss, MaterialNames, comboindex);
        CheckCurrentMaterial(comboindex);

        ECGui::NextColumn();
        ImGui::Dummy(ImVec2(1, 5));

        ECGui::DrawTextWidget<const char*>("Material Name:", EMPTY_STRING);
        ECGui::DrawInputTextHintWidget(my_strcat("Material Name", 1).c_str(), "Material Name", const_cast<char*>(engine->gPBRManager->gMaterialEditorSettings->CurrentMaterial.Name.data()), 256, true, ImGuiInputTextFlags_None);
        ECGui::NextColumn();
        ImGui::Dummy(ImVec2(1, 5));

        ECGui::DrawTextWidget<const char*>("Base Reflectivity:", EMPTY_STRING);
        ECGui::DrawSliderFloat3Widget("BaseRFeflectivity", &engine->gPBRManager->gMaterialEditorSettings->CurrentMaterial.BaseReflectivity, true, 0.0f, 1.0f);
        ImGui::Dummy(ImVec2(1, 2));

        if (ECGui::ButtonBool("Reset BaseReflectivity", { ImGui::GetColumnWidth(), 25 }))
        {
            engine->gPBRManager->gMaterialEditorSettings->CurrentMaterial.BaseReflectivity = ECVec3{ 0.4f };
        }
        ImGui::Dummy(ImVec2(1, 5));
        ECGui::NextColumn();

        if (filter.PassFilter(name) && ECGui::CreateCollapsingHeader(name))
        {
            ECGui::DrawTextWidget<const char*>("HasTexture", EMPTY_STRING);
            ECGui::InsertSameLine();
            ECGui::CheckBoxBool("HasTexture", &engine->gPBRManager->gMaterialEditorSettings->CurrentMaterial.HasTexture);
            ECGui::NextColumn();

            // If We going to assign texture to his material
            if (engine->gPBRManager->gMaterialEditorSettings->CurrentMaterial.HasTexture)
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
                ImGui::Dummy(ImVec2(1, 5));
            }
            else
            {
                ECGui::DrawTextWidget<const char*>("AlbedoConstant", EMPTY_STRING);
                ECGui::NextColumn();
                ECGui::ColorPicker3("AlbedoConstant", (float*)&engine->gPBRManager->gMaterialEditorSettings->CurrentMaterial.AlbedoConstant, ImGuiColorEditFlags_PickerHueWheel | ImGuiColorEditFlags_DisplayRGB);

                //engine->gPBRManager->gMaterialEditorSettings->CurrentMaterial.AlbedoConstant.setX(ColorPicker.getX());
                //engine->gPBRManager->gMaterialEditorSettings->CurrentMaterial.AlbedoConstant.setY(ColorPicker.getY());
                //engine->gPBRManager->gMaterialEditorSettings->CurrentMaterial.AlbedoConstant.setZ(ColorPicker.getZ());
                //ECGui::DrawSliderFloat3Widget("AlbedoConstant", &engine->gPBRManager->gMaterialEditorSettings->CurrentMaterial.AlbedoConstant, true, 0.0f, 1.0f);

                ECGui::DrawTextWidget<const char*>("MetallicConstant", EMPTY_STRING);
                ECGui::PushItemWidth(ECGui::GetWindowSize().x);
                ECGui::DrawSliderFloatWidget("MetallicConstant", &engine->gPBRManager->gMaterialEditorSettings->CurrentMaterial.MetallicConstant, true, 0.0f, 1.0f);

                ECGui::NextColumn();

                ECGui::DrawTextWidget<const char*>("RoughnessConstant", EMPTY_STRING);
                ECGui::NextColumn();
                ECGui::DrawSliderFloatWidget("RoughnessConstant", &engine->gPBRManager->gMaterialEditorSettings->CurrentMaterial.RoughnessConstant, true, 0.0f, 1.0f);

                ECGui::NextColumn();

                ECGui::DrawTextWidget<const char*>("AoConstant", EMPTY_STRING);
                ECGui::PushItemWidth(ECGui::GetWindowSize().x);
                ECGui::DrawSliderFloatWidget("AoConstant", &engine->gPBRManager->gMaterialEditorSettings->CurrentMaterial.AoConstant, true, 0.0f, 1.0f);
                ImGui::Dummy(ImVec2(1, 5));
            }
        }

        return false;
    }
}