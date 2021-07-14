#include "pch.h"
#include "Inspector.h"
#include "ECS/ComponentManager/Components/EntityComponent.h"
#include "ECS/ComponentManager/Components/TransformComponent.h"

namespace Eclipse
{
    void InspectorWindow::Update()
    {
        if (IsVisible)
            ECGui::DrawMainWindow<void()>(WindowName, std::bind(&InspectorWindow::DrawImpl, this));
    }

    InspectorWindow::InspectorWindow()
    {
        Type = EditorWindowType::EWT_INSPECTOR;
        WindowName = "Inspector";
    }

    void InspectorWindow::DrawImpl()
    {
        WindowSize_.setX(ECGui::GetWindowSize().x);
        WindowSize_.setY(ECGui::GetWindowSize().y);

        if (!engine->editorManager->EntityHierarchyList_.empty())
        {
            Entity currEnt = engine->editorManager->EntityHierarchyList_[engine->editorManager->GEHIndex_];
            auto& entcom = engine->world.GetComponent<EntityComponent>(currEnt);
            std::string entityName = entcom.Name + " " + std::to_string(currEnt);

            ECGui::DrawInputTextWidget("EntityName", const_cast<char*>(entityName.c_str()),
                entityName.size(), ImGuiInputTextFlags_ReadOnly);

            ECGui::InsertHorizontalLineSeperator();
            ECGui::PushItemWidth(WindowSize_.getX());

            ShowEntityProperty(currEnt);
            ShowTransformProperty(currEnt);
            ShowPointLightProperty(currEnt);
            ShowSpotLightProperty(currEnt);
            ShowDirectionalLightProperty(currEnt);
            ShowGridSettings(currEnt);
            SetGamma();
        }
        else
        {
            const char* entName = "No Entities";

            ECGui::DrawInputTextWidget("EntityName", const_cast<char*>(entName),
                strlen(entName), ImGuiInputTextFlags_ReadOnly);
        }

        ECGui::InsertHorizontalLineSeperator();
    }

    bool InspectorWindow::ShowEntityProperty(Entity ID)
    {
        if (engine->world.CheckComponent<EntityComponent>(ID))
        {
            if (ECGui::CreateCollapsingHeader("Tag"))
            {
                static char entNameInput[256];
                auto& entCom = engine->world.GetComponent<EntityComponent>(ID);

                ECGui::DrawTextWidget<std::string>("Entity Tag", lexical_cast<std::string>(entCom.Tag));
                ECGui::DrawTextWidget<const char*>("Edit Name:", "");
                ECGui::InsertSameLine();
                ECGui::DrawInputTextHintWidget("InputEntityName", "Enter Entity Name", entNameInput, 256);

                if (ECGui::ButtonBool("Set Name"))
                    entCom.Name = entNameInput;
            }
        }

        return false;
    }

    bool InspectorWindow::ShowTransformProperty(Entity ID)
    {
        if (engine->world.CheckComponent<TransformComponent>(ID))
        {
            if (ID == engine->GridMap.GetGridID())
                return false;

            if (ECGui::CreateCollapsingHeader("Transform"))
            {
                auto& transCom = engine->world.GetComponent<TransformComponent>(ID);

                ECGui::DrawTextWidget<const char*>("Position", "");
                ECGui::DrawSliderFloat3Widget("TransVec", &transCom.position, true, -50.f, 50.f);

                ECGui::DrawTextWidget<const char*>("Rotation", "");
                ECGui::DrawSliderFloat3Widget("TransRot", &transCom.rotation, true, -360.f, 360.f);

                ECGui::DrawTextWidget<const char*>("Scale", "");
                ECGui::DrawSliderFloat3Widget("TransScale", &transCom.scale);
            }
        }

        return false;
    }

