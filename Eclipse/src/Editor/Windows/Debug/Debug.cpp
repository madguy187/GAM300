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
        WindowName = "Settings";
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
            ECGui::CheckBoxBool("Enable PostProcess", &engine->GraphicsManager.PostProcess->AllowPostProcess, false);
            ECGui::InsertSameLine();
            ECGui::CheckBoxBool("Visualize Normals", &engine->GraphicsManager.VisualizeNormalVectors, false);

            ECGui::DrawTextWidget<const char*>("Gamma:", EMPTY_STRING);
            ECGui::DrawSliderFloatWidget("Gamma", &engine->GraphicsManager.GammaCorrection, true, 0.5f, 2.5f);

            ECGui::DrawTextWidget<const char*>("BackGroundColour:", EMPTY_STRING);
            ECGui::DrawSliderFloat3Widget("BackGroundColour", &engine->GraphicsManager.BackGroundColour, true, 0.f, 1.0f);

            ECGui::DrawTextWidget<const char*>("HDR Exposure:", EMPTY_STRING);
            ECGui::DrawSliderFloatWidget("HDR Exposure", &engine->GraphicsManager.Exposure, true, 0.1f, 10.5f);

            if (engine->GraphicsManager.PostProcess->AllowPostProcess)
            {
                std::vector<std::string> Methods = { "NONE" , "INVERSE" , "GREYSCALE" ,"KERNEL" , "BLUR"};
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
        }
    }
}
