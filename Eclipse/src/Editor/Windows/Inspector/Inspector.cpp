#include "pch.h"
#include "Inspector.h"
#include "ECS/ComponentManager/Components/EntityComponent.h"
#include "ECS/ComponentManager/Components/TransformComponent.h"
#include "ECS/ComponentManager/Components/RigidBodyComponent.h"
#include "ECS/ComponentManager/Components/ScriptComponent.h"
#include "Editor/Windows/SwitchViews/TopSwitchViewWindow.h"
#include "Editor/Windows/MeshEditor/MeshEditor.h"
#include "ECS/ComponentManager/Components/ParentComponent.h"
#include "ECS/ComponentManager/Components/ChildComponent.h"
#include "ECS/SystemManager/Systems/System/Collision/CollisionSystem.h"
#include "ECS/SystemManager/Systems/System/MonoSystem/MonoSystem.h"

namespace Eclipse
{
    void InspectorWindow::Update()
    {
        if (IsVisible)
            ECGui::DrawMainWindow<void()>(WindowName, std::bind(&InspectorWindow::DrawImpl, this));
    }

    void InspectorWindow::Init()
    {
        Type = EditorWindowType::EWT_INSPECTOR;
        WindowName = "Inspector " ICON_MDI_MAGNIFY_SCAN;
    }

    void InspectorWindow::Unload()
    {
    }

    void InspectorWindow::DrawImpl()
    {
        WindowSize_.setX(ECGui::GetWindowSize().x);
        WindowSize_.setY(ECGui::GetWindowSize().y);

        if (!engine->editorManager->IsEntityListEmpty())
        {
            auto* mesheditor = engine->editorManager->GetEditorWindow<MeshEditorWindow>();
            Entity currEnt = mesheditor->IsVisible ? mesheditor->GetMeshID() : engine->editorManager->GetSelectedEntity();
            auto& entcom = engine->world.GetComponent<EntityComponent>(currEnt);

            ECGui::PushItemWidth(WindowSize_.getX());
            if (ECGui::DrawInputTextHintWidget("InputEntityName", "Enter Entity Name", EntNameInput,
                256, true, ImGuiInputTextFlags_EnterReturnsTrue))
            {
                std::string oldName = entcom.Name;
                entcom.Name = EntNameInput;
                CommandHistory::RegisterCommand(new PrimitiveDeltaCommand<std::string>{ oldName, entcom.Name });
            }

            ECGui::PushItemWidth(WindowSize_.getX() * 0.4f);
            ECGui::DrawTextWidget<const char*>("Tag ", EMPTY_STRING);
            ECGui::InsertSameLine();
            ECGui::DrawInputTextWidget("Tag", const_cast<char*>(lexical_cast_toStr(entcom.Tag).c_str()),
                lexical_cast_toStr(entcom.Tag).size(), ImGuiInputTextFlags_ReadOnly, true);

            ECGui::InsertSameLine();

            ECGui::DrawTextWidget<const char*>("Layer ", EMPTY_STRING);
            ECGui::InsertSameLine();
            OnLayerListUpdate(entcom);
           
            ECGui::PushItemWidth(WindowSize_.getX());
            ECGui::InsertHorizontalLineSeperator();

            static ImGuiTextFilter CompFilter;
            CompFilter.Draw(EMPTY_STRING, 0.0f, "Component Filter");

            ShowPrefebProperty(currEnt);
            ShowBPProperty("BluePrint", currEnt, CompFilter);
            ShowTransformProperty("Transform", currEnt, CompFilter, mesheditor->IsVisible);
            ShowPointLightProperty("PointLight", currEnt, CompFilter);
            ShowSpotLightProperty("SpotLight", currEnt, CompFilter);
            ShowDirectionalLightProperty("DirectionalLight", currEnt, CompFilter);
            ShowRigidBodyProperty("RigidBody", currEnt, CompFilter);
            ShowEditorCameraProperty("Camera", currEnt, CompFilter);
            ShowTextureProperty("Texture", currEnt, CompFilter);
            ShowMaterialProperty("Material", currEnt, CompFilter);
            ShowMesh3DProperty("Mesh", currEnt, CompFilter);
            ShowScriptProperty("Script Details", currEnt, CompFilter);
            ShowAudioProperty("Audio", currEnt, CompFilter);
            ShowCollisionProperty("Collision", currEnt, CompFilter);
            ShowAIProperty("AI Properties", currEnt, CompFilter);
            ShowParentProperty("Parent", currEnt, CompFilter);
            ShowChildProperty("Child", currEnt, CompFilter);




            AddComponentsController(currEnt);
            ECGui::NextColumn();
            RemoveComponentsController(currEnt);
            ECGui::SetColumns(1, nullptr, true);
        }
        else
        {
            const char* entName = "No Entities";

            ECGui::DrawInputTextWidget("EntityName", const_cast<char*>(entName),
                strlen(entName), ImGuiInputTextFlags_ReadOnly);
        }

        ECGui::InsertHorizontalLineSeperator();
    }

    bool InspectorWindow::ShowTransformProperty(const char* name, Entity ID, ImGuiTextFilter& filter, bool IsNotInScene)
    {
        if (engine->world.CheckComponent<TransformComponent>(ID))
        {
            if (filter.PassFilter(name) && ECGui::CreateCollapsingHeader(name))
            {
                ECGui::SetColumns(2, nullptr, true);
                ECGui::InsertHorizontalLineSeperator();
                ECGui::SetColumnOffset(1, 140);

                auto& transCom = engine->world.GetComponent<TransformComponent>(ID);
                ECGui::DrawTextWidget<const char*>("Position", EMPTY_STRING);
                ECGui::NextColumn();
                ECGui::PushItemWidth(ECGui::GetWindowSize().x);

                auto& ent = engine->world.GetComponent<EntityComponent>(ID);

                if (ECGui::DrawSliderFloat3Widget("TransVec", &transCom.position, true, -100.f, 100.f, ID))
                {
                    if (engine->world.CheckComponent<ChildComponent>(ID))
                    {
                        auto& child = engine->world.GetComponent<ChildComponent>(ID);
                        child.UpdateChildren = true;

                        if (ent.Tag != EntityType::ENT_MESH)
                        {
                            engine->gPicker.UpdateAabb(ID);
                            engine->gDynamicAABBTree.UpdateData(ID);
                        }
                    }
                }
                else
                {
                    if (ImGui::IsItemDeactivatedAfterChange() && engine->world.CheckComponent<ChildComponent>(ID))
                    {
                        auto& child = engine->world.GetComponent<ChildComponent>(ID);
                        child.UpdateChildren = false;
                    }
                }

                ECGui::NextColumn();
                ECGui::DrawTextWidget<const char*>("Rotation", EMPTY_STRING);
                ECGui::NextColumn();
                ECGui::PushItemWidth(ECGui::GetWindowSize().x);

                if (ECGui::DrawSliderFloat3Widget("TransRot", &transCom.rotation, true, -360.f, 360.f, ID))
                {
                    if (engine->world.CheckComponent<SpotLightComponent>(ID))
                    {

                    }
                }

                ECGui::NextColumn();
                ECGui::DrawTextWidget<const char*>("Scale", EMPTY_STRING);
                ECGui::NextColumn();
                ECGui::PushItemWidth(ECGui::GetWindowSize().x);

                ECGui::DrawSliderFloat3Widget("TransScale", &transCom.scale, true, -100.f, 100.f, ID);

                //Update for DynamicAABB Tree -Rachel
                if (!IsNotInScene)
                {
                    engine->gPicker.UpdateAabb(ID);
                    engine->gDynamicAABBTree.UpdateData(ID);

                    if ((engine->world.CheckComponent<ParentComponent>(ID)) && (ent.Tag != EntityType::ENT_MODEL))
                    {
                        auto& parent = engine->world.GetComponent<ParentComponent>(ID);

                        for (auto& it : parent.child)
                        {
                            engine->gPicker.UpdateAabb(it);
                            engine->gDynamicAABBTree.UpdateData(it);
                        }
                    }
                }

                ECGui::SetColumns(1, nullptr, true);
                ECGui::InsertHorizontalLineSeperator();
            }
        }

        return false;
    }

    bool InspectorWindow::ShowPointLightProperty(const char* name, Entity ID, ImGuiTextFilter& filter)
    {
        if (engine->world.CheckComponent<PointLightComponent>(ID))
        {
            if (filter.PassFilter(name) && ECGui::CreateCollapsingHeader(name))
            {
                ECGui::SetColumns(2, nullptr, true);
                ECGui::InsertHorizontalLineSeperator();
                ECGui::SetColumnOffset(1, 140);

                auto& _PointLight = engine->world.GetComponent<PointLightComponent>(ID);

                ECGui::DrawTextWidget<const char*>("IntensityStrength", EMPTY_STRING);
                ECGui::NextColumn();
                ECGui::PushItemWidth(ECGui::GetWindowSize().x);
                ECGui::DrawSliderFloatWidget("IntensityFloat", &_PointLight.IntensityStrength, true, 0.f, 100.0f);
                ECGui::NextColumn();

                ECGui::DrawTextWidget<const char*>("Affects World", EMPTY_STRING);
                ECGui::NextColumn();
                ECGui::PushItemWidth(ECGui::GetWindowSize().x);
                ECGui::CheckBoxBool("Affects World", &_PointLight.AffectsWorld);
                ECGui::NextColumn();

                ECGui::DrawTextWidget<const char*>("Light Colour", EMPTY_STRING);
                ECGui::NextColumn();
                ECGui::ColorPicker3("PLightColor", (float*)&_PointLight.RGBColor,
                    ImGuiColorEditFlags_PickerHueWheel | ImGuiColorEditFlags_DisplayRGB);
                ECGui::DrawSliderFloat4Widget("ColourVec", &_PointLight.RGBColor, true, 0.0f, 1.0f);
                engine->LightManager.SetLightColor(_PointLight, { _PointLight.RGBColor.getX() ,_PointLight.RGBColor.getY() , _PointLight.RGBColor.getZ() , 1.0f });
                ECGui::NextColumn();

                ECGui::DrawTextWidget<const char*>("Attenuation Level", EMPTY_STRING);
                ECGui::NextColumn();

                ECGui::PushItemWidth(ECGui::GetWindowSize().x);
                ECGui::DrawSliderIntWidget("PLightColourVec", &_PointLight.AttenuationLevel, true, 0, 10);
                engine->LightManager.SetAttenuation(_PointLight, _PointLight.AttenuationLevel);
                ECGui::NextColumn();

                //ECGui::DrawTextWidget<const char*>("Light Ambient", EMPTY_STRING);
                //ECGui::NextColumn();
                //ECGui::PushItemWidth(ECGui::GetWindowSize().x);
                //ECGui::DrawSliderFloat3Widget("PLightAmbientVec", &_PointLight.ambient, true, 0.0f, 1.0f);
                //ECGui::NextColumn();
                //ECGui::DrawTextWidget<const char*>("Light Diffuse", EMPTY_STRING);
                //ECGui::NextColumn();
                //ECGui::PushItemWidth(ECGui::GetWindowSize().x);
                //ECGui::DrawSliderFloat3Widget("PLightDiffuseVec", &_PointLight.diffuse, true, 0.0f, 1.0f);
                //ECGui::NextColumn();
                //ECGui::DrawTextWidget<const char*>("Light Specular", EMPTY_STRING);
                //ECGui::NextColumn();
                //ECGui::PushItemWidth(ECGui::GetWindowSize().x);
                //ECGui::DrawSliderFloat3Widget("PLightSpecularVec", &_PointLight.specular, true, 0.0f, 1.0f);
                //ECGui::NextColumn();
                //ECGui::DrawTextWidget<const char*>("Intensity Strength", EMPTY_STRING);
                //ECGui::NextColumn();
                //ECGui::PushItemWidth(ECGui::GetWindowSize().x);
                //ECGui::DrawSliderFloatWidget("Intensity Strength", &_PointLight.IntensityStrength, true, 0.0f, 50.0f);
                //ECGui::NextColumn();
                //ECGui::DrawTextWidget<const char*>("Constant", EMPTY_STRING);
                //ECGui::NextColumn();
                //ECGui::PushItemWidth(ECGui::GetWindowSize().x);
                //ECGui::DrawSliderFloatWidget("Constant", &_PointLight.constant, true, 0.0f, 50.0f);
                //ECGui::NextColumn();
                //ECGui::DrawTextWidget<const char*>("Linear", EMPTY_STRING);
                //ECGui::NextColumn();
                //ECGui::PushItemWidth(ECGui::GetWindowSize().x);
                //ECGui::DrawSliderFloatWidget("Linear", &_PointLight.linear, true, 0.0f, 50.0f);
                //ECGui::NextColumn();
                //ECGui::DrawTextWidget<const char*>("Quadratic", EMPTY_STRING);
                //ECGui::NextColumn();
                //ECGui::PushItemWidth(ECGui::GetWindowSize().x);
                //ECGui::DrawSliderFloatWidget("Quadratic", &_PointLight.quadratic, true, 0.0f, 50.0f);
                //ECGui::NextColumn();
                //ECGui::DrawTextWidget<const char*>("Radius", EMPTY_STRING);
                //ECGui::NextColumn();
                //ECGui::PushItemWidth(ECGui::GetWindowSize().x);
                //ECGui::DrawSliderFloatWidget("Radius", &_PointLight.radius, true, 0.0f, 50.0f);
                //ECGui::NextColumn();
                //ECGui::DrawTextWidget<const char*>("Enable Blinn Phong", EMPTY_STRING);
                //ECGui::NextColumn();
                //ECGui::PushItemWidth(ECGui::GetWindowSize().x);
                //ECGui::CheckBoxBool("Enable Blinn Phong", &_PointLight.EnableBlinnPhong);
                //ECGui::NextColumn();
                //ECGui::DrawTextWidget<const char*>("Visible", EMPTY_STRING);
                //ECGui::NextColumn();
                //ECGui::PushItemWidth(ECGui::GetWindowSize().x);
                //ECGui::CheckBoxBool("Enable Blinn Phong Visible", &_PointLight.visible);
                //ECGui::NextColumn();
                //ECGui::DrawTextWidget<const char*>("Affects World", EMPTY_STRING);
                //ECGui::NextColumn();
                //ECGui::PushItemWidth(ECGui::GetWindowSize().x);
                //ECGui::CheckBoxBool("Affects World", &_PointLight.AffectsWorld);
                ECGui::SetColumns(1, nullptr, true);
                ECGui::InsertHorizontalLineSeperator();
            }
        }

        return false;
    }

