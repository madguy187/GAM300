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

            ECGui::DrawTextWidget<const char*>("Draw Grid", EMPTY_STRING);
            ECGui::InsertSameLine();
            ECGui::CheckBoxBool("DrawGrid", &engine->GridManager->Visible);
            ECGui::InsertSameLine();
            ECGui::DrawTextWidget<const char*>("Enable PostProcess", EMPTY_STRING);
            ECGui::InsertSameLine();
            ECGui::CheckBoxBool("Enable PostProcess", &engine->GraphicsManager.PostProcess->AllowPostProcess);

            ECGui::DrawTextWidget<const char*>("Gamma", EMPTY_STRING);
            ECGui::InsertSameLine();
            ECGui::DrawSliderFloatWidget("Gamma", &engine->GraphicsManager.GammaCorrection, true, 0.5f, 2.5f);
            ECGui::InsertSameLine();
            ECGui::DrawTextWidget<const char*>("BackGroundColour", EMPTY_STRING);
            ECGui::InsertSameLine();
            ECGui::DrawSliderFloat3Widget("BackGroundColour", &engine->GraphicsManager.BackGroundColour, true, 0.f, 1.0f);

            ECGui::DrawTextWidget<const char*>("HDR Exposure", EMPTY_STRING);
            ECGui::InsertSameLine();
            ECGui::DrawSliderFloatWidget("HDR Exposure", &engine->GraphicsManager.Exposure, true, 0.1f, 10.5f);

            ECGui::InsertSameLine();
            if (engine->GraphicsManager.PostProcess->AllowPostProcess)
            {
                ECGui::InsertSameLine();
                std::vector<std::string> Methods = { "NONE" , "INVERSE" , "GREYSCALE" ,"KERNEL" , "BLUR"};
                ComboListSettings settingsss = { "PostProcess Methods" };
                static size_t comboindex = 0;
                ECGui::CreateComboList(settingsss, Methods, comboindex);
                engine->GraphicsManager.PostProcess->PPType_ = static_cast<FrameBuffer::PostProcessType>(comboindex);
            }
        }
    }
}
