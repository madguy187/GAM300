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
        WindowName = "Debug";
        IsVisible = false;
    }

    void DebugWindow::Unload()
    {
    }

    void DebugWindow::DrawImpl()
    {
        auto* scene = engine->editorManager->GetEditorWindow<SceneWindow>();
        ECGui::PushItemWidth(ECGui::GetWindowSize().x / 3.5f);

        ECGui::DrawTextWidget<const char*>("Snap Settings:", "");
        {
            ECGui::DrawTextWidget<const char*>("Pos", "");
            ECGui::InsertSameLine();
            ECGui::DrawInputFloatWidget("PosSnap", &scene->GetRefToSnapSettings().mPosSnapValue, true, 0.5f);
            ECGui::InsertSameLine();

            ECGui::DrawTextWidget<const char*>("Scale", "");
            ECGui::InsertSameLine();
            ECGui::DrawInputFloatWidget("ScaleSnap", &scene->GetRefToSnapSettings().mScaleSnapValue, true, 0.5f);
            ECGui::InsertSameLine();

            ECGui::DrawTextWidget<const char*>("Rot", "");
            ECGui::InsertSameLine();
            ECGui::DrawInputFloatWidget("RotSnap", &scene->GetRefToSnapSettings().mRotSnapValue, true, 45.f);
        }

        if (engine->gCamera.GetEditorCameraID() != MAX_ENTITY)
        {
            auto& camCom = engine->world.GetComponent<CameraComponent>(engine->gCamera.GetEditorCameraID());

            ECGui::DrawTextWidget<const char*>("Camera Speed:", "");
            {
                ECGui::DrawTextWidget<const char*>("Speed", "");
                ECGui::InsertSameLine();
                ECGui::DrawSliderFloatWidget("CamSpeed", &camCom.cameraSpeed, true, 1.f, 200.f);
            }

            ImGui::Dummy({ 5,5 });
        }

        ECGui::DrawTextWidget<const char*>("Render Settings:", "");
        {

            ECGui::DrawTextWidget<const char*>("Draw Grid", "");
            ECGui::InsertSameLine();
            ECGui::CheckBoxBool("DrawGrid", &engine->GridManager->Visible);
            ECGui::InsertSameLine();
            ECGui::DrawTextWidget<const char*>("Enable PostProcess", "");
            ECGui::InsertSameLine();
            ECGui::CheckBoxBool("Enable PostProcess", &engine->GraphicsManager.PostProcess->AllowPostProcess);

            ECGui::DrawTextWidget<const char*>("Gamma", "");
            ECGui::InsertSameLine();
            ECGui::DrawSliderFloatWidget("Gamma", &engine->GraphicsManager.GammaCorrection, true, 0.5f, 2.5f);
            ECGui::InsertSameLine();
            ECGui::DrawTextWidget<const char*>("BackGroundColour", "");
            ECGui::InsertSameLine();
            ECGui::DrawSliderFloat3Widget("BackGroundColour", &engine->GraphicsManager.BackGroundColour, true, 0.f, 1.0f);

            ECGui::DrawTextWidget<const char*>("HDR Exposure", "");
            ECGui::InsertSameLine();
            ECGui::DrawSliderFloatWidget("HDR Exposure", &engine->GraphicsManager.Exposure, true, 0.1f, 10.5f);

            ECGui::InsertSameLine();
            if (engine->GraphicsManager.PostProcess->AllowPostProcess)
            {
                ECGui::InsertSameLine();
                std::vector<std::string> Methods = { "NONE" , "INVERSE" , "GREYSCALE" ,"PPT_KERNEL" };
                ComboListSettings settingsss = { "PostProcess Methods" };
                static size_t comboindex = 0;
                ECGui::CreateComboList(settingsss, Methods, comboindex);
                engine->GraphicsManager.PostProcess->PPType_ = static_cast<FrameBuffer::PostProcessType>(comboindex);
            }
        }
    }
}
