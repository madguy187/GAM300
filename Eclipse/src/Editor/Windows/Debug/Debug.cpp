#include "pch.h"
#include "Debug.h"
#include "../Scene/SceneView.h"

namespace Eclipse
{
    void DebugWindow::Update()
    {
        if (IsVisible)
            ECGui::DrawMainWindow<void()>(WindowName, std::bind(&DebugWindow::DrawImpl, this));
    }

    void DebugWindow::Init()
    {
        Type = EditorWindowType::EWT_DEBUG;
        WindowName = "Settings " ICON_MDI_ACCOUNT_COG;
        IsVisible = false;
    }

    void DebugWindow::Unload()
    {
    }

    void DebugWindow::DrawImpl()
    {
        ECGui::DrawTextWidget<const char*>("Render Settings:", EMPTY_STRING);
        {

            ECGui::CheckBoxBool("Draw Grid", &engine->GridManager->Visible, false);
            ECGui::InsertSameLine();
            ECGui::CheckBoxBool("PostProcess", &engine->GraphicsManager.PostProcess->AllowPostProcess, false);
            ECGui::InsertSameLine();
            ECGui::CheckBoxBool("Draw Normals", &engine->GraphicsManager.VisualizeNormalVectors, false);
            ECGui::InsertSameLine();
            ECGui::CheckBoxBool("Draw DebugBoxes", &engine->GraphicsManager.AllAABBs.DrawAABBS, false);
            ECGui::InsertSameLine();
            ECGui::CheckBoxBool("Draw Frustrum", &engine->gDebugManager.Visible, false);
            ECGui::InsertSameLine();
            ECGui::CheckBoxBool("Draw Sky", &engine->GraphicsManager.DrawSky, false);
            ECGui::InsertSameLine();
            ECGui::CheckBoxBool("Normal Mapx", &engine->GraphicsManager.EnableNormalMapping, false);
            ECGui::InsertSameLine();
            ECGui::CheckBoxBool("Environment Maps", &engine->GraphicsManager.EnableEnvironmentMapForAll, false);

            ECGui::DrawTextWidget<const char*>("Gamma:", EMPTY_STRING);
            ECGui::DrawSliderFloatWidget("Gamma", &engine->GraphicsManager.GammaCorrection, true, 0.5f, 2.5f);

            ECGui::DrawTextWidget<const char*>("BackGroundColour:", EMPTY_STRING);
            ECGui::DrawSliderFloat3Widget("BackGroundColour", &engine->GraphicsManager.BackGroundColour, true, 0.f, 1.0f);

            ECGui::DrawTextWidget<const char*>("HDR Exposure:", EMPTY_STRING);
            ECGui::DrawSliderFloatWidget("HDR Exposure", &engine->GraphicsManager.Exposure, true, 0.1f, 10.5f);

            if (engine->GraphicsManager.PostProcess->AllowPostProcess)
            {
                std::vector<std::string> Methods = { "NONE" , "INVERSE" , "GREYSCALE" ,"KERNEL" , "BLUR" };
                ComboListSettings settingsss = { "PostProcess Methods" };
                static size_t comboindex = 0;
                ECGui::DrawTextWidget<const char*>("PostProcess Types:", EMPTY_STRING);
                ECGui::CreateComboList(settingsss, Methods, comboindex);
                engine->GraphicsManager.PostProcess->PPType_ = static_cast<FrameBuffer::PostProcessType>(comboindex);
            }

            if (engine->GraphicsManager.VisualizeNormalVectors)
            {
                ECGui::DrawTextWidget<const char*>("Normals Length:", EMPTY_STRING);
                ECGui::DrawSliderFloatWidget("Normals Length", &engine->GraphicsManager.Magnitude, true, 0.2f, 1.0f);
            }

            // INPUT MANAGER
            //std::vector<std::string> Methods = engine->InputManager->GetAllKeys();

            //ECGui::DrawTextWidget<const char*>(my_strcat("List of Inputs (", engine->InputManager->KeyMappings1.size(), "):").c_str(), "");

            //ComboListSettings settingsss = { "Input Keys" };
            //static size_t comboindex = 0;
            //ECGui::DrawTextWidget<const char*>("Input Keys:", EMPTY_STRING);
            //ECGui::CreateComboList(settingsss, Methods, comboindex);

            //for (size_t i = 0; i < engine->InputManager->KeyMappings1.size(); ++i)
            //{
            //    ECGui::DrawInputTextHintWidget(my_strcat("Input Name", i + 1).c_str(), "Input",const_cast<char*>(engine->InputManager->KeyMappings1[i].c_str()), 256, true, ImGuiInputTextFlags_None);
            //}

            //if (ECGui::ButtonBool("Add Input", { ImGui::GetColumnWidth(), 25 }))
            //{
            //    std::string InputName;
            //    InputName.reserve(256);
            //    engine->InputManager->KeyMappings1.push_back(InputName);
            //}
        }
    }
}