    bool InspectorWindow::ShowPointLightProperty(Entity ID)
    {
        if (engine->world.CheckComponent<PointLightComponent>(ID))
        {
            if (ECGui::CreateCollapsingHeader("PointLight Component"))
            {
                auto& _PointLight = engine->world.GetComponent<PointLightComponent>(ID);

                static bool enable = true;
                static bool isVisible = false;
                ECGui::CheckBoxBool("Enable", &_PointLight.EnableBlinnPhong, enable);
                ECGui::CheckBoxBool("IsVisible", &_PointLight.visible, isVisible);

                ECGui::DrawTextWidget<const char*>("IntensityStrength", "");
                ECGui::DrawSliderFloatWidget("IntensityFloat", &_PointLight.IntensityStrength, true, 0.f, 150.f);

                ECGui::DrawTextWidget<const char*>("Light Colour", "");
                ImGui::ColorPicker3("Color", (float*)&_PointLight.Color, ImGuiColorEditFlags_PickerHueWheel | ImGuiColorEditFlags_DisplayRGB);
                //ECGui::DrawSliderFloat4Widget("ColourVec", &_PointLight.Color, true, 0.0f, 1.0f);
                engine->LightManager.SetLightColor(_PointLight, { _PointLight.Color.getX() ,_PointLight.Color.getY() , _PointLight.Color.getZ() , 1.0f });

                ECGui::DrawTextWidget<const char*>("Attenuation Level", "");
                ECGui::DrawSliderIntWidget("ColourVec", &_PointLight.AttenuationLevel, true, 0, 10);
                engine->LightManager.SetAttenuation(_PointLight, _PointLight.AttenuationLevel);

                ECGui::DrawTextWidget<const char*>("Light Ambient", "");
                ECGui::DrawSliderFloat3Widget("AmbientVec", &_PointLight.ambient, true, 0.0f, 1.0f);

                ECGui::DrawTextWidget<const char*>("Light Diffuse", "");
                ECGui::DrawSliderFloat3Widget("DiffuseVec", &_PointLight.diffuse, true, 0.0f, 1.0f);

                ECGui::DrawTextWidget<const char*>("Light Specular", "");
                ECGui::DrawSliderFloat3Widget("SpecularVec", &_PointLight.specular, true, 0.0f, 1.0f);
            }
        }

        return false;
    }

    bool InspectorWindow::ShowSpotLightProperty(Entity ID)
    {
        if (engine->world.CheckComponent<SpotLightComponent>(ID))
        {
            if (ECGui::CreateCollapsingHeader("SpotLight Component"))
            {
                auto& _SpotLight = engine->world.GetComponent<SpotLightComponent>(ID);

                static bool enable = true;
                static bool isVisible = false;
                ECGui::CheckBoxBool("Enable", &_SpotLight.EnableBlinnPhong, enable);
                ECGui::CheckBoxBool("IsVisible", &_SpotLight.visible, isVisible);

                ECGui::DrawTextWidget<const char*>("IntensityStrength", "");
                ECGui::DrawSliderFloatWidget("IntensityFloat", &_SpotLight.IntensityStrength, true, 0.f, 150.f);

                ECGui::DrawTextWidget<const char*>("Light Colour", "");
                ImGui::ColorPicker3("Color", (float*)&_SpotLight.lightColor, ImGuiColorEditFlags_PickerHueWheel | ImGuiColorEditFlags_DisplayRGB);

                ECGui::DrawTextWidget<const char*>("OuterCutOff", "");
                ECGui::DrawSliderFloatWidget("OuterCutOffFloat", &_SpotLight.outerCutOff, true, 0.f, 50.0f);

                ECGui::DrawTextWidget<const char*>("CutOff", "");
                ECGui::DrawSliderFloatWidget("CutOffFloat", &_SpotLight.cutOff, true, 0.f, (_SpotLight.outerCutOff - 5.0f));

                ECGui::DrawTextWidget<const char*>("Direction", "");
                ECGui::DrawSliderFloat3Widget("DirectionVec", &_SpotLight.direction, true, 0.f, 150.f);

                ECGui::DrawTextWidget<const char*>("Attenuation Level", "");
                ECGui::DrawSliderIntWidget("ColourVec", &_SpotLight.AttenuationLevel, true, 0, 10);
                engine->LightManager.SetAttenuation(_SpotLight, _SpotLight.AttenuationLevel);

                ECGui::DrawTextWidget<const char*>("Light Ambient", "");
                ECGui::DrawSliderFloat3Widget("AmbientVec", &_SpotLight.ambient, true, 0.0f, 1.0f);

                ECGui::DrawTextWidget<const char*>("Light Diffuse", "");
                ECGui::DrawSliderFloat3Widget("DiffuseVec", &_SpotLight.diffuse, true, 0.0f, 1.0f);

                ECGui::DrawTextWidget<const char*>("Light Specular", "");
                ECGui::DrawSliderFloat3Widget("SpecularVec", &_SpotLight.specular, true, 0.0f, 1.0f);

                ECGui::DrawTextWidget<const char*>("Surrounding Attenuation Level", "");
                ECGui::DrawSliderFloatWidget("SurroundIntensity", &_SpotLight.SurroundingAttenuationLevel, true, 0.f,
                    ((_SpotLight.ambient.getX() + _SpotLight.ambient.getY() + _SpotLight.ambient.getZ()) / 3));
            }
        }

        return false;
    }