    bool InspectorWindow::ShowSpotLightProperty(const char* name, Entity ID, ImGuiTextFilter& filter)
    {
        if (engine->world.CheckComponent<SpotLightComponent>(ID))
        {
            if (filter.PassFilter(name) && ECGui::CreateCollapsingHeader(name))
            {
                ECGui::SetColumns(2, nullptr, true);
                ECGui::InsertHorizontalLineSeperator();
                ECGui::SetColumnOffset(1, 140);

                auto& _SpotLight = engine->world.GetComponent<SpotLightComponent>(ID);

                ECGui::DrawTextWidget<const char*>("IntensityStrength", EMPTY_STRING);
                ECGui::NextColumn();
                ECGui::PushItemWidth(ECGui::GetWindowSize().x);
                ECGui::DrawSliderFloatWidget("IntensityFloat", &_SpotLight.IntensityStrength, true, 0.f, 100.0f);
                ECGui::NextColumn();

                ECGui::DrawTextWidget<const char*>("Affects World", EMPTY_STRING);
                ECGui::NextColumn();
                ECGui::PushItemWidth(ECGui::GetWindowSize().x);
                ECGui::CheckBoxBool("Affects World", &_SpotLight.AffectsWorld);
                ECGui::NextColumn();

                ECGui::DrawTextWidget<const char*>("Light Colour", EMPTY_STRING);
                ECGui::NextColumn();
                ECGui::ColorPicker3("SLightColor", (float*)&_SpotLight.RGBColor,
                    ImGuiColorEditFlags_PickerHueWheel | ImGuiColorEditFlags_DisplayRGB);
                ECGui::NextColumn();

                ECGui::DrawTextWidget<const char*>("Light Direction", EMPTY_STRING);
                ECGui::NextColumn();
                ECGui::PushItemWidth(ECGui::GetWindowSize().x);
                ECGui::DrawSliderFloat3Widget("PLightDirectionVec", &_SpotLight.direction, true, -5.0f, 5.0f);
                ECGui::NextColumn();

                ECGui::DrawTextWidget<const char*>("Attenuation Level", EMPTY_STRING);
                ECGui::NextColumn();
                ECGui::PushItemWidth(ECGui::GetWindowSize().x);
                ECGui::DrawSliderIntWidget("PLightColourVec", &_SpotLight.AttenuationLevel, true, 0, 10);
                engine->LightManager.SetAttenuation(_SpotLight, _SpotLight.AttenuationLevel);
                ECGui::NextColumn();

                //ECGui::DrawTextWidget<const char*>("Light Ambient", EMPTY_STRING);
                //ECGui::NextColumn();
                //ECGui::PushItemWidth(ECGui::GetWindowSize().x);
                //ECGui::DrawSliderFloat3Widget("PLightAmbientVec", &_SpotLight.ambient, true, 0.0f, 1.0f);
                //ECGui::NextColumn();
                //ECGui::DrawTextWidget<const char*>("Light Diffuse", EMPTY_STRING);
                //ECGui::NextColumn();
                //ECGui::PushItemWidth(ECGui::GetWindowSize().x);
                //ECGui::DrawSliderFloat3Widget("PLightDiffuseVec", &_SpotLight.diffuse, true, 0.0f, 1.0f);
                //ECGui::NextColumn();
                //ECGui::DrawTextWidget<const char*>("Light Specular", EMPTY_STRING);
                //ECGui::NextColumn();
                //ECGui::PushItemWidth(ECGui::GetWindowSize().x);
                //ECGui::DrawSliderFloat3Widget("PLightSpecularVec", &_SpotLight.specular, true, 0.0f, 1.0f);
                //ECGui::NextColumn();

                ECGui::DrawTextWidget<const char*>("OuterCutOff", EMPTY_STRING);
                ECGui::NextColumn();
                ECGui::PushItemWidth(ECGui::GetWindowSize().x);
                ECGui::DrawSliderFloatWidget("SLightOuterCutOffFloat", &_SpotLight.outerCutOff, true, 0.f, 80.0f);
                ECGui::NextColumn();

                ECGui::DrawTextWidget<const char*>("CutOff", EMPTY_STRING);
                ECGui::NextColumn();
                ECGui::PushItemWidth(ECGui::GetWindowSize().x);
                ECGui::DrawSliderFloatWidget("SLightCutOffFloat", &_SpotLight.cutOff, true, 0.f, (_SpotLight.outerCutOff - 1.0f));
                ECGui::NextColumn();

                ECGui::DrawTextWidget<const char*>("Direction", EMPTY_STRING);
                ECGui::NextColumn();
                ECGui::PushItemWidth(ECGui::GetWindowSize().x);
                ECGui::DrawSliderFloat3Widget("SLightDirectionVec", &_SpotLight.direction, true, 0.f, 150.f);
                ECGui::NextColumn();

                ECGui::DrawTextWidget<const char*>("Constant", EMPTY_STRING);
                ECGui::NextColumn();
                ECGui::PushItemWidth(ECGui::GetWindowSize().x);
                ECGui::DrawSliderFloatWidget("Constant", &_SpotLight.constant, true, 0.0f, 50.0f);
                ECGui::NextColumn();

                ECGui::DrawTextWidget<const char*>("Linear", EMPTY_STRING);
                ECGui::NextColumn();
                ECGui::PushItemWidth(ECGui::GetWindowSize().x);
                ECGui::DrawSliderFloatWidget("Linear", &_SpotLight.linear, true, 0.0f, 50.0f);
                ECGui::NextColumn();

                ECGui::DrawTextWidget<const char*>("Quadratic", EMPTY_STRING);
                ECGui::NextColumn();
                ECGui::PushItemWidth(ECGui::GetWindowSize().x);
                ECGui::DrawSliderFloatWidget("Quadratic", &_SpotLight.quadratic, true, 0.0f, 50.0f);
                ECGui::NextColumn();

                ECGui::DrawTextWidget<const char*>("Radius", EMPTY_STRING);
                ECGui::NextColumn();
                ECGui::PushItemWidth(ECGui::GetWindowSize().x);
                ECGui::DrawSliderFloatWidget("Radius", &_SpotLight.radius, true, 0.0f, 50.0f);
                ECGui::NextColumn();

                //ECGui::DrawTextWidget<const char*>("Enable Blinn Phong", EMPTY_STRING);
                //ECGui::NextColumn();
                //ECGui::PushItemWidth(ECGui::GetWindowSize().x);
                //ECGui::CheckBoxBool("Enable Blinn Phong", &_SpotLight.EnableBlinnPhong);
                //ECGui::NextColumn();
                //ECGui::DrawTextWidget<const char*>("Visible", EMPTY_STRING);
                //ECGui::NextColumn();
                //ECGui::PushItemWidth(ECGui::GetWindowSize().x);
                //ECGui::CheckBoxBool("Enable Blinn PhongVisible", &_SpotLight.visible);
                //ECGui::NextColumn();

                ECGui::SetColumns(1, nullptr, true);
                ECGui::InsertHorizontalLineSeperator();
            }
        }

        return false;
    }

    bool InspectorWindow::ShowDirectionalLightProperty(const char* name, Entity ID, ImGuiTextFilter& filter)
    {
        if (engine->world.CheckComponent<DirectionalLightComponent>(ID))
        {
            if (filter.PassFilter(name) && ECGui::CreateCollapsingHeader(name))
            {
                ECGui::SetColumns(2, nullptr, true);
                ECGui::InsertHorizontalLineSeperator();
                ECGui::SetColumnOffset(1, 140);

                auto& _DLight = engine->world.GetComponent<DirectionalLightComponent>(ID);

                ECGui::DrawTextWidget<const char*>("Light Colour", EMPTY_STRING);
                ECGui::NextColumn();
                ECGui::ColorPicker3("DLightColor", (float*)&_DLight.lightColor,
                    ImGuiColorEditFlags_PickerHueWheel | ImGuiColorEditFlags_DisplayRGB);
                ECGui::NextColumn();

                ECGui::DrawTextWidget<const char*>("DLight Direction", EMPTY_STRING);
                ECGui::NextColumn();
                ECGui::PushItemWidth(ECGui::GetWindowSize().x);
                ECGui::DrawSliderFloat3Widget("DLight Direction", &_DLight.Direction, true, -10.0f, 10.0f);
                ECGui::NextColumn();

                ECGui::DrawTextWidget<const char*>("Affects World", EMPTY_STRING);
                ECGui::NextColumn();
                ECGui::PushItemWidth(ECGui::GetWindowSize().x);
                ECGui::CheckBoxBool("Affects World", &_DLight.AffectsWorld);
                ECGui::NextColumn();

                //ECGui::DrawTextWidget<const char*>("DLight Ambient", EMPTY_STRING);
                //ECGui::NextColumn();
                //ECGui::PushItemWidth(ECGui::GetWindowSize().x);
                //ECGui::DrawSliderFloat3Widget("DLightAmbientVec", &_DLight.ambient, true, 0.0f, 1.0f);
                //ECGui::NextColumn();
                //ECGui::DrawTextWidget<const char*>("DLight Diffuse", EMPTY_STRING);
                //ECGui::NextColumn();
                //ECGui::PushItemWidth(ECGui::GetWindowSize().x);
                //ECGui::DrawSliderFloat3Widget("DLightDiffuseVec", &_DLight.diffuse, true, 0.0f, 1.0f);
                //ECGui::NextColumn();
                //ECGui::DrawTextWidget<const char*>("DLight Specular", EMPTY_STRING);
                //ECGui::NextColumn();
                //ECGui::PushItemWidth(ECGui::GetWindowSize().x);
                //ECGui::DrawSliderFloat3Widget("DLightSpecularVec", &_DLight.specular, true, 0.0f, 1.0f);
                //ECGui::NextColumn();

                ECGui::SetColumns(1);
                ECGui::InsertHorizontalLineSeperator();
            }
        }

        return false;
    }

