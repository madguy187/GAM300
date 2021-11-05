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

            ImGui::Begin("Material BluePrint", &IsVisible);
            MaterialEditor.DrawMaterialNodeEditor("Material BluePrint", MaterialEditor);
            ImGui::End();
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
        IsVisible = false;
        engine->gPBRManager->gMaterialEditorSettings->ClearCurrentMaterial();
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

        if (ECGui::IsItemHovered())
        {
            OnCameraMoveEvent();
            OnCameraZoomEvent();
        }
    }

    void MaterialEditorWindow::OnCameraZoomEvent()
    {
        ImGuiIO& io = ImGui::GetIO();

        if (io.MouseWheel != 0.0f)
        {
            // ImGui Scroll Up Detection
            if (io.MouseWheel > 0.0f)
                engine->gCamera.GetMaterialInput().set(8, 1);
            else
                engine->gCamera.GetMaterialInput().set(8, 0);

            // ImGui Scroll Down Detection
            if (io.MouseWheel < 0.0f)
                engine->gCamera.GetMaterialInput().set(9, 1);
            else
                engine->gCamera.GetMaterialInput().set(9, 0);
        }
        else
        {
            engine->gCamera.GetMaterialInput().set(8, 0);
            engine->gCamera.GetMaterialInput().set(9, 0);
        }
    }

    void MaterialEditorWindow::OnCameraMoveEvent()
    {
        ImGuiIO& io = ImGui::GetIO();
        ImVec2 value_with_lock_threshold = ECGui::GetMouseDragDelta(1);
        const float benchmarkValue = 0.0f;

        // ImGui Right Click Detection
        if (ECGui::IsMouseDragging(1))
        {
            // Camera Yaw Right
            if (value_with_lock_threshold.x > benchmarkValue && io.MouseDelta.x > 0.0f)
                engine->gCamera.GetMaterialInput().set(7, 1);
            else
                engine->gCamera.GetMaterialInput().set(7, 0);

            // Camera Yaw Left
            if (value_with_lock_threshold.x < benchmarkValue && io.MouseDelta.x < 0.0f)
                engine->gCamera.GetMaterialInput().set(6, 1);
            else
                engine->gCamera.GetMaterialInput().set(6, 0);

            // Camera Pitch Down
            if (value_with_lock_threshold.y > benchmarkValue && io.MouseDelta.y > 0.0f)
                engine->gCamera.GetMaterialInput().set(5, 1);
            else
                engine->gCamera.GetMaterialInput().set(5, 0);

            // Camera Pitch Up
            if (value_with_lock_threshold.y < benchmarkValue && io.MouseDelta.y < 0.0f)
                engine->gCamera.GetMaterialInput().set(4, 1);
            else
                engine->gCamera.GetMaterialInput().set(4, 0);

            // Camera Move Front
            if (ECGui::IsKeyDown(ECGui::GetKeyIndex(ImGuiKey_W)))
                engine->gCamera.GetMaterialInput().set(2, 1);
            else
                engine->gCamera.GetMaterialInput().set(2, 0);

            // Camera Move Left
            if (ECGui::IsKeyDown(ECGui::GetKeyIndex(ImGuiKey_A)))
                engine->gCamera.GetMaterialInput().set(1, 1);
            else
                engine->gCamera.GetMaterialInput().set(1, 0);

            // Camera Move Back
            if (ECGui::IsKeyDown(ECGui::GetKeyIndex(ImGuiKey_S)))
                engine->gCamera.GetMaterialInput().set(3, 1);
            else
                engine->gCamera.GetMaterialInput().set(3, 0);

            // Camera Move Right
            if (ECGui::IsKeyDown(ECGui::GetKeyIndex(ImGuiKey_D)))
                engine->gCamera.GetMaterialInput().set(0, 1);
            else
                engine->gCamera.GetMaterialInput().set(0, 0);

            // Camera Move Up
            if (ECGui::IsKeyDown(ECGui::GetKeyIndex(ImGuiKey_Q)))
                engine->gCamera.GetMaterialInput().set(10, 1);
            else
                engine->gCamera.GetMaterialInput().set(10, 0);

            // Camera Move Down
            if (ECGui::IsKeyDown(ECGui::GetKeyIndex(ImGuiKey_E)))
                engine->gCamera.GetMaterialInput().set(11, 1);
            else
                engine->gCamera.GetMaterialInput().set(11, 0);
        }
        else
        {
            engine->gCamera.GetMaterialInput().set(0, 0);
            engine->gCamera.GetMaterialInput().set(1, 0);
            engine->gCamera.GetMaterialInput().set(2, 0);
            engine->gCamera.GetMaterialInput().set(3, 0);
            engine->gCamera.GetMaterialInput().set(4, 0);
            engine->gCamera.GetMaterialInput().set(5, 0);
            engine->gCamera.GetMaterialInput().set(6, 0);
            engine->gCamera.GetMaterialInput().set(7, 0);
            engine->gCamera.GetMaterialInput().set(10, 0);
            engine->gCamera.GetMaterialInput().set(11, 0);
        }
    }

    void MaterialEditorWindow::RunMaterialSettings()
    {
        if (IsVisible && ECGui::ButtonBool("Close Material Editor"))
        {
            Unload();
            engine->gPBRManager->gMaterialEditorSettings->SelectedIndex = 0;

            auto& cam = engine->world.GetComponent<TransformComponent>(engine->gCamera.GetCameraID(CameraComponent::CameraType::MaterialEditor_Camera));
            cam.position = ECVec3{ 0.0f, 0.0f, 30.0f };;
            cam.rotation = ECVec3{ 0.0f, -90.0f, 0.0f };
        }
        ImGui::Dummy(ImVec2(1, 10));

        static ImGuiTextFilter CompFilter;

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
        if (strcmp(MaterialName_.c_str(), "Default") != 0)
        {
            if (ECGui::ButtonBool("SetUp Blank Material", { ImGui::GetColumnWidth(), 25 }))
            {
                ColorPicker = ECVec3{ 1.0f };
                engine->gPBRManager->gMaterialEditorSettings->ClearCurrentMaterial();
                engine->gPBRManager->gMaterialEditorSettings->ClearTextureFields();
            }
        }

        if (engine->gPBRManager->AllMaterialInstances.find(MaterialName_) == engine->gPBRManager->AllMaterialInstances.end())
        {
            ImGui::Dummy(ImVec2(1, 2));

            if (strcmp(MaterialName_.c_str(), "Default") != 0)
            {
                if (ECGui::ButtonBool("Create Material", { ImGui::GetColumnWidth(), 25 }))
                {
                    engine->gPBRManager->gMaterialEditorSettings->CreateMaterialInstance();
                }
            }
        }

        ImGui::Dummy(ImVec2(1, 2));

        if (ECGui::ButtonBool("Clear Changes", { ImGui::GetColumnWidth(), 25 }))
        {
            ColorPicker = ECVec3{ 1.0f };
            engine->gPBRManager->gMaterialEditorSettings->ClearCurrentMaterial();
            engine->gPBRManager->gMaterialEditorSettings->ClearTextureFields();
        }
        ImGui::Dummy(ImVec2(1, 2));

        if (engine->gPBRManager->AllMaterialInstances.find(MaterialName_) != engine->gPBRManager->AllMaterialInstances.end())
        {
            ImGui::Dummy(ImVec2(1, 5));

            if (ECGui::ButtonBool("Delete Material", { ImGui::GetColumnWidth(), 25 }))
            {
                std::vector<std::string>::iterator it;
                for (it = engine->gPBRManager->AllMaterialInstName.begin(); it != engine->gPBRManager->AllMaterialInstName.end();)
                {
                    std::vector<std::string>::iterator curr = it++;

                    if (strcmp(curr->c_str(), MaterialName_.data()) == 0)
                    {
                        std::string path = "src/Assets/MaterialInstances/" + MaterialName_ + ".mat";
                        std::filesystem::remove(path);
                        engine->gPBRManager->AllMaterialInstances.erase(MaterialName_);
                        comboindex = 0;
                        engine->gPBRManager->gMaterialEditorSettings->SelectedIndex = 0;
                        engine->gPBRManager->AllMaterialInstName.erase(curr);
                        ColorPicker = ECVec3{ 1.0f };
                        engine->gPBRManager->gMaterialEditorSettings->ClearCurrentMaterial();
                        engine->gPBRManager->gMaterialEditorSettings->ClearTextureFields();
                        return;
                    }
                }
            }
        }

        if (strcmp(MaterialName_.c_str(), "Default") != 0)
        {
            if (ECGui::ButtonBool("Update Material", { ImGui::GetColumnWidth(), 25 }))
            {
                if (engine->gPBRManager->AllMaterialInstances.find(MaterialName_) != engine->gPBRManager->AllMaterialInstances.end())
                {
                    engine->gPBRManager->AllMaterialInstances[MaterialName_] = std::make_unique<MaterialInstance>(engine->gPBRManager->gMaterialEditorSettings->CurrentMaterial);
                }
            }
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
        //const auto& MaterialNames = engine->gPBRManager->AllMaterialInstName;
        //ComboListSettings settingsss = { "Current Material" };
        //ECGui::DrawTextWidget<const char*>("Current Materials:", EMPTY_STRING);
        //ECGui::CreateComboList(settingsss, MaterialNames, comboindex);
        //CheckCurrentMaterial(comboindex);

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
            ImGui::Dummy(ImVec2(1, 2));

            // If We going to assign texture to his material
            if (engine->gPBRManager->gMaterialEditorSettings->CurrentMaterial.HasTexture)
            {
                ECGui::DrawTextWidget<const char*>("Normal Map", EMPTY_STRING);
                ECGui::InsertSameLine();
                ECGui::CheckBoxBool("Normal Map", &engine->gPBRManager->gMaterialEditorSettings->CurrentMaterial.IsNormalMap);
                ECGui::NextColumn();
                ImGui::Dummy(ImVec2(1, 2));

                ECGui::DrawTextWidget<const char*>("Height Scale: ", EMPTY_STRING);
                ECGui::NextColumn();
                ECGui::PushItemWidth(ECGui::GetWindowSize().x);
                ECGui::DrawSliderFloatWidget("Height Scale", &engine->gPBRManager->gMaterialEditorSettings->CurrentMaterial.HeightScale, true, 0.f, 0.1f);
                ECGui::NextColumn();
                ImGui::Dummy(ImVec2(1, 2));

                ECGui::DrawInputTextHintWidget(my_strcat("Albdeo Name", 1).c_str(), "Drag Albdeo Texture here", const_cast<char*>(engine->gPBRManager->gMaterialEditorSettings->AlbedoTexture.c_str()), 256, true, ImGuiInputTextFlags_None);
                engine->editorManager->DragAndDropInst_.StringPayloadTarget("dds", engine->gPBRManager->gMaterialEditorSettings->AlbedoTexture, "Albdeo Texture Inserted.", PayloadTargetType::PTT_ASSETS);
                engine->gPBRManager->Clear(engine->gPBRManager->gMaterialEditorSettings->AlbedoTexture, MaterialType::MT_ALBEDO);

                ECGui::NextColumn();
                ECGui::DrawInputTextHintWidget(my_strcat("Normal Name", 1).c_str(), "Drag Normal Texture here", const_cast<char*>(engine->gPBRManager->gMaterialEditorSettings->NormalTexture.c_str()), 256, true, ImGuiInputTextFlags_None);
                engine->editorManager->DragAndDropInst_.StringPayloadTarget("dds", engine->gPBRManager->gMaterialEditorSettings->NormalTexture, "Normal Texture Inserted.", PayloadTargetType::PTT_ASSETS);
                engine->gPBRManager->Clear(engine->gPBRManager->gMaterialEditorSettings->NormalTexture, MaterialType::MT_NORMAL);

                ECGui::NextColumn();
                ECGui::DrawInputTextHintWidget(my_strcat("Metallic Name", 1).c_str(), "Drag Metallic Texture here", const_cast<char*>(engine->gPBRManager->gMaterialEditorSettings->MetallicTexture.c_str()), 256, true, ImGuiInputTextFlags_None);
                engine->editorManager->DragAndDropInst_.StringPayloadTarget("dds", engine->gPBRManager->gMaterialEditorSettings->MetallicTexture, "Metallic Texture Inserted.", PayloadTargetType::PTT_ASSETS);
                engine->gPBRManager->Clear(engine->gPBRManager->gMaterialEditorSettings->MetallicTexture, MaterialType::MT_METALLIC);


                ECGui::NextColumn();
                ECGui::DrawInputTextHintWidget(my_strcat("Roughness Name", 1).c_str(), "Drag Roughness Texture here", const_cast<char*>(engine->gPBRManager->gMaterialEditorSettings->RoughnessTexture.c_str()), 256, true, ImGuiInputTextFlags_None);
                engine->editorManager->DragAndDropInst_.StringPayloadTarget("dds", engine->gPBRManager->gMaterialEditorSettings->RoughnessTexture, "Roughness Texture Inserted.", PayloadTargetType::PTT_ASSETS);
                engine->gPBRManager->Clear(engine->gPBRManager->gMaterialEditorSettings->RoughnessTexture, MaterialType::MT_ROUGHNESS);

                ECGui::NextColumn();
                ECGui::DrawInputTextHintWidget(my_strcat("AO Name", 1).c_str(), "Drag AO Texture here", const_cast<char*>(engine->gPBRManager->gMaterialEditorSettings->AoTexture.c_str()), 256, true, ImGuiInputTextFlags_None);
                engine->editorManager->DragAndDropInst_.StringPayloadTarget("dds", engine->gPBRManager->gMaterialEditorSettings->AoTexture, "AO Texture Inserted.", PayloadTargetType::PTT_ASSETS);
                engine->gPBRManager->Clear(engine->gPBRManager->gMaterialEditorSettings->AoTexture, MaterialType::MT_AO);
                ECGui::NextColumn();

                ECGui::NextColumn();
                ECGui::DrawInputTextHintWidget(my_strcat("Height Name", 1).c_str(), "Drag Height Texture here", const_cast<char*>(engine->gPBRManager->gMaterialEditorSettings->HeightTexture.c_str()), 256, true, ImGuiInputTextFlags_None);
                engine->editorManager->DragAndDropInst_.StringPayloadTarget("dds", engine->gPBRManager->gMaterialEditorSettings->HeightTexture, "Height Texture Inserted.", PayloadTargetType::PTT_ASSETS);
                engine->gPBRManager->Clear(engine->gPBRManager->gMaterialEditorSettings->HeightTexture, MaterialType::MT_HEIGHT);
                ECGui::NextColumn();

                ImGui::Dummy(ImVec2(1, 5));

                ECGui::DrawTextWidget<const char*>("Surface Colour", EMPTY_STRING);
                ECGui::NextColumn();
                ECGui::ColorPicker3("Surface Colour", (float*)&engine->gPBRManager->gMaterialEditorSettings->CurrentMaterial.SurfaceColour, ImGuiColorEditFlags_DisplayRGB);

                ImGui::Dummy(ImVec2(1, 5));
            }
            else
            {
                ECGui::DrawTextWidget<const char*>("AlbedoConstant", EMPTY_STRING);
                ECGui::NextColumn();
                ECGui::ColorPicker3("AlbedoConstant", (float*)&engine->gPBRManager->gMaterialEditorSettings->CurrentMaterial.AlbedoConstant, ImGuiColorEditFlags_PickerHueWheel | ImGuiColorEditFlags_DisplayRGB);

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