    bool InspectorWindow::ShowDirectionalLightProperty(Entity ID)
    {
        if (engine->world.CheckComponent<DirectionalLightComponent>(ID))
        {
            if (ECGui::CreateCollapsingHeader("DirectionalLightComponent"))
            {
                auto& _DLight = engine->world.GetComponent<DirectionalLightComponent>(ID);

                static bool enable = true;
                ECGui::CheckBoxBool("Enable", &_DLight.EnableBlinnPhong, enable);

                ECGui::DrawTextWidget<const char*>("Light Colour", "");
                ImGui::ColorPicker3("Color", (float*)&_DLight.lightColor, ImGuiColorEditFlags_PickerHueWheel | ImGuiColorEditFlags_DisplayRGB);

                ECGui::DrawTextWidget<const char*>("DLight Ambient", "");
                ECGui::DrawSliderFloat3Widget("AmbientVec", &_DLight.ambient, true, 0.0f, 1.0f);

                ECGui::DrawTextWidget<const char*>("DLight Diffuse", "");
                ECGui::DrawSliderFloat3Widget("DiffuseVec", &_DLight.diffuse, true, 0.0f, 1.0f);

                ECGui::DrawTextWidget<const char*>("DLight Specular", "");
                ECGui::DrawSliderFloat3Widget("SpecularVec", &_DLight.specular, true, 0.0f, 1.0f);
            }
        }

        return false;
    }

    void InspectorWindow::SetGamma()
    {
        static float Test = 2.2f;
        ECGui::DrawTextWidget<const char*>("Gamma Correction Value", "");
        ECGui::DrawSliderFloatWidget("Gamma", &Test, true, 0.f, 3.0f);
        engine->GraphicsManager.SetGammaCorrection(Test);
    }

    bool InspectorWindow::ShowGridSettings(Entity ID)
    {
        if (engine->GridMap.GetModelReference() == nullptr)
            return false;

        if (engine->world.CheckComponent<TransformComponent>(ID))
        {
            if (ECGui::CreateCollapsingHeader("Grid Settings"))
            {
                static bool enable = true;
                static bool show = true;
                ECGui::DrawTextWidget<const char*>("Show Grid", "");
                ECGui::CheckBoxBool("Visible", &show, enable);
                engine->GridMap.SetGridToShow(show);

                ECGui::DrawTextWidget<const char*>("Grid Colour", "");
                ECGui::DrawSliderFloat3Widget("Colour", &engine->GridMap.GridColour, true, 0.0f, 1.0f);

                ECGui::DrawTextWidget<const char*>("Grid Scale", "");
                ECGui::DrawSliderFloatWidget("Scale", &engine->GridMap.GridScale, true, 0.0f, 20.0f);
            }
        }

        return false;
    }
}