    bool InspectorWindow::ShowRigidBodyProperty(const char* name, Entity ID, ImGuiTextFilter& filter)
    {
        if (engine->world.CheckComponent<RigidBodyComponent>(ID))
        {
            if (filter.PassFilter(name) && ECGui::CreateCollapsingHeader(name))
            {
                ECGui::SetColumns(2, nullptr, true);
                ECGui::InsertHorizontalLineSeperator();
                ECGui::SetColumnOffset(1, 140);

                auto& _RigidB = engine->world.GetComponent<RigidBodyComponent>(ID);

                ECGui::DrawTextWidget<const char*>("Enable Gravity", EMPTY_STRING);
                ECGui::NextColumn();
                ECGui::PushItemWidth(ECGui::GetWindowSize().x);
                ECGui::CheckBoxBool("Rigid Body Enable Gravity", &_RigidB.enableGravity);
                ECGui::NextColumn();
                ECGui::DrawTextWidget<const char*>("Forces", EMPTY_STRING);
                ECGui::NextColumn();
                ECGui::PushItemWidth(ECGui::GetWindowSize().x);
                ECGui::DrawSliderFloat3Widget("Rigid Body Forces", &_RigidB.forces, true, 0.0f, 1.0f);
                ECGui::NextColumn();
                ECGui::DrawTextWidget<const char*>("Velocity", EMPTY_STRING);
                ECGui::NextColumn();
                ECGui::PushItemWidth(ECGui::GetWindowSize().x);
                ECGui::DrawSliderFloat3Widget("Rigid Body Velocity", &_RigidB.velocity, true, 0.0f, 1.0f);
                ECGui::SetColumns(1, nullptr, true);
                ECGui::InsertHorizontalLineSeperator();
            }
        }

        return false;
    }

    bool InspectorWindow::ShowEditorCameraProperty(const char* name, Entity ID, ImGuiTextFilter& filter)
    {
        if (engine->world.CheckComponent<CameraComponent>(ID))
        {
            if (filter.PassFilter(name) && ECGui::CreateCollapsingHeader(name))
            {
                auto& _Camera = engine->world.GetComponent<CameraComponent>(ID);
                ECGui::SetColumns(2, nullptr, true);
                ECGui::InsertHorizontalLineSeperator();
                ECGui::SetColumnOffset(1, 140);

                ECGui::DrawTextWidget<const char*>("Camera Speed", EMPTY_STRING);
                ECGui::NextColumn();
                ECGui::PushItemWidth(ECGui::GetWindowSize().x);
                ECGui::DrawSliderFloatWidget("Camera Speed", &_Camera.cameraSpeed);
                ECGui::SetColumns(1, nullptr, true);
                ECGui::InsertHorizontalLineSeperator();
            }
        }

        return false;
    }

    bool InspectorWindow::ShowTextureProperty(const char* name, Entity ID, ImGuiTextFilter& filter)
    {
        if (engine->world.CheckComponent<MaterialComponent>(ID))
        {
            if (filter.PassFilter(name) && ECGui::CreateCollapsingHeader(name))
            {
                ECGui::SetColumns(2, nullptr, true);
                ECGui::InsertHorizontalLineSeperator();
                ECGui::SetColumnOffset(1, 140);
                auto& _Texture = engine->world.GetComponent<MaterialComponent>(ID);

                ECGui::DrawTextWidget<const char*>("NoTextures", EMPTY_STRING);
                ECGui::NextColumn();
                ECGui::PushItemWidth(ECGui::GetWindowSize().x);
                ECGui::CheckBoxBool("NoTextures", &_Texture.NoTextures);

                ComboListSettings settings = { "Texture Type" };
                ECGui::NextColumn();
                ECGui::DrawTextWidget<const char*>("Texture Type", EMPTY_STRING);
                ECGui::NextColumn();
                ECGui::PushItemWidth(ECGui::GetWindowSize().x);

                std::vector<std::string> _TextureVector = { "TT_UNASSIGNED","TT_2D","BasicPrimitives","TT_3D" };

                std::map<std::string, TextureType> _Map = { {"TT_UNASSIGNED",TextureType::TT_UNASSIGNED}, {"TT_2D",TextureType::TT_2D},
                                                            {"TT_3D",TextureType::TT_3D} };

                if (engine->world.CheckComponent<ModelComponent>(ID))
                {
                    if (ECGui::CreateComboList(settings, _TextureVector, _Texture.ComboIndex))
                    {
                        TextureType oldtemp = _Texture.Type;
                        _Texture.Type = _Map[_TextureVector[_Texture.ComboIndex]];
                        CommandHistory::RegisterCommand(new PrimitiveDeltaCommand<TextureType>{ oldtemp,  _Texture.Type });
                    }

                    ECGui::NextColumn();
                    ChangeTextureController(_Texture, ID);
                }
                else
                {
                    if (_Texture.hasTexture)
                    {
                        if (ECGui::CreateComboList(settings, _TextureVector, _Texture.ComboIndex))
                        {
                            TextureType oldtemp = _Texture.Type;
                            _Texture.Type = _Map[_TextureVector[_Texture.ComboIndex]];
                            CommandHistory::RegisterCommand(new PrimitiveDeltaCommand<TextureType>{ oldtemp,  _Texture.Type });
                        }

                        ECGui::NextColumn();
                        ChangeTextureController(_Texture, ID);
                    }
                }

                ECGui::SetColumns(1, nullptr, true);
                ECGui::InsertHorizontalLineSeperator();
            }
        }

        return false;
    }

    bool InspectorWindow::ShowRenderProperty(const char* name, Entity ID, ImGuiTextFilter& filter)
    {
        if (engine->world.CheckComponent<MeshComponent>(ID))
        {
            if (filter.PassFilter(name) && ECGui::CreateCollapsingHeader(name))
            {
                auto& _Render = engine->world.GetComponent<MeshComponent>(ID);

                ECGui::DrawTextWidget<const char*>("Transparency", EMPTY_STRING);
                ECGui::DrawSliderFloatWidget("Render Transparency", &_Render.transparency, true, 0.0f, 200.0f);

                //sceneloaded models
                //THIS IS WORK IN PROGRESS TESTING OUT FUNCITONALITIES AND ARE NOT MEANT TO BE IN THE FINAL
                //VERSION *NOT FOR FINAL VERSION* - TIAN YU
                std::string nameString = _Render.modelRef + " (Mesh Filter)";
                ECGui::PushStyleColor(ImGuiCol_Header, IM_COL32(0, 1, 1, 1));
                if (filter.PassFilter(nameString.c_str()) && ECGui::CreateCollapsingHeader(nameString.c_str()))
                {
                    ECGui::DrawTextWidget<const char*>("Mesh ", EMPTY_STRING);
                    ECGui::InsertSameLine();
                    ChangeMeshController(ID);
                }
                ECGui::PopStyleColor();
            }
        }

        return false;
    }

    bool InspectorWindow::ShowMaterialProperty(const char* name, Entity ID, ImGuiTextFilter& filter)
    {
        if (engine->world.CheckComponent<MaterialComponent>(ID))
        {
            if (filter.PassFilter(name) && ECGui::CreateCollapsingHeader(name))
            {
                ECGui::SetColumns(2, nullptr, true);
                ECGui::InsertHorizontalLineSeperator();
                ECGui::SetColumnOffset(1, 140);

                auto& _Material = engine->world.GetComponent<MaterialComponent>(ID);

                std::vector<std::string> _ModelVector = { "None","BasicPrimitives","Models3D" };

                std::map<std::string, MaterialModelType> _Map = { {"None",MaterialModelType::MT_NONE}, {"BasicPrimitives",MaterialModelType::MT_BASIC},
                                                            {"Models3D",MaterialModelType::MT_MODELS3D} };

                ComboListSettings settings = { "Model Type" };

                ECGui::DrawTextWidget<const char*>("Model Type", "");
                ECGui::NextColumn();
                ECGui::PushItemWidth(ECGui::GetWindowSize().x);
                ECGui::CreateComboList(settings, _ModelVector, _Material.ComboIndex);
                _Material.Modeltype = _Map[_ModelVector[_Material.ComboIndex]];

                ECGui::NextColumn();
                ECGui::DrawTextWidget<const char*>("Normal Map", EMPTY_STRING);
                ECGui::NextColumn();
                ECGui::PushItemWidth(ECGui::GetWindowSize().x);
                if (_Material.MaterialInstanceName.empty() == true)
                {
                    ECGui::CheckBoxBool("Normal Map", &_Material.IsNormalMap);
                }
                ECGui::NextColumn();

                //ECGui::NextColumn();
                //ECGui::DrawTextWidget<const char*>("Ambient", EMPTY_STRING);
                //ECGui::NextColumn();
                //ECGui::PushItemWidth(ECGui::GetWindowSize().x);
                //ECGui::DrawSliderFloat3Widget("Material Ambient", &_Material.ambient, true, 0.0f, 1.0f);
                //ECGui::NextColumn();
                //
                //ECGui::DrawTextWidget<const char*>("Diffuse", EMPTY_STRING);
                //ECGui::NextColumn();
                //ECGui::PushItemWidth(ECGui::GetWindowSize().x);
                //ECGui::DrawSliderFloat3Widget("Material Diffuse", &_Material.diffuse, true, 0.0f, 1.0f);
                //ECGui::NextColumn();
                //
                //ECGui::DrawTextWidget<const char*>("Specular", EMPTY_STRING);
                //ECGui::NextColumn();
                //ECGui::PushItemWidth(ECGui::GetWindowSize().x);
                //ECGui::DrawSliderFloat3Widget("Material Specular", &_Material.specular, true, 0.0f, 1.0f);
                //ECGui::NextColumn();
                //
                //ECGui::DrawTextWidget<const char*>("MaximumShininess", EMPTY_STRING);
                //ECGui::NextColumn();
                //ECGui::PushItemWidth(ECGui::GetWindowSize().x);
                //ECGui::DrawSliderFloatWidget("Material MaximumShininess", &_Material.MaximumShininess, true, 0.0f, 200.0f);
                //ECGui::NextColumn();

                ECGui::DrawTextWidget<const char*>("Highlight", EMPTY_STRING);
                ECGui::NextColumn();
                ECGui::PushItemWidth(ECGui::GetWindowSize().x);
                ECGui::CheckBoxBool("Enable Blinn Phong", &_Material.Highlight);
                ECGui::SetColumns(1, nullptr, true);
                ECGui::InsertHorizontalLineSeperator();

                ECGui::NextColumn();
                ECGui::PushItemWidth(ECGui::GetWindowSize().x);
                ECGui::DrawInputTextHintWidget(my_strcat("MaterialInstance", 1).c_str(), "Drag Material Instance here", const_cast<char*>(_Material.MaterialInstanceName.c_str()), 256, true, ImGuiInputTextFlags_None);
                engine->editorManager->DragAndDropInst_.StringPayloadTarget("mat", _Material.MaterialInstanceName, "Albdeo Texture Inserted.", PayloadTargetType::PTT_ASSETS, ID);

                if (ECGui::ButtonBool("Clear Material", { ImGui::GetColumnWidth(), 25 }))
                {
                    _Material.MaterialInstanceName.clear();
                }

                ECGui::SetColumns(1, nullptr, true);
                ECGui::InsertHorizontalLineSeperator();
            }
        }

        return false;
    }

    bool InspectorWindow::ShowMesh3DProperty(const char* name, Entity ID, ImGuiTextFilter& filter)
    {
        if (engine->world.CheckComponent<MeshComponent>(ID))
        {
            if (filter.PassFilter(name) && ECGui::CreateCollapsingHeader(name))
            {
                ECGui::SetColumns(2, nullptr, true);
                ECGui::InsertHorizontalLineSeperator();
                ECGui::SetColumnOffset(1, 140);

                auto& _Mesh = engine->world.GetComponent<MeshComponent>(ID);

                ECGui::DrawTextWidget<const char*>("Model Name: ", EMPTY_STRING);
                ECGui::NextColumn();
                ECGui::PushItemWidth(ECGui::GetWindowSize().x);
                ECGui::DrawTextWidget<const char*>(_Mesh.MeshName.data(), EMPTY_STRING);
                ECGui::NextColumn();

                ECGui::DrawTextWidget<const char*>("Transparecncy: ", EMPTY_STRING);
                ECGui::NextColumn();
                ECGui::PushItemWidth(ECGui::GetWindowSize().x);
                ECGui::DrawSliderFloatWidget("Transparecncy", &_Mesh.transparency, true, 0.f, 1.0f);
                ECGui::NextColumn();

                ECGui::DrawTextWidget<const char*>("Environment Map", EMPTY_STRING);
                ECGui::NextColumn();
                ECGui::PushItemWidth(ECGui::GetWindowSize().x);
                ECGui::CheckBoxBool("Environment Map", &_Mesh.ENV_MAP);

                if (_Mesh.ENV_MAP)
                {
                    static size_t comboindex = 0;
                    std::vector<std::string> MapVector = { "REFLECT", "REFRACT" };
                    ComboListSettings settings = { "Map Type" };
                    ECGui::NextColumn();
                    ECGui::DrawTextWidget<const char*>("Map Type", "");
                    ECGui::NextColumn();
                    ECGui::PushItemWidth(ECGui::GetWindowSize().x);
                    ECGui::CreateComboList(settings, MapVector, comboindex);
                    ECGui::NextColumn();
                    _Mesh.ENV_TYPE = static_cast<MeshComponent::MapType>(comboindex);
                }

                ImGui::Dummy({ 2,2 });

                std::string nameString = _Mesh.modelRef + " (Mesh Filter)";
                ImGui::PushStyleColor(ImGuiCol_Header, IM_COL32(0, 1, 1, 1));
                ECGui::NextColumn();
                ECGui::SetColumns(1, nullptr, true);
                if (filter.PassFilter(nameString.c_str()) && ECGui::CreateCollapsingHeader(nameString.c_str()))
                {
                    ECGui::SetColumns(2, nullptr, true);
                    ECGui::SetColumnOffset(1, 140);
                    ECGui::DrawTextWidget<const char*>("Mesh ", EMPTY_STRING);
                    ECGui::NextColumn();
                    ECGui::PushItemWidth(ECGui::GetWindowSize().x);
                    ChangeMeshController(ID);
                }
                ImGui::PopStyleColor();
                ECGui::SetColumns(1, nullptr, true);
                ECGui::InsertHorizontalLineSeperator();
            }
        }
        return false;
    }

    bool InspectorWindow::ShowModelInfoProperty(const char* name, Entity ID, ImGuiTextFilter& filter)
    {
        if (engine->world.CheckComponent<ModelComponent>(ID))
        {
            if (filter.PassFilter(name) && ECGui::CreateCollapsingHeader(name))
            {
                ECGui::SetColumns(2, nullptr, true);
                ECGui::InsertHorizontalLineSeperator();
                ECGui::SetColumnOffset(1, 140);

                std::vector<std::string> _ModelInfoVector = { "MT_UNASSIGNED","MT_HUMAN","MT_ANIMAL","MT_HOUSE","MT_ENVIRONMENT" };

                std::map<std::string, ModelType> _Map = { {"MT_UNASSIGNED",ModelType::MT_UNASSIGNED}, {"MT_HUMAN",ModelType::MT_HUMAN},
                                                            {"MT_ANIMAL",ModelType::MT_ANIMAL},{"MT_HOUSE",ModelType::MT_HOUSE},
                                                            {"MT_ENVIRONMENT",ModelType::MT_ENVIRONMENT} };

                auto& _ModelInfo = engine->world.GetComponent<ModelComponent>(ID);

                ComboListSettings settings{ "Texture Type" };

                ECGui::DrawTextWidget<const char*>("Model Directory: ", EMPTY_STRING);
                ECGui::NextColumn();
                ECGui::PushItemWidth(ECGui::GetWindowSize().x);
                ECGui::DrawTextWidget<const char*>(_ModelInfo.Directory.c_str(), "");
                ECGui::NextColumn();
                ECGui::DrawTextWidget<const char*>("Model Name: ", EMPTY_STRING);
                ECGui::NextColumn();
                ECGui::PushItemWidth(ECGui::GetWindowSize().x);
                ECGui::DrawTextWidget<const char*>(_ModelInfo.NameOfModel.c_str(), "");
                ECGui::NextColumn();
                ECGui::DrawTextWidget<const char*>("Model Type", EMPTY_STRING);
                ECGui::NextColumn();
                ECGui::PushItemWidth(ECGui::GetWindowSize().x);
                ECGui::CreateComboList(settings, _ModelInfoVector, _ModelInfo.ComboIndex);
                _ModelInfo.type = _Map[_ModelInfoVector[_ModelInfo.ComboIndex]];
                ECGui::SetColumns(1, nullptr, true);
                ECGui::InsertHorizontalLineSeperator();
            }
        }
        return false;
    }

    bool InspectorWindow::ShowScriptProperty(const char* name, Entity ID, ImGuiTextFilter& filter)
    {
        if (engine->world.CheckComponent<ScriptComponent>(ID))
        {
            if (filter.PassFilter(name) && ECGui::CreateCollapsingHeader(name))
            {
                ECGui::InsertHorizontalLineSeperator();
                auto& scriptCom = engine->world.GetComponent<ScriptComponent>(ID);

                ECGui::DrawTextWidget<const char*>(my_strcat("List of Scripts (",
                    scriptCom.scriptList.size(), "):").c_str(), "");

                for (size_t i = 0; i < scriptCom.scriptList.size(); ++i)
                {
                    if (!IsRemovingScripts)
                    {
                        ECGui::DrawInputTextHintWidget(my_strcat("ScriptName", i + 1).c_str(), "Drag Script files here",
                            const_cast<char*>(scriptCom.scriptList[i].scriptName.c_str()), 256,
                            true, ImGuiInputTextFlags_ReadOnly);
                        engine->editorManager->DragAndDropInst_.StringPayloadTarget("cs", scriptCom.scriptList[i].scriptName,
                            "Script File inserted.", PayloadTargetType::PTT_WIDGET, ID, i);
                    }
                    else
                    {
                        bool selected = false;

                        if (ECGui::CreateSelectableButton(my_strcat(scriptCom.scriptList[i].scriptName.c_str(), " ", i + 1).c_str(), &selected))
                        {
                            auto posItr = scriptCom.scriptList.begin() + i;
                            scriptCom.scriptList.erase(posItr);
                        }
                    }
                }

                ECGui::SetColumns(2, nullptr, true);
                ECGui::InsertHorizontalLineSeperator();

                if (!IsRemovingScripts)
                {
                    if (ECGui::ButtonBool("Add Script", { ImGui::GetColumnWidth(), 25 }))
                    {
                        std::string scriptName;
                        scriptName.reserve(256);
                        scriptCom.scriptList.push_back({});
                        scriptCom.scriptList.back().scriptName = scriptName;
                    }

                    ECGui::NextColumn();

                    if (ECGui::ButtonBool("Remove Script", { ImGui::GetColumnWidth(), 25 }))
                    {
                        IsRemovingScripts = true;
                    }
                }
                else
                {

                    ECGui::SetColumns(1, nullptr, true);
                    ECGui::NextColumn();
                    if (ECGui::ButtonBool("Cancel Remove", { ImGui::GetColumnWidth(), 25 }))
                    {
                        IsRemovingScripts = false;
                    }
                }

                ECGui::SetColumns(1, nullptr, true);
                ECGui::InsertHorizontalLineSeperator();
            }
        }

        return false;
    }

    bool InspectorWindow::ShowAudioProperty(const char* name, Entity ID, ImGuiTextFilter& filter)
    {
        if (engine->world.CheckComponent<AudioComponent>(ID))
        {
            if (filter.PassFilter(name) && ECGui::CreateCollapsingHeader(name))
            {
                ECGui::InsertHorizontalLineSeperator();
                static bool IsAudioPlaying = false;
                auto& audio = engine->world.GetComponent<AudioComponent>(ID);

                if (!IsAudioPlaying)
                {
                    if (audio.AudioPath.max_size() <= 0)
                    {
                        audio.AudioPath.reserve(256);
                    }

                    if (ECGui::ButtonBool("Play Audio"))
                    {
                        if (!audio.AudioPath.empty())
                        {
                            SimulateAudio(ID, audio);
                            IsAudioPlaying = true;
                            EDITOR_LOG_INFO("Playing sound...");
                        }
                        else
                        {
                            EDITOR_LOG_WARN("No audio path detected. Please specify and audio file.");
                        }
                    }

                    ECGui::DrawInputTextHintWidget("AudioPath", "Drag Audio files here",
                        const_cast<char*>(audio.AudioPath.c_str()), 256,
                        true, ImGuiInputTextFlags_ReadOnly);

                    engine->editorManager->DragAndDropInst_.StringPayloadTarget("wav", audio.AudioPath,
                        "Wav File inserted.");

                    if (ECGui::ButtonBool("Clear Audio", { ImGui::GetColumnWidth(),25 }))
                    {
                        audio.AudioPath.clear();
                    }
                    ECGui::InsertHorizontalLineSeperator();

                    ECGui::CheckBoxBool("IsMuted", &audio.IsMuted, false);

                    ECGui::DrawTextWidget<const char*>("Volume: ", EMPTY_STRING);
                    ECGui::NextColumn();
                    ECGui::PushItemWidth(ECGui::GetWindowSize().x);
                    ECGui::DrawSliderFloatWidget("Volume", &audio.Volume, true, 0.f, 1.0f);
                    ECGui::NextColumn();
                    // Pitch value, 0.5 = half pitch, 2.0 = double pitch, etc default = 1.0.
                    ECGui::DrawTextWidget<const char*>("Pitch: ", EMPTY_STRING);
                    ECGui::DrawSliderFloatWidget("Pitch", &audio.Pitch, true, 0.f, 2.0f);

                    // Relative speed of the song from 0.01 to 100.0. 0.5 = half speed, 
                    // 2.0 = double speed. Default = 1.0.
                    ECGui::DrawTextWidget<const char*>("Speed: ", EMPTY_STRING);
                    ECGui::DrawSliderFloatWidget("Speed", &audio.Speed, true, 0.f, 5.0f);
                    ECGui::NextColumn();
                    ECGui::SetColumns(1, nullptr, true);
                    ECGui::InsertHorizontalLineSeperator();
                    ECGui::CheckBoxBool("Loop", &audio.IsLooping, false);
                    ECGui::CheckBoxBool("Is3D", &audio.Is3D, false);

                    if (audio.Is3D)
                    {
                        ECGui::InsertHorizontalLineSeperator();
                        ECGui::SetColumns(2, nullptr, true);
                        // Inside cone angle, in degrees, from 0 to 360. 
                        // This is the angle within which the sound is at its normal volume. 
                        // Must not be greater than outsideconeangle. Default = 360.
                        ECGui::DrawTextWidget<const char*>("Inner Cone Angle: ", EMPTY_STRING);
                        ECGui::DrawSliderFloatWidget("InnerConeAngle", &audio.InnerConeAngle, true, 0.f, 360.0f);
                        ECGui::NextColumn();
                        // Outside cone angle, in degrees, from 0 to 360. 
                        // This is the angle outside of which the sound is at its outside volume.
                        // Must not be less than insideconeangle. Default = 360.
                        ECGui::DrawTextWidget<const char*>("Outer Cone Angle: ", EMPTY_STRING);
                        ECGui::DrawSliderFloatWidget("OuterConeAngle", &audio.OuterConeAngle, true, 0.f, 360.f);
                        ECGui::NextColumn();
                        // The volume of the sound outside the outside angle of the sound projection cone.
                        ECGui::DrawTextWidget<const char*>("Outer Volume: ", EMPTY_STRING);
                        ECGui::DrawSliderFloatWidget("OuterVolume", &audio.OuterVolume, true, 0.f, 1.f);
                        ECGui::SetColumns(1, nullptr, true);
                        ECGui::InsertHorizontalLineSeperator();

                        // Increase the mindistance of a sound to make it 'louder' in a 3D world, 
                        // and decrease it to make it 'quieter' in a 3D world.
                        // Max distance is effectively obsolete unless you need the sound to stop 
                        // fading out at a certain point. 
                        // Do not adjust this from the default if you dont need to.
                        // Summary:
                        // Min -> min dist where it will start growing louder when getting closer
                        // Max -> max dist where it will stop fading
                        ECGui::DrawTextWidget<const char*>("Sound Attenuation: ", EMPTY_STRING);
                        ECGui::DrawSliderFloatWidget("Min Distance", &audio.Min, false, 0.f, 500.f);
                        ECGui::DrawSliderFloatWidget("Max Distance", &audio.Max, false, 0.f, 1000.f);

                        ECGui::InsertHorizontalLineSeperator();

                        ECGui::DrawTextWidget<const char*>("Special Settings: ", EMPTY_STRING);
                        ECGui::CheckBoxBool("Radio Effect", &audio.HasRadioEffect, false);
                        ECGui::CheckBoxBool("Echo Effect", &audio.HasEchoEffect, false);

                        if (audio.HasEchoEffect)
                            ECGui::DrawSliderFloatWidget("Delay Time", &audio.EchoDelayTime, false, 10.f, 1000.f);
                        ECGui::NextColumn();
                        ECGui::SetColumns(1, nullptr, true);
                        ECGui::InsertHorizontalLineSeperator();
                    }
                }
                else
                {
                    if (ECGui::ButtonBool("Stop Audio"))
                    {
                        engine->audioManager.UnloadSound(audio.AudioPath);
                        IsAudioPlaying = false;
                        EDITOR_LOG_INFO("Stopping sound...");
                    }

                    if (ECGui::CheckBoxBool("IsMuted", &audio.IsMuted, false))
                    {
                        engine->audioManager.SetChannelMute(audio.ChannelID, audio.IsMuted);
                    }
                }

                ECGui::SetColumns(1, nullptr, true);
                ECGui::InsertHorizontalLineSeperator();
            }
        }
        return false;
    }

    bool InspectorWindow::ShowCollisionProperty(const char* name, Entity ID, ImGuiTextFilter& filter)
    {
        char hxValue[256];
        char hyValue[256];
        char hzValue[256];
        char radiusValue[256];
        if (engine->world.CheckComponent<CollisionComponent>(ID))
        {
            if (filter.PassFilter(name) && ECGui::CreateCollapsingHeader(name))
            {
                ECGui::InsertHorizontalLineSeperator();
                auto& _Collision = engine->world.GetComponent<CollisionComponent>(ID);

                switch (_Collision.shape.shape)
                {
                case PxShapeType::Px_CUBE:
                    ECGui::DrawTextWidget<const char*>("CUBE ", EMPTY_STRING);
                    ECGui::InsertHorizontalLineSeperator();
                    ECGui::SetColumns(2, nullptr, true);
                    ECGui::InsertHorizontalLineSeperator();

                    ECGui::DrawTextWidget<const char*>("Hx: ", EMPTY_STRING);
                    ECGui::NextColumn();
                    ECGui::PushItemWidth(ECGui::GetWindowSize().x);
                    snprintf(hxValue, 256, "%f", _Collision.shape.hx);
                    ECGui::DrawInputTextWidget("Hx: ", hxValue, 256, ImGuiInputTextFlags_EnterReturnsTrue);
                    _Collision.shape.hx = static_cast<float>(atof(hxValue));
                    ECGui::NextColumn();
                    ECGui::DrawTextWidget<const char*>("Hy: ", EMPTY_STRING);
                    ECGui::NextColumn();
                    ECGui::PushItemWidth(ECGui::GetWindowSize().x);
                    snprintf(hyValue, 256, "%f", _Collision.shape.hy);
                    ECGui::DrawInputTextWidget("Hy: ", hyValue, 256, ImGuiInputTextFlags_EnterReturnsTrue);
                    _Collision.shape.hy = static_cast<float>(atof(hyValue));
                    ECGui::NextColumn();
                    ECGui::DrawTextWidget<const char*>("Hz: ", EMPTY_STRING);
                    ECGui::NextColumn();
                    ECGui::PushItemWidth(ECGui::GetWindowSize().x);
                    snprintf(hzValue, 256, "%f", _Collision.shape.hz);
                    ECGui::DrawInputTextWidget("Hz: ", hzValue, 256, ImGuiInputTextFlags_EnterReturnsTrue);
                    _Collision.shape.hz = static_cast<float>(atof(hzValue));
                    ECGui::NextColumn();
                    ECGui::SetColumns(1, nullptr, true);
                    ECGui::InsertHorizontalLineSeperator();
                    break;
                case PxShapeType::Px_SPHERE:
                    ECGui::DrawTextWidget<const char*>("SPHERE ", EMPTY_STRING);
                    ECGui::InsertHorizontalLineSeperator();
                    ECGui::SetColumns(2, nullptr, true);
                    ECGui::DrawTextWidget<const char*>("Radius: ", EMPTY_STRING);
                    ECGui::NextColumn();
                    ECGui::PushItemWidth(ECGui::GetWindowSize().x);
                    snprintf(radiusValue, 256, "%f", _Collision.shape.radius);
                    ECGui::DrawInputTextWidget("Radius: ", radiusValue, 256, ImGuiInputTextFlags_EnterReturnsTrue);
                    ECGui::NextColumn();
                    _Collision.shape.radius = static_cast<float>(atof(radiusValue));
                    ECGui::SetColumns(1, nullptr, true);
                    ECGui::InsertHorizontalLineSeperator();
                    break;
                }

                if (engine->world.CheckComponent<ScriptComponent>(ID))
                {
                    OnCollisionMatrixUpdate(ID);
                }
            }
        }
        return false;
    }

    bool InspectorWindow::ShowPrefebProperty(Entity ID)
    {
        if (engine->world.CheckComponent<PrefabComponent>(ID) && !engine->world.GetComponent<PrefabComponent>(ID).IsInstance)
        {
            ECGui::InsertHorizontalLineSeperator();
            ECGui::SetColumns(2, nullptr, true);
            ECGui::DrawTextWidget<const char*>("Prefeb: ", EMPTY_STRING);
            ECGui::NextColumn();

            if (ECGui::ButtonBool("Apply changes to all"))
            {
                engine->pfManager.ApplyChangesToAll(ID);
            }
            if (ECGui::IsItemHovered())
            {
                ECGui::BeginToolTip();
                ECGui::PushTextWrapPos(ECGui::GetFontSize() * 35.0f);
                ECGui::TextUnformatted("Apply changes to all prefab instances.");
                ECGui::PopTextWrapPos();
                ECGui::EndTooltip();
            }
            ECGui::SetColumns(1, nullptr, true);
            ECGui::InsertHorizontalLineSeperator();
        }
        return false;
    }

    bool InspectorWindow::ShowAIProperty(const char* name, Entity ID, ImGuiTextFilter& filter)
    {
        if (engine->world.CheckComponent<AIComponent>(ID))
        {
            if (filter.PassFilter(name) && ECGui::CreateCollapsingHeader(name))
            {
                // ECGui::SetColumns(2, nullptr, true);
                ECGui::InsertHorizontalLineSeperator();
                //ECGui::SetColumnOffset(1, 150);
                auto& ai = engine->world.GetComponent<AIComponent>(ID);
                static std::string registeredEnt;

                ECGui::DrawTextWidget<const char*>("Register Entity as Waypoint: ", EMPTY_STRING);
                ECGui::DrawTextWidget<size_t>("Total Count", ai.waypoints.size());
                AddWaypointController(registeredEnt);

                if (ECGui::ButtonBool("Register"))
                {
                    if (registeredEnt != EMPTY_STRING)
                        engine->gAI.AddWaypoint(ID, lexical_cast<Entity>(registeredEnt));
                }

                ECGui::InsertHorizontalLineSeperator();
            }
        }

        return false;
    }

    bool InspectorWindow::ShowBPProperty(const char* name, Entity ID, ImGuiTextFilter& filter)
    {
        (void)name;
        (void)ID;
        (void)filter;
        //if (engine->world.CheckComponent<Editor_>(ID))
        //{
        //    if (filter.PassFilter(name) && ECGui::CreateCollapsingHeader(name))
        //    {
        //        ECGui::InsertHorizontalLineSeperator();
        //
        //       auto& nodeEditor = engine->world.GetComponent<Editor_>(ID);
        //
        //        ImGui::Begin("test");
        //        temps.DrawNodeEditor(name, nodeEditor);
        //     
        //        ImGui::End();
        //        ECGui::InsertHorizontalLineSeperator();
        //    }
        //}

        return false;
    }

    bool InspectorWindow::ShowParentProperty(const char* name, Entity ID, ImGuiTextFilter& filter)
    {
        if (engine->world.CheckComponent<ParentComponent>(ID))
        {
            if (filter.PassFilter(name) && ECGui::CreateCollapsingHeader(name))
            {
                ECGui::InsertHorizontalLineSeperator();

                auto& parent = engine->world.GetComponent<ParentComponent>(ID);
                ECGui::DrawTextWidget<const char*>("Child:", EMPTY_STRING);
                for (auto& it : parent.child)
                {
                    auto& en = engine->world.GetComponent<EntityComponent>(it);
                    ECGui::DrawTextWidget<const char*>(my_strcat(en.Name, " ", it).c_str(), EMPTY_STRING);
                }

                ECGui::InsertHorizontalLineSeperator();
            }
        }

        return false;
    }

    bool InspectorWindow::ShowChildProperty(const char* name, Entity ID, ImGuiTextFilter& filter)
    {
        if (engine->world.CheckComponent<ChildComponent>(ID))
        {
            if (filter.PassFilter(name) && ECGui::CreateCollapsingHeader(name))
            {
                ECGui::InsertHorizontalLineSeperator();

                ECGui::DrawTextWidget<const char*>("Parent:", EMPTY_STRING);
                auto& child = engine->world.GetComponent<ChildComponent>(ID);
                auto& childEntComp = engine->world.GetComponent<EntityComponent>(ID);
                auto& en = engine->world.GetComponent<EntityComponent>(child.parentIndex);
                ECGui::DrawTextWidget<const char*>(my_strcat(en.Name, " ", child.parentIndex).c_str(), EMPTY_STRING);

                ECGui::InsertHorizontalLineSeperator();

                if (ECGui::ButtonBool("Break From Parent"))
                {
                    auto& parentComp = engine->world.GetComponent<EntityComponent>(child.parentIndex);
                    auto& parent = engine->world.GetComponent<ParentComponent>(child.parentIndex);
                    for (auto& allchld : parentComp.Child)
                    {
                        if (allchld == ID)
                        {
                            parentComp.Child.erase(std::remove(parentComp.Child.begin(), parentComp.Child.end(), allchld), parentComp.Child.end());
                        }
                    }
                    for (auto& allchld : parent.child)
                    {
                        if (allchld == ID)
                        {
                            parent.child.erase(std::remove(parent.child.begin(), parent.child.end(), allchld), parent.child.end());
                        }
                    }

                    if (parentComp.Child.empty())
                    {
                        ComponentRegistry<ParentComponent>("ParentComponent", child.parentIndex, childEntComp.Name, EditComponent::EC_REMOVECOMPONENT);
                    }

                    childEntComp.Parent.clear();

                    //ComponentRegistry<ParentComponent>("ParentComponent", ID, childEntComp.Name, EditComponent::EC_ADDCOMPONENT);
                    ComponentRegistry<ChildComponent>("ChildComponent", ID, childEntComp.Name, EditComponent::EC_REMOVECOMPONENT);

                }

            }
        }
        return false;
    }

    void InspectorWindow::AddComponentsController(Entity ID)
    {
        if (ECGui::ButtonBool(("Add Component"), { ImGui::GetColumnWidth(),25 }))
        {
            ECGui::OpenPopup("Add Component");
        }
        if (ECGui::BeginPopup("Add Component"))
        {
            ECGui::SetScrollY(5);
            ChildSettings settings{ "Add Components", ImVec2{ 250,200 } };
            ECGui::DrawChildWindow<void(Entity)>(settings, std::bind(&InspectorWindow::ShowAddComponentList,
                this, std::placeholders::_1), ID);
            ECGui::EndPopup();
        }
    }

    void InspectorWindow::RemoveComponentsController(Entity ID)
    {
        if (ECGui::ButtonBool(("Remove Component"), { ImGui::GetColumnWidth(),25 }))
        {
            ECGui::OpenPopup("Remove Component");
        }
        if (ECGui::BeginPopup("Remove Component"))
        {
            ECGui::SetScrollY(5);
            ChildSettings settings{ "Remove Components", ImVec2{ 250,200 } };
            ECGui::DrawChildWindow<void(Entity)>(settings, std::bind(&InspectorWindow::ShowRemoveComponentList,
                this, std::placeholders::_1), ID);
            ECGui::EndPopup();
        }
    }

    void InspectorWindow::ShowAddComponentList(Entity ID)
    {
        static ImGuiTextFilter AddComponentFilter;

        auto& entCom = engine->world.GetComponent<EntityComponent>(ID);

        AddComponentFilter.Draw("Filter", 160);

        for (int i = 0; i < engine->world.GetAllComponentNames().size(); i++)
        {
            if (AddComponentFilter.PassFilter(engine->world.GetAllComponentNames()[i].c_str()))
            {
                if (ECGui::ButtonBool(engine->world.GetAllComponentNames()[i].c_str(), ImVec2(200, 0)))
                {
                    switch (str2int(engine->world.GetAllComponentNames()[i].c_str()))
                    {
                    case str2int("TransformComponent"):
                        ComponentRegistry<TransformComponent>("TransformComponent", ID, entCom.Name,
                            EditComponent::EC_ADDCOMPONENT);
                        break;
                    case str2int("MeshComponent"):
                        ComponentRegistry<MeshComponent>("MeshComponent", ID, entCom.Name,
                            EditComponent::EC_ADDCOMPONENT);
                        break;
                    case str2int("CameraComponent"):
                        ComponentRegistry<CameraComponent>("CameraComponent", ID, entCom.Name,
                            EditComponent::EC_ADDCOMPONENT);
                        break;
                    case str2int("PointLightComponent"):
                        ComponentRegistry<PointLightComponent>("PointLightComponent", ID, entCom.Name,
                            EditComponent::EC_ADDCOMPONENT);
                        break;
                    case str2int("DirectionalLightComponent"):
                        ComponentRegistry<DirectionalLightComponent>("DirectionalLightComponent", ID, entCom.Name,
                            EditComponent::EC_ADDCOMPONENT);
                        break;
                    case str2int("AABBComponent"):
                        ComponentRegistry<AABBComponent>("AABBComponent", ID, entCom.Name,
                            EditComponent::EC_ADDCOMPONENT);
                        break;
                    case str2int("SpotLightComponent"):
                        ComponentRegistry<SpotLightComponent>("SpotLightComponent", ID, entCom.Name,
                            EditComponent::EC_ADDCOMPONENT);
                        break;
                    case str2int("MaterialComponent"):
                        ComponentRegistry<MaterialComponent>("MaterialComponent", ID, entCom.Name,
                            EditComponent::EC_ADDCOMPONENT);
                        break;
                    case str2int("RigidBodyComponent"):
                        ComponentRegistry<RigidBodyComponent>("RigidBodyComponent", ID, entCom.Name,
                            EditComponent::EC_ADDCOMPONENT);
                        break;
                    case str2int("TextureComponent"):
                        ComponentRegistry<TextureComponent>("TextureComponent", ID, entCom.Name,
                            EditComponent::EC_ADDCOMPONENT);
                        break;
                    case str2int("ModelComponent"):
                        ComponentRegistry<ModelComponent>("ModelComponent", ID, entCom.Name,
                            EditComponent::EC_ADDCOMPONENT);
                        break;
                    case str2int("ScriptComponent"):
                        ComponentRegistry<ScriptComponent>("ScriptComponent", ID, entCom.Name,
                            EditComponent::EC_ADDCOMPONENT);
                        break;
                    case str2int("AudioComponent"):
                        ComponentRegistry<AudioComponent>("AudioComponent", ID, entCom.Name,
                            EditComponent::EC_ADDCOMPONENT);
                        break;
                    case str2int("ParentComponent"):
                        ComponentRegistry<ParentComponent>("ParentComponent", ID, entCom.Name,
                            EditComponent::EC_ADDCOMPONENT);
                        break;
                    case str2int("ChildComponent"):
                        ComponentRegistry<ChildComponent>("ChildComponent", ID, entCom.Name,
                            EditComponent::EC_ADDCOMPONENT);
                        break;
                    case str2int("CollisionComponent"):
                        ComponentRegistry<CollisionComponent>("CollisionComponent", ID, entCom.Name,
                            EditComponent::EC_ADDCOMPONENT);
                        break;
                    case str2int("AIComponent"):
                        ComponentRegistry<AIComponent>("AIComponent", ID, entCom.Name,
                            EditComponent::EC_ADDCOMPONENT);
                        break;
                    case str2int("NodeEditor"):
                        ComponentRegistry<NodeEditor>("NodeEditor", ID, entCom.Name,
                            EditComponent::EC_ADDCOMPONENT);
                        break;
                    }

                    AddComponentFilter.Clear();
                }
            }
        }

    }

    void InspectorWindow::ShowRemoveComponentList(Entity ID)
    {
        static ImGuiTextFilter RemoveComponentFilter;

        auto& entCom = engine->world.GetComponent<EntityComponent>(ID);

        RemoveComponentFilter.Draw("Filter", 160);

        for (int i = 0; i < engine->world.GetAllComponentNames().size(); i++)
        {
            if (RemoveComponentFilter.PassFilter(engine->world.GetAllComponentNames()[i].c_str()))
            {
                if (ECGui::ButtonBool(engine->world.GetAllComponentNames()[i].c_str(), ImVec2(200, 0)))
                {
                    switch (str2int(engine->world.GetAllComponentNames()[i].c_str()))
                    {
                    case str2int("TransformComponent"):
                        ComponentRegistry<TransformComponent>("TransformComponent", ID, entCom.Name,
                            EditComponent::EC_REMOVECOMPONENT);
                        break;
                    case str2int("MeshComponent"):
                        ComponentRegistry<MeshComponent>("MeshComponent", ID, entCom.Name,
                            EditComponent::EC_REMOVECOMPONENT);
                        break;
                    case str2int("CameraComponent"):
                        ComponentRegistry<CameraComponent>("CameraComponent", ID, entCom.Name,
                            EditComponent::EC_REMOVECOMPONENT);
                        break;
                    case str2int("PointLightComponent"):
                        ComponentRegistry<PointLightComponent>("PointLightComponent", ID, entCom.Name,
                            EditComponent::EC_REMOVECOMPONENT);
                        break;
                    case str2int("DirectionalLightComponent"):
                        ComponentRegistry<DirectionalLightComponent>("DirectionalLightComponent", ID, entCom.Name,
                            EditComponent::EC_REMOVECOMPONENT);
                        break;
                    case str2int("AABBComponent"):
                        ComponentRegistry<AABBComponent>("AABBComponent", ID, entCom.Name,
                            EditComponent::EC_REMOVECOMPONENT);
                        break;
                    case str2int("SpotLightComponent"):
                        ComponentRegistry<SpotLightComponent>("SpotLightComponent", ID, entCom.Name,
                            EditComponent::EC_REMOVECOMPONENT);
                        break;
                    case str2int("MaterialComponent"):
                        ComponentRegistry<MaterialComponent>("MaterialComponent", ID, entCom.Name,
                            EditComponent::EC_REMOVECOMPONENT);
                        break;
                    case str2int("RigidBodyComponent"):
                        ComponentRegistry<RigidBodyComponent>("RigidBodyComponent", ID, entCom.Name,
                            EditComponent::EC_REMOVECOMPONENT);
                        break;
                    case str2int("TextureComponent"):
                        ComponentRegistry<TextureComponent>("TextureComponent", ID, entCom.Name,
                            EditComponent::EC_REMOVECOMPONENT);
                        break;
                    case str2int("ModelComponent"):
                        ComponentRegistry<ModelComponent>("ModelComponent", ID, entCom.Name,
                            EditComponent::EC_REMOVECOMPONENT);
                        break;
                    case str2int("ScriptComponent"):
                        ComponentRegistry<ScriptComponent>("ScriptComponent", ID, entCom.Name,
                            EditComponent::EC_REMOVECOMPONENT);
                        break;
                    case str2int("AudioComponent"):
                        ComponentRegistry<AudioComponent>("AudioComponent", ID, entCom.Name,
                            EditComponent::EC_REMOVECOMPONENT);
                        break;
                    case str2int("ParentComponent"):
                        ComponentRegistry<ParentComponent>("ParentComponent", ID, entCom.Name,
                            EditComponent::EC_REMOVECOMPONENT);
                        break;
                    case str2int("ChildComponent"):
                        ComponentRegistry<ChildComponent>("ChildComponent", ID, entCom.Name,
                            EditComponent::EC_REMOVECOMPONENT);
                        break;
                    case str2int("CollisionComponent"):
                        ComponentRegistry<CollisionComponent>("CollisionComponent", ID, entCom.Name,
                            EditComponent::EC_REMOVECOMPONENT);
                        break;
                    case str2int("AIComponent"):
                        ComponentRegistry<AIComponent>("AIComponent", ID, entCom.Name,
                            EditComponent::EC_REMOVECOMPONENT);
                        break;
                    case str2int("NodeEditor"):
                        ComponentRegistry<NodeEditor>("NodeEditor", ID, entCom.Name,
                            EditComponent::EC_REMOVECOMPONENT);
                        break;
                    }

                    RemoveComponentFilter.Clear();
                }
            }
        }
    }

    void InspectorWindow::ChangeTextureController(MaterialComponent& Item, Entity ID)
    {
        ImVec2 buttonSize = { 180,20 };
        ECGui::DrawTextWidget<const char*>("Texture  ", EMPTY_STRING);
        ECGui::NextColumn();
        ECGui::PushItemWidth(ECGui::GetWindowSize().x);

        if (ECGui::ButtonBool((Item.TextureRef.c_str()), buttonSize) || (ECGui::IsItemClicked(0) && ECGui::IsItemHovered()))
        {
            ECGui::OpenPopup("Texture Changer");
        }

        if (ECGui::BeginPopup("Texture Changer"))
        {
            ECGui::SetScrollY(5);
            ChildSettings settings{ "Texture Changer", ImVec2{ 250,250 } };

            if (engine->world.CheckComponent<ModelComponent>(ID))
            {
                ECGui::DrawChildWindow<void(MaterialComponent&)>(settings, std::bind(&InspectorWindow::ModelTextureList,
                    this, std::placeholders::_1), Item);
            }
            else
            {
                ECGui::DrawChildWindow<void(MaterialComponent&)>(settings, std::bind(&InspectorWindow::TextureList,
                    this, std::placeholders::_1), Item);
            }

            ECGui::EndPopup();
        }
    }

    void InspectorWindow::TextureList(MaterialComponent& Item)
    {
        static ImGuiTextFilter AddComponentFilter;
        MaterialComponent FolderIcon;
        std::vector<std::string> textureNames;
        textureNames.reserve(Graphics::textures.size());
        MaterialComponent icon;
        static float padding = 16.0f;
        static float thumbnaimsize = 50;
        float cellsize = thumbnaimsize + padding;
        float panelwidth = ECGui::GetContentRegionAvail().x;
        int columncount = (int)(panelwidth / cellsize);
        AddComponentFilter.Draw("Filter", 160);
        if (thumbnaimsize <= 30)
        {
            columncount = 1;
        }
        if (columncount < 1)
        {
            columncount = 1;
        }
        ECGui::DrawSliderFloatWidget("Size: ", &thumbnaimsize, false, 10, 200);
        ECGui::SetColumns(columncount, nullptr, true);
        for (auto it : Graphics::textures)
        {
            textureNames.push_back(it.first);
        }

        for (int i = 0; i < textureNames.size(); ++i)
        {

            FolderIcon.TextureRef = Graphics::textures.find(textureNames[i].c_str())->first;
            icon = FolderIcon;

            if (AddComponentFilter.PassFilter(textureNames[i].c_str()))
            {
                ECGui::ImageButton((void*)(intptr_t)Graphics::FindTextures((icon).TextureRef).GetHandle(),
                    { thumbnaimsize,thumbnaimsize },
                    { 1,0 },
                    { 2,1 });

                if (ECGui::IsItemClicked(0) && ECGui::IsItemHovered())
                {
                    ///////////////
                    std::pair<MMAPIterator, MMAPIterator> result = Graphics::textures.equal_range(textureNames[i].c_str());
                    Item.HoldingTextures.clear();
                    for (MMAPIterator it = result.first; it != result.second; it++)
                    {
                        Item.HoldingTextures.push_back(it->second);
                    }
                    //////////////////

                    Item.TextureRef = Graphics::textures.find((textureNames[i].c_str()))->first;
                    AddComponentFilter.Clear();
                    ECGui::CloseCurrentPopup();
                }

                ECGui::DrawTextWrappedWidget(textureNames[i].c_str(), "");
                ECGui::NextColumn();
            }

        }

    }

    void InspectorWindow::ModelTextureList(MaterialComponent& Item)
    {
        static ImGuiTextFilter AddComponentFilter;
        MaterialComponent FolderIcon;
        std::vector<std::string> textureNames;
        textureNames.reserve(Graphics::textures.size());
        MaterialComponent icon;
        static float padding = 16.0f;
        static float thumbnaimsize = 50;
        float cellsize = thumbnaimsize + padding;
        float panelwidth = ECGui::GetContentRegionAvail().x;
        int columncount = (int)(panelwidth / cellsize);
        AddComponentFilter.Draw("Filter", 160);
        if (thumbnaimsize <= 30)
        {
            columncount = 1;
        }
        if (columncount < 1)
        {
            columncount = 1;
        }
        ECGui::DrawSliderFloatWidget("Size: ", &thumbnaimsize, false, 10, 200);
        ECGui::SetColumns(columncount, nullptr, true);
        for (auto it : Graphics::textures)
        {
            textureNames.push_back(it.first);
        }

        for (int i = 0; i < textureNames.size(); ++i)
        {

            FolderIcon.TextureRef = Graphics::textures.find(textureNames[i].c_str())->first;
            icon = FolderIcon;

            if (AddComponentFilter.PassFilter(textureNames[i].c_str()))
            {
                ECGui::ImageButton((void*)(intptr_t)Graphics::FindTextures((icon).TextureRef).GetHandle(),
                    { thumbnaimsize,thumbnaimsize },
                    { 1,0 },
                    { 2,1 });

                if (ECGui::IsItemClicked(0) && ECGui::IsItemHovered())
                {
                    ///////////////
                    std::pair<MMAPIterator, MMAPIterator> result = Graphics::textures.equal_range(textureNames[i].c_str());
                    Item.HoldingTextures.clear();
                    for (MMAPIterator it = result.first; it != result.second; it++)
                    {
                        Item.HoldingTextures.push_back(it->second);
                        return;
                    }
                    //////////////////
                }

                ECGui::DrawTextWrappedWidget(textureNames[i].c_str(), "");
                ECGui::NextColumn();
            }

        }

    }

    void InspectorWindow::ChangeMeshController(Entity ID)
    {
        auto& Item = engine->world.GetComponent<MeshComponent>(ID);

        ImVec2 buttonSize = { 180,20 };

        if (engine->world.CheckComponent<ModelComponent>(ID))
        {
            if (ECGui::ButtonBool((Item.MeshName.data()), buttonSize))
            {
                ECGui::OpenPopup("Mesh Changer");
            }
        }
        else
        {
            if (ECGui::ButtonBool((Item.modelRef.c_str()), buttonSize))
            {
                ECGui::OpenPopup("Mesh Changer");
            }
        }
        if (ECGui::BeginPopup("Mesh Changer"))
        {
            ECGui::SetScrollY(5);
            ChildSettings settings{ "Mesh Changer", ImVec2{ 250,250 } };
            ECGui::DrawChildWindow<void(Entity&)>(settings, std::bind(&InspectorWindow::MeshList,
                this, std::placeholders::_1), ID);

            ECGui::EndPopup();
        }
    }

    void InspectorWindow::MeshList(Entity ID)
    {
        auto& Item = engine->world.GetComponent<MeshComponent>(ID);
        static ImGuiTextFilter AddComponentFilter;
        TextureComponent FolderIcon;

        AddComponentFilter.Draw("Filter", 160);
        FolderIcon.textureRef = Graphics::textures.find("FolderIcon")->first;

        TextureComponent icon = FolderIcon;

        static float padding = 16.0f;
        static float thumbnaimsize = 50;
        float cellsize = thumbnaimsize + padding;
        float panelwidth = ECGui::GetContentRegionAvail().x;
        int columncount = (int)(panelwidth / cellsize);
        if (thumbnaimsize <= 30)
        {
            columncount = 1;
        }
        if (columncount < 1)
        {
            columncount = 1;
        }
        ECGui::DrawSliderFloatWidget("Size: ", &thumbnaimsize, false, 10, 200);

        ECGui::SetColumns(columncount, nullptr, true);

        //use model info component to identify if the dude is basic or not 

        if (!engine->world.CheckComponent<ModelComponent>(ID))
        {
            for (int i = 0; i < engine->AssimpManager.GetPrimitiveNames().size(); ++i)
            {
                if (AddComponentFilter.PassFilter((engine->AssimpManager.GetPrimitiveNames()[i].c_str())))
                {
                    ECGui::ImageButton((void*)(intptr_t)Graphics::FindTextures(icon.textureRef).GetHandle(),
                        { thumbnaimsize,thumbnaimsize },
                        { 1,0 },
                        { 2,1 });

                    if (ECGui::IsItemClicked(0) && ECGui::IsItemHovered())
                    {
                        std::string oldtemp = Item.modelRef;
                        Item.modelRef = Graphics::models.find((engine->AssimpManager.GetPrimitiveNames()[i].c_str()))->first;
                        CommandHistory::RegisterCommand(new PrimitiveDeltaCommand<std::string>{ oldtemp, Item.modelRef });
                        AddComponentFilter.Clear();
                    }

                    ECGui::DrawTextWrappedWidget(engine->AssimpManager.GetPrimitiveNames()[i].c_str(), "");
                    ECGui::NextColumn();
                }

            }
        }
        else
        {
            for (int i = 0; i < engine->AssimpManager.GetMeshNames().size(); ++i)
            {
                if (engine->AssimpManager.GeometryContainerCheck(engine->AssimpManager.GetMeshNames()[i].c_str()) == false)
                    continue;

                if (AddComponentFilter.PassFilter((engine->AssimpManager.GetMeshNames()[i].c_str())))
                {
                    ECGui::ImageButton((void*)(intptr_t)Graphics::FindTextures((icon).textureRef).GetHandle(),
                        { thumbnaimsize,thumbnaimsize },
                        { 1,0 },
                        { 2,1 });

                    if (ECGui::IsItemClicked(0) && ECGui::IsItemHovered())
                    {
                        engine->AssimpManager.SetMeshComponent(ID, engine->AssimpManager.GetMeshNames()[i].c_str());
                        engine->AssimpManager.SetSingleMesh(ID, engine->AssimpManager.GetMeshNames()[i]);
                        //Item.MeshName = Graphics::models.find((engine->AssimpManager.AllMeshNames[i].c_str()))->first;
                        AddComponentFilter.Clear();
                    }

                    ECGui::DrawTextWrappedWidget(engine->AssimpManager.GetMeshNames()[i].c_str(), "");
                    ECGui::NextColumn();
                }

            }
        }

    }

    void InspectorWindow::SimulateAudio(Entity ID, AudioComponent& audioCom)
    {
        if (audioCom.Is3D)
        {
            auto& trans = engine->world.GetComponent<TransformComponent>(ID);
            engine->audioManager.Play3DSounds(audioCom, trans);
        }
        else
        {
            /*audioCom.ChannelID = engine->audioManager.Play2DSounds(audioCom.AudioPath, audioCom.Volume,
                audioCom.IsLooping);*/
            engine->audioManager.Play2DSounds(audioCom);
        }
    }

    void InspectorWindow::AddWaypointController(std::string& currentSelection)
    {
        static size_t index = 0;

        if (ImGuiAPI::BeginComboList("WaypointListBegin", currentSelection.c_str(), true))
        {
            for (size_t n = 0; n < engine->gAI.GetTargetPoints().size(); n++)
            {
                auto& entCom = engine->world.GetComponent<EntityComponent>(engine->gAI.GetTargetPoints()[n]);
                const bool is_selected = (index == n);

                if (ImGui::Selectable(my_strcat(entCom.Name, " ", engine->gAI.GetTargetPoints()[n]).c_str(), is_selected))
                {
                    index = n;
                    currentSelection = lexical_cast<std::string>(engine->gAI.GetTargetPoints()[n]);
                }

                if (is_selected)
                    ImGui::SetItemDefaultFocus();
            }

            ImGuiAPI::EndComboList();
        }
    }

    template <typename TComponents>
    void InspectorWindow::AddComponentsFeedback(const char* Components, const std::string& name, Entity ID, bool exist)
    {
        if (!exist)
        {
            engine->world.AddComponent(ID, TComponents{});

            if (!strcmp(Components, "AABBComponent"))
            {
                auto& Transform_ = engine->world.GetComponent<TransformComponent>(ID);
                auto& Entity_ = engine->world.GetComponent<EntityComponent>(ID);
                engine->gPicker.GenerateAabb(ID, Transform_, Entity_.Tag);
            }

            if (!strcmp(Components, "ScriptComponent"))
            {
                auto& Script_ = engine->world.GetComponent<ScriptComponent>(ID);
                auto& Entity_ = engine->world.GetComponent<EntityComponent>(ID);

                if (Entity_.LayerIndex != 0)
                    Script_.LayerMask.set(0, false);

                LoadScriptBitset(Script_);
            }

            std::string Comp = my_strcat(Components, " added for ", name, ". Add component succeeded.");
            EDITOR_LOG_INFO(Comp.c_str());
        }
        else
        {
            std::string Comp = my_strcat(Components, " already exists in ", name, ". Add component failed.");
            EDITOR_LOG_WARN(Comp.c_str());
        }
    }

    template <typename TComponents>
    void InspectorWindow::RemoveComponentsFeedback(const char* Components, const std::string& name, Entity ID, bool exist)
    {
        if (!strcmp(Components, "TransformComponent") || !strcmp(Components, "EntityComponent"))
        {
            EDITOR_LOG_WARN("Cannot remove this compulsory component for an Entity.");
        }
        else if (!exist)
        {
            std::string Comp = my_strcat(Components, " does not exists in ", name, ". Remove component failed.");
            EDITOR_LOG_WARN(Comp.c_str());
        }
        else
        {
            std::string Comp = my_strcat(Components, " removed for ", name, ". Remove component succeeded.");
            EDITOR_LOG_INFO(Comp.c_str());

            //Remove data from DynamicAABBTree if AABBComponent is deleted - Rachel
            if (!strcmp(Components, "AABBComponent"))
            {
                engine->gDynamicAABBTree.RemoveData(ID);
                engine->gCullingManager->Remove(ID);
            }

            engine->world.DestroyComponent<TComponents>(ID);
        }
    }

    void InspectorWindow::OnCollisionMatrixUpdate(Entity ID)
    {
        auto& entCom = engine->world.GetComponent<EntityComponent>(ID);
        auto& scriptCom = engine->world.GetComponent<ScriptComponent>(ID);
        auto* settings = engine->editorManager->GetEditorWindow<DebugWindow>();

        const char* currentLabel = nullptr;

        if (CollisionLayerChecker.Current.IsNothing)
        {
            currentLabel = "Nothing";
        }
        else if (CollisionLayerChecker.Current.IsEverything)
        {
            currentLabel = "Everything";
        }
        else if (CollisionLayerChecker.Current.UnLayerTracker.size() > 1)
        {
            currentLabel = "Mixed...";
        }
        else
        {
            currentLabel = settings->GetStringLayer(*CollisionLayerChecker.Current.UnLayerTracker.begin()).c_str();
        }

        ECGui::DrawTextWidget<const char*>("Collision Mask ", EMPTY_STRING);
        ECGui::InsertSameLine();
        if (ImGuiAPI::BeginComboList("CollisionLayerComboList", currentLabel, true))
        {
            for (const auto& pair : settings->GetLayerList())
            {
                if (!strcmp(pair.second.c_str(), EMPTY_STRING) || pair.first  == 0) continue;

                if (ImGui::Selectable(pair.second.c_str(), CollisionLayerChecker.Current.IndexActiveList[pair.first]))
                {
                    UpdateCollisionLayerTracker(settings, pair.first);
                    SetScriptBitset(scriptCom, entCom);
                }
            }

            ImGuiAPI::EndComboList();
        }
    }

    void InspectorWindow::UpdateCollisionLayerTracker(DebugWindow* dw, int ClickedIndex)
    {
        if (!strcmp(dw->GetStringLayer(ClickedIndex).c_str(), "Nothing"))
        {
            for (auto& [key, val] : CollisionLayerChecker.Current.IndexActiveList)
            {
                if (key == ClickedIndex)
                    val = true;
                else
                    val = false;
            }

            CollisionLayerChecker.Current.IsEverything = false;
            CollisionLayerChecker.Current.IsNothing = true;
            CollisionLayerChecker.Current.UnLayerTracker.clear();
        }
        else if (!strcmp(dw->GetStringLayer(ClickedIndex).c_str(), "Everything"))
        {
            int count = 0;

            for (auto& [key, val] : CollisionLayerChecker.Current.IndexActiveList)
            {
                if (count == dw->GetLayerListSize() + 3) break;
                if (!strcmp(dw->GetStringLayer(key).c_str(), EMPTY_STRING) || key == 0) continue;

                if (dw->GetIndexLayer("Nothing") == key)
                {
                    val = false;
                }
                else
                {
                    if (CollisionLayerChecker.Current.UnLayerTracker.find(key) == CollisionLayerChecker.Current.UnLayerTracker.end())
                    {
                        if (key != 2)
                            CollisionLayerChecker.Current.UnLayerTracker.insert(key);

                        val = true;
                    }
                }

                count++;
            }

            CollisionLayerChecker.Current.IsEverything = true;
            CollisionLayerChecker.Current.IsNothing = false;
        }
        else
        {
            auto search = CollisionLayerChecker.Current.IndexActiveList.find(ClickedIndex);

            if (search != CollisionLayerChecker.Current.IndexActiveList.end())
            {
                if (CollisionLayerChecker.Current.IndexActiveList[ClickedIndex])
                {
                    CollisionLayerChecker.Current.IndexActiveList[ClickedIndex] = false;
                    CollisionLayerChecker.Current.UnLayerTracker.erase(ClickedIndex);
                    CollisionLayerChecker.Current.IsEverything = false;

                    // Everything
                    CollisionLayerChecker.Current.IndexActiveList[2] = false;

                    if (CollisionLayerChecker.Current.UnLayerTracker.size() == 0)
                    {
                        CollisionLayerChecker.Current.IsNothing = true;
                        CollisionLayerChecker.Current.IndexActiveList[1] = true;
                    }
                }
                else
                {
                    CollisionLayerChecker.Current.IndexActiveList[ClickedIndex] = true;
                    CollisionLayerChecker.Current.UnLayerTracker.insert(ClickedIndex);
                    CollisionLayerChecker.Current.IsNothing = false;

                    // Nothing
                    CollisionLayerChecker.Current.IndexActiveList[1] = false;

                    if (CollisionLayerChecker.Current.UnLayerTracker.size() == dw->GetLayerListSize())
                    {
                        CollisionLayerChecker.Current.IsEverything = true;
                        CollisionLayerChecker.Current.IndexActiveList[2] = true;
                    }
                    
                }
            }
        }
    }

    void InspectorWindow::SetCollisionLayerTracker(const std::unordered_map<int, std::string>& layerlist)
    {
        auto scriptsys = engine->world.GetSystem<MonoSystem>();
        auto* dw = engine->editorManager->GetEditorWindow<DebugWindow>();

        CollisionLayerChecker.Current.Clear();

        for (const auto& [key, val] : layerlist)
        {
            if (key == 0 || key == 1)
                CollisionLayerChecker.Current.IndexActiveList[key] = true;
            else
                CollisionLayerChecker.Current.IndexActiveList[key] = false;
        }

        for (const auto& ent : scriptsys->mEntities)
        {
            auto& scriptcom = engine->world.GetComponent<ScriptComponent>(ent);
            auto& entcom = engine->world.GetComponent<EntityComponent>(ent);
            int counter = 0;

            for (const auto& [key, val] : layerlist)
            {
                if (!strcmp(val.c_str(), EMPTY_STRING))
                {
                    scriptcom.LayerMask.set((size_t)key, false);

                    if (entcom.LayerIndex == key)
                        entcom.LayerIndex = 0;

                    if (key != MAX_LAYER_SIZE - 1)
                        continue;
                }

                if (ent == engine->editorManager->GetSelectedEntity() && key > 2)
                {
                    if (scriptcom.LayerMask.test((size_t)key))
                    {
                        CollisionLayerChecker.Current.UnLayerTracker.insert(key);
                        CollisionLayerChecker.Current.IndexActiveList[key] = true;
                        counter++;
                    }

                    if (key == MAX_LAYER_SIZE - 1)
                    {
                        if (entcom.LayerIndex != 0)
                            scriptcom.LayerMask.set(0, false);
                        else
                            scriptcom.LayerMask.set(1, true);

                        if (counter == dw->GetLayerListSize())
                        {
                            CollisionLayerChecker.Current.IsEverything = true;
                            scriptcom.LayerMask.set(2, true);
                            CollisionLayerChecker.Current.IndexActiveList[2] = true;

                            CollisionLayerChecker.Current.IsNothing = false;
                            scriptcom.LayerMask.set(1, false);
                            CollisionLayerChecker.Current.IndexActiveList[1] = false;
                        }
                        else if (counter == 0)
                        {
                            CollisionLayerChecker.Current.IsNothing = true;
                            scriptcom.LayerMask.set(1, true);
                            CollisionLayerChecker.Current.IndexActiveList[1] = true;

                            CollisionLayerChecker.Current.IsEverything = false;
                            scriptcom.LayerMask.set(2, false);
                            CollisionLayerChecker.Current.IndexActiveList[2] = false;
                        }
                        else
                        {
                            CollisionLayerChecker.Current.IsEverything = false;
                            CollisionLayerChecker.Current.IsNothing = false;
                            scriptcom.LayerMask.set(2, false);
                            scriptcom.LayerMask.set(1, false);
                            CollisionLayerChecker.Current.IndexActiveList[2] = false;
                            CollisionLayerChecker.Current.IndexActiveList[1] = false;
                        }
                    }
                }
            }
        }
    }

    void InspectorWindow::SetScriptBitset(ScriptComponent& scriptCom, EntityComponent& entcom)
    {
        scriptCom.LayerMask.reset();

        if (entcom.LayerIndex != 0)
            scriptCom.LayerMask.set(0, false);

        for (const auto& [key, val] : CollisionLayerChecker.Current.IndexActiveList)
        {
            if (val)
                scriptCom.LayerMask.set(key);
            else
                scriptCom.LayerMask.set(key, val);
        }

        std::cout << scriptCom.LayerMask << std::endl;
    }

    void InspectorWindow::OnLayerListUpdate(EntityComponent& entcom)
    {
        auto* settings = engine->editorManager->GetEditorWindow<DebugWindow>();

        const char* currentLabel = settings->GetLayerList().find(entcom.LayerIndex)->second.c_str();

        if (ImGuiAPI::BeginComboList("LayerComboList", currentLabel, true))
        {
            for (const auto& pair : settings->GetLayerList())
            {
                if (!strcmp(pair.second.c_str(), EMPTY_STRING) || pair.first == 1 || pair.first == 2) continue;

                const bool is_selected = (entcom.LayerIndex == pair.first);

                if (ImGui::Selectable(pair.second.c_str(), is_selected))
                {
                    entcom.LayerIndex = pair.first;

                    if (engine->world.CheckComponent<ScriptComponent>(engine->editorManager->GetSelectedEntity()))
                    {
                        auto& scriptcom = engine->world.GetComponent<ScriptComponent>(engine->editorManager->GetSelectedEntity());
                        
                        if (entcom.LayerIndex != 0)
                            scriptcom.LayerMask.set(0, false);
                    }
                }

                if (is_selected)
                    ImGui::SetItemDefaultFocus();
            }

            ImGuiAPI::EndComboList();
        }
    }

    void InspectorWindow::LoadScriptBitset(ScriptComponent& scriptCom)
    {
        CollisionLayerChecker.Current.Clear();
        size_t counter = 0;
        
        for (size_t i = 0; i < scriptCom.LayerMask.size(); ++i)
        {
            if (scriptCom.LayerMask[i])
            {
                CollisionLayerChecker.Current.IndexActiveList[(int)i] = true;

                if (i > 2)
                {
                    CollisionLayerChecker.Current.UnLayerTracker.insert((int)i);
                    counter++;
                }
            }
            else
            {
                CollisionLayerChecker.Current.IndexActiveList[(int)i] = false;
            }
        }

        auto* dw = engine->editorManager->GetEditorWindow<DebugWindow>();

        if (counter == dw->GetLayerListSize())
        {
            CollisionLayerChecker.Current.IsEverything = true;
            CollisionLayerChecker.Current.IsNothing = false;
        }
        else if (counter == 0)
        {
            CollisionLayerChecker.Current.IsNothing = true;
            CollisionLayerChecker.Current.IsEverything = false;
        }
        else
        {
            CollisionLayerChecker.Current.IsNothing = false;
            CollisionLayerChecker.Current.IsEverything = false;
        }
    }
}
