#include "pch.h"
#include "Inspector.h"
#include "ECS/ComponentManager/Components/EntityComponent.h"
#include "ECS/ComponentManager/Components/TransformComponent.h"
#include "ECS/ComponentManager/Components/RigidBodyComponent.h"
#include "ECS/ComponentManager/Components/ScriptComponent.h"
#include "Editor/Windows/SwitchViews/TopSwitchViewWindow.h"
#include "ECS/ComponentManager/Components/ParentComponent.h"
#include "ECS/ComponentManager/Components/ChildComponent.h"
#include "ECS/SystemManager/Systems/System/Collision/CollisionSystem.h"

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
        WindowName = "Inspector";
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
            Entity currEnt = engine->editorManager->GetSelectedEntity();
            auto& entcom = engine->world.GetComponent<EntityComponent>(currEnt);
            std::string entityName = entcom.Name + " " + std::to_string(currEnt);

            ECGui::DrawInputTextWidget("EntityName", const_cast<char*>(entityName.c_str()),
                entityName.size(), ImGuiInputTextFlags_ReadOnly);

            ECGui::InsertHorizontalLineSeperator();

            static ImGuiTextFilter CompFilter;
            CompFilter.Draw();

            ECGui::PushItemWidth(WindowSize_.getX());
            //std::cout<<engine->editorManager->GetSelectedEntity();
            ShowPrefebProperty("Prefeb", currEnt, CompFilter);
            ShowEntityProperty("Tag", currEnt, CompFilter);
            ShowTransformProperty("Transform", currEnt, CompFilter);
            ShowPointLightProperty("PointLight", currEnt, CompFilter);
            ShowSpotLightProperty("SpotLight", currEnt, CompFilter);
            ShowDirectionalLightProperty("DirectionalLight", currEnt, CompFilter);
            ShowRigidBodyProperty("RigidBody", currEnt, CompFilter);
            ShowEditorCameraProperty("Camera", currEnt, CompFilter);
            ShowTextureProperty("Texture", currEnt, CompFilter);
            ShowRenderProperty("Render", currEnt, CompFilter);
            ShowMaterialProperty("Material", currEnt, CompFilter);
            ShowMesh3DProperty("Mesh", currEnt, CompFilter);
            ShowModelInfoProperty("ModelInfo", currEnt, CompFilter);
            ShowScriptProperty("Script Details", currEnt, CompFilter);
            ShowAudioProperty("Audio", currEnt, CompFilter);
            ShowCollisionProperty("Collision", currEnt, CompFilter);

            ECGui::InsertHorizontalLineSeperator();
            /*ECGui::PushItemWidth(WindowSize_.getX());*/
            AddComponentsController(currEnt);
            ECGui::InsertSameLine();
            RemoveComponentsController(currEnt);
        }
        else
        {
            const char* entName = "No Entities";

            ECGui::DrawInputTextWidget("EntityName", const_cast<char*>(entName),
                strlen(entName), ImGuiInputTextFlags_ReadOnly);
        }

        ECGui::InsertHorizontalLineSeperator();
    }

    bool InspectorWindow::ShowEntityProperty(const char* name, Entity ID, ImGuiTextFilter& filter)
    {
        if (engine->world.CheckComponent<EntityComponent>(ID))
        {
            if (filter.PassFilter(name) && ECGui::CreateCollapsingHeader(name))
            {
                static char entNameInput[256];
                auto& entCom = engine->world.GetComponent<EntityComponent>(ID);

                ECGui::DrawTextWidget<std::string>("Entity Tag", lexical_cast_toStr<EntityType>(entCom.Tag));
                ECGui::DrawTextWidget<const char*>("Edit Name:", "");
                ECGui::InsertSameLine();
                if (ECGui::DrawInputTextHintWidget("InputEntityName", "Enter Entity Name", entNameInput,
                    256, true, ImGuiInputTextFlags_EnterReturnsTrue))
                {
                    std::string oldName = entCom.Name;
                    entCom.Name = entNameInput;
                    CommandHistory::RegisterCommand(new PrimitiveDeltaCommand<std::string>{ oldName, entCom.Name });
                }

                /*engine->editorManager->Item_.GenericPayloadTarget("TESTING", testtest, "SUCCESSFUL");
                strcpy(test, testtest.c_str());*/
            }
        }

        return false;
    }

    bool InspectorWindow::ShowTransformProperty(const char* name, Entity ID, ImGuiTextFilter& filter)
    {
        if (engine->world.CheckComponent<TransformComponent>(ID))
        {
            if (filter.PassFilter(name) && ECGui::CreateCollapsingHeader(name))
            {
                auto& transCom = engine->world.GetComponent<TransformComponent>(ID);

                ECGui::DrawTextWidget<const char*>("Position", "");
                ECGui::DrawSliderFloat3Widget("TransVec", &transCom.position, true, -50.f, 50.f);

                ECGui::DrawTextWidget<const char*>("Rotation", "");
                ECGui::DrawSliderFloat3Widget("TransRot", &transCom.rotation, true, -360.f, 360.f);

                ECGui::DrawTextWidget<const char*>("Scale", "");
                ECGui::DrawSliderFloat3Widget("TransScale", &transCom.scale);

                //Update for DynamicAABB Tree -Rachel
                engine->gPicker.UpdateAabb(ID);
                engine->gDynamicAABBTree.UpdateData(ID);
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
                auto& _PointLight = engine->world.GetComponent<PointLightComponent>(ID);

                ECGui::DrawTextWidget<const char*>("IntensityStrength", "");
                ECGui::DrawSliderFloatWidget("IntensityFloat", &_PointLight.IntensityStrength, true, 0.f, 150.f);

                ECGui::DrawTextWidget<const char*>("Light Colour", "");

                ECGui::ColorPicker3("PLightColor", (float*)&_PointLight.Color,
                    ImGuiColorEditFlags_PickerHueWheel | ImGuiColorEditFlags_DisplayRGB);
                //ECGui::DrawSliderFloat4Widget("ColourVec", &_PointLight.Color, true, 0.0f, 1.0f);
                engine->LightManager.SetLightColor(_PointLight,
                    { _PointLight.Color.getX() ,_PointLight.Color.getY() , _PointLight.Color.getZ() , 1.0f });

                ECGui::DrawTextWidget<const char*>("Attenuation Level", "");
                ECGui::DrawSliderIntWidget("PLightColourVec", &_PointLight.AttenuationLevel, true, 0, 10);
                engine->LightManager.SetAttenuation(_PointLight, _PointLight.AttenuationLevel);

                ECGui::DrawTextWidget<const char*>("Light Ambient", "");
                ECGui::DrawSliderFloat3Widget("PLightAmbientVec", &_PointLight.ambient, true, 0.0f, 1.0f);

                ECGui::DrawTextWidget<const char*>("Light Diffuse", "");
                ECGui::DrawSliderFloat3Widget("PLightDiffuseVec", &_PointLight.diffuse, true, 0.0f, 1.0f);

                ECGui::DrawTextWidget<const char*>("Light Specular", "");
                ECGui::DrawSliderFloat3Widget("PLightSpecularVec", &_PointLight.specular, true, 0.0f, 1.0f);

                ECGui::DrawTextWidget<const char*>("Intensity Strength", "");
                ECGui::DrawSliderFloatWidget("Intensity Strength", &_PointLight.IntensityStrength, true, 0.0f, 50.0f);

                ECGui::DrawTextWidget<const char*>("Constant", "");
                ECGui::DrawSliderFloatWidget("Constant", &_PointLight.constant, true, 0.0f, 50.0f);

                ECGui::DrawTextWidget<const char*>("Linear", "");
                ECGui::DrawSliderFloatWidget("Linear", &_PointLight.linear, true, 0.0f, 50.0f);

                ECGui::DrawTextWidget<const char*>("Quadratic", "");
                ECGui::DrawSliderFloatWidget("Quadratic", &_PointLight.quadratic, true, 0.0f, 50.0f);

                ECGui::DrawTextWidget<const char*>("Radius", "");
                ECGui::DrawSliderFloatWidget("Radius", &_PointLight.radius, true, 0.0f, 50.0f);

                ECGui::SetColumns(2, NULL, true);
                ECGui::DrawTextWidget<const char*>("Enable Blinn Phong", "");
                ECGui::DrawTextWidget<const char*>("Visible", "");
                ECGui::DrawTextWidget<const char*>("Affects World", "");
                ECGui::NextColumn();
                ECGui::CheckBoxBool("Enable Blinn Phong", &_PointLight.EnableBlinnPhong);
                ECGui::CheckBoxBool("Enable Blinn PhongVisible", &_PointLight.visible);
                ECGui::CheckBoxBool("Affects World", &_PointLight.AffectsWorld);
                ECGui::SetColumns(1, NULL, true);

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
                auto& _SpotLight = engine->world.GetComponent<SpotLightComponent>(ID);

                ECGui::DrawTextWidget<const char*>("IntensityStrength", "");
                ECGui::DrawSliderFloatWidget("IntensityFloat", &_SpotLight.IntensityStrength, true, 0.f, 150.f);

                ECGui::DrawTextWidget<const char*>("Light Colour", "");
                ECGui::ColorPicker3("SLightColor", (float*)&_SpotLight.lightColor,
                    ImGuiColorEditFlags_PickerHueWheel | ImGuiColorEditFlags_DisplayRGB);

                ECGui::DrawTextWidget<const char*>("Attenuation Level", "");
                ECGui::DrawSliderIntWidget("PLightColourVec", &_SpotLight.AttenuationLevel, true, 0, 10);
                engine->LightManager.SetAttenuation(_SpotLight, _SpotLight.AttenuationLevel);

                ECGui::DrawTextWidget<const char*>("Light Ambient", "");
                ECGui::DrawSliderFloat3Widget("PLightAmbientVec", &_SpotLight.ambient, true, 0.0f, 1.0f);

                ECGui::DrawTextWidget<const char*>("Light Diffuse", "");
                ECGui::DrawSliderFloat3Widget("PLightDiffuseVec", &_SpotLight.diffuse, true, 0.0f, 1.0f);

                ECGui::DrawTextWidget<const char*>("Light Specular", "");
                ECGui::DrawSliderFloat3Widget("PLightSpecularVec", &_SpotLight.specular, true, 0.0f, 1.0f);

                ECGui::DrawTextWidget<const char*>("OuterCutOff", "");
                ECGui::DrawSliderFloatWidget("SLightOuterCutOffFloat", &_SpotLight.outerCutOff, true, 0.f, 50.0f);

                ECGui::DrawTextWidget<const char*>("CutOff", "");
                ECGui::DrawSliderFloatWidget("SLightCutOffFloat", &_SpotLight.cutOff, true, 0.f, (_SpotLight.outerCutOff - 5.0f));

                ECGui::DrawTextWidget<const char*>("Direction", "");
                ECGui::DrawSliderFloat3Widget("SLightDirectionVec", &_SpotLight.direction, true, 0.f, 150.f);

                ECGui::DrawTextWidget<const char*>("Constant", "");
                ECGui::DrawSliderFloatWidget("Constant", &_SpotLight.constant, true, 0.0f, 50.0f);

                ECGui::DrawTextWidget<const char*>("Linear", "");
                ECGui::DrawSliderFloatWidget("Linear", &_SpotLight.linear, true, 0.0f, 50.0f);

                ECGui::DrawTextWidget<const char*>("Quadratic", "");
                ECGui::DrawSliderFloatWidget("Quadratic", &_SpotLight.quadratic, true, 0.0f, 50.0f);

                ECGui::DrawTextWidget<const char*>("Radius", "");
                ECGui::DrawSliderFloatWidget("Radius", &_SpotLight.radius, true, 0.0f, 50.0f);

                ECGui::SetColumns(2, NULL, true);
                ECGui::DrawTextWidget<const char*>("Enable Blinn Phong", "");
                ECGui::DrawTextWidget<const char*>("Visible", "");
                ECGui::DrawTextWidget<const char*>("Affects World", "");
                ECGui::NextColumn();
                ECGui::CheckBoxBool("Enable Blinn Phong", &_SpotLight.EnableBlinnPhong);
                ECGui::CheckBoxBool("Enable Blinn PhongVisible", &_SpotLight.visible);
                ECGui::CheckBoxBool("Affects World", &_SpotLight.AffectsWorld);
                ECGui::SetColumns(1, NULL, true);
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
                auto& _DLight = engine->world.GetComponent<DirectionalLightComponent>(ID);
                ECGui::PushItemWidth(WindowSize_.getX() - 100.f);
                ECGui::DrawTextWidget<const char*>("Light Colour", "");
                ECGui::ColorPicker3("DLightColor", (float*)&_DLight.lightColor,
                    ImGuiColorEditFlags_PickerHueWheel | ImGuiColorEditFlags_DisplayRGB);

                ECGui::PushItemWidth(WindowSize_.getX());
                ECGui::DrawTextWidget<const char*>("DLight Ambient", "");
                ECGui::DrawSliderFloat3Widget("DLightAmbientVec", &_DLight.ambient, true, 0.0f, 1.0f);

                ECGui::DrawTextWidget<const char*>("DLight Diffuse", "");
                ECGui::DrawSliderFloat3Widget("DLightDiffuseVec", &_DLight.diffuse, true, 0.0f, 1.0f);

                ECGui::DrawTextWidget<const char*>("DLight Specular", "");
                ECGui::DrawSliderFloat3Widget("DLightSpecularVec", &_DLight.specular, true, 0.0f, 1.0f);
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
                auto& _RigidB = engine->world.GetComponent<RigidBodyComponent>(ID);

                ECGui::DrawTextWidget<const char*>("Enable Gravity", "");
                ECGui::CheckBoxBool("Rigid Body Enable Gravity", &_RigidB.enableGravity);

                ECGui::DrawTextWidget<const char*>("Forces", "");
                ECGui::DrawSliderFloat3Widget("Rigid Body Forces", &_RigidB.forces, true, 0.0f, 1.0f);

                ECGui::DrawTextWidget<const char*>("Velocity", "");
                ECGui::DrawSliderFloat3Widget("Rigid Body Velocity", &_RigidB.velocity, true, 0.0f, 1.0f);

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

                ECGui::DrawTextWidget<const char*>("Camera Speed", "");
                ECGui::DrawSliderFloatWidget("Camera Speed", &_Camera.cameraSpeed);

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
                auto& _Texture = engine->world.GetComponent<MaterialComponent>(ID);

                std::vector<std::string> _TextureVector = { "TT_UNASSIGNED","TT_2D","BasicPrimitives","TT_3D" };

                std::map<std::string, TextureType> _Map = { {"TT_UNASSIGNED",TextureType::TT_UNASSIGNED}, {"TT_2D",TextureType::TT_2D},
                                                            {"TT_3D",TextureType::TT_3D} };

                ComboListSettings settings = { "Texture Type" };

                ECGui::DrawTextWidget<const char*>("KEY ID: ", "");
                ECGui::InsertSameLine();
                //ECGui::DrawTextWidget<const char*>(std::to_string(_Texture.ID).c_str(), "");

                ECGui::DrawTextWidget<const char*>("Texture Type", "");
                ECGui::CreateComboList(settings, _TextureVector, _Texture.ComboIndex);
                _Texture.Type = _Map[_TextureVector[_Texture.ComboIndex]];

                ChangeTextureController(_Texture);
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

                ECGui::DrawTextWidget<const char*>("Transparency", "");
                ECGui::DrawSliderFloatWidget("Render Transparency", &_Render.transparency, true, 0.0f, 200.0f);


                //sceneloaded models
                //THIS IS WORK IN PROGRESS TESTING OUT FUNCITONALITIES AND ARE NOT MEANT TO BE IN THE FINAL
                //VERSION *NOT FOR FINAL VERSION* - TIAN YU
                std::string nameString = _Render.modelRef + " (Mesh Filter)";
                ECGui::PushStyleColor(ImGuiCol_Header, IM_COL32(0, 1, 1, 1));
                if (filter.PassFilter(nameString.c_str()) && ECGui::CreateCollapsingHeader(nameString.c_str()))
                {
                    ECGui::DrawTextWidget<const char*>("Mesh ", "");
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

                auto& _Material = engine->world.GetComponent<MaterialComponent>(ID);

                std::vector<std::string> _ModelVector = { "None","BasicPrimitives","Models3D" };

                std::map<std::string, MaterialModelType> _Map = { {"None",MaterialModelType::MT_NONE}, {"BasicPrimitives",MaterialModelType::MT_BASIC},
                                                            {"Models3D",MaterialModelType::MT_MODELS3D} };

                ComboListSettings settings = { "Model Type" };

                ECGui::DrawTextWidget<const char*>("Model Type", "");
                ECGui::CreateComboList(settings, _ModelVector, _Material.ComboIndex);
                _Material.Modeltype = _Map[_ModelVector[_Material.ComboIndex]];

                ECGui::DrawTextWidget<const char*>("Ambient", "");
                ECGui::DrawSliderFloat3Widget("Material Ambient", &_Material.ambient, true, 0.0f, 1.0f);

                ECGui::DrawTextWidget<const char*>("Diffuse", "");
                ECGui::DrawSliderFloat3Widget("Material Diffuse", &_Material.diffuse, true, 0.0f, 1.0f);

                ECGui::DrawTextWidget<const char*>("Specular", "");
                ECGui::DrawSliderFloat3Widget("Material Specular", &_Material.specular, true, 0.0f, 1.0f);

                ECGui::DrawTextWidget<const char*>("MaximumShininess", "");
                ECGui::DrawSliderFloatWidget("Material MaximumShininess", &_Material.MaximumShininess, true, 0.0f, 200.0f);

                //ECGui::DrawTextWidget<const char*>("Thickness", "");
                //ECGui::DrawSliderFloatWidget("Material Thickness", &_Material.Thickness, true, 0.0f, 200.0f);

                //ECGui::DrawTextWidget<const char*>("ScaleUp", "");
                //ECGui::DrawSliderFloatWidget("Material ScaleUp", &_Material.ScaleUp, true, 0.0f, 200.0f);

                ECGui::SetColumns(2, NULL, true);
                ECGui::DrawTextWidget<const char*>("Highlight", "");
                ECGui::NextColumn();
                ECGui::CheckBoxBool("Enable Blinn Phong", &_Material.Highlight);
                ECGui::SetColumns(1, NULL, true);
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
                auto& _Mesh = engine->world.GetComponent<MeshComponent>(ID);

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
                std::vector<std::string> _ModelInfoVector = { "MT_UNASSIGNED","MT_HUMAN","MT_ANIMAL","MT_HOUSE","MT_ENVIRONMENT" };

                std::map<std::string, ModelType> _Map = { {"MT_UNASSIGNED",ModelType::MT_UNASSIGNED}, {"MT_HUMAN",ModelType::MT_HUMAN},
                                                            {"MT_ANIMAL",ModelType::MT_ANIMAL},{"MT_HOUSE",ModelType::MT_HOUSE},
                                                            {"MT_ENVIRONMENT",ModelType::MT_ENVIRONMENT} };

                auto& _ModelInfo = engine->world.GetComponent<ModelComponent>(ID);

                ComboListSettings settings{ "Texture Type" };

                ECGui::DrawTextWidget<const char*>("Model Directory: ", "");
                ECGui::InsertSameLine();
                ECGui::DrawTextWidget<const char*>(_ModelInfo.Directory.c_str(), "");
                ECGui::DrawTextWidget<const char*>("Model Name: ", "");
                ECGui::InsertSameLine();
                ECGui::DrawTextWidget<const char*>(_ModelInfo.NameOfModel.c_str(), "");

                ECGui::DrawTextWidget<const char*>("Model Type", "");
                ECGui::CreateComboList(settings, _ModelInfoVector, _ModelInfo.ComboIndex);
                _ModelInfo.type = _Map[_ModelInfoVector[_ModelInfo.ComboIndex]];
            }
        }
        return false;
    }

    bool InspectorWindow::ShowScriptProperty(const char* name, Entity ID, ImGuiTextFilter& filter)
    {
        /*
        * FOR NICO
        * When script comp is up, just replace the entity com here with ur script,
        * and the vector with the vector of stdstrings in ur script com
        */
        if (engine->world.CheckComponent<ScriptComponent>(ID))
        {
            if (filter.PassFilter(name) && ECGui::CreateCollapsingHeader(name))
            {
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

                if (!IsRemovingScripts)
                {
                    if (ECGui::ButtonBool("Add Script"))
                    {
                        std::string fucknicosmother;
                        fucknicosmother.reserve(256);
                        scriptCom.scriptList.push_back({});
                        scriptCom.scriptList.back().scriptName = fucknicosmother;
                    }

                    ECGui::InsertSameLine();

                    if (ECGui::ButtonBool("Remove Script"))
                    {
                        IsRemovingScripts = true;
                    }
                }
                else
                {
                    if (ECGui::ButtonBool("Cancel Remove"))
                    {
                        IsRemovingScripts = false;
                    }
                }

                /*PopUpButtonSettings settings{ "Remove Script", "Removing script" };
                ECGui::BeginPopUpButtonList<void(std::vector<std::string>&)>(settings,
                    std::bind(&InspectorWindow::RemoveElementFromVectorStringList,
                    this, std::placeholders::_1), entCom.ScriptListComTest);*/
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
                static bool IsAudioPlaying = false;
                auto& audio = engine->world.GetComponent<AudioComponent>(ID);

                if (!IsAudioPlaying)
                {
                    if (audio.AudioPath.max_size() <= 0)
                    {
                        audio.AudioPath.reserve(256);
                    }

                    if (ECGui::ButtonBool("Play"))
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

                    if (ECGui::ButtonBool("Clear Audio"))
                    {
                        audio.AudioPath.clear();
                    }

                    ECGui::DrawTextWidget<const char*>("Volume: ", "");
                    ECGui::DrawSliderFloatWidget("Volume", &audio.Volume, true, 0.f, 1.0f);

                    // Pitch value, 0.5 = half pitch, 2.0 = double pitch, etc default = 1.0.
                    ECGui::DrawTextWidget<const char*>("Pitch: ", "");
                    ECGui::DrawSliderFloatWidget("Pitch", &audio.Pitch, true, 0.f, 2.0f);

                    // Relative speed of the song from 0.01 to 100.0. 0.5 = half speed, 
                    // 2.0 = double speed. Default = 1.0.
                    ECGui::DrawTextWidget<const char*>("Speed: ", "");
                    ECGui::DrawSliderFloatWidget("Speed", &audio.Speed, true, 0.f, 100.0f);

                    ECGui::CheckBoxBool("Loop", &audio.IsLooping, false);
                    ECGui::CheckBoxBool("Is3D", &audio.Is3D, false);

                    if (audio.Is3D)
                    {
                        // Inside cone angle, in degrees, from 0 to 360. 
                        // This is the angle within which the sound is at its normal volume. 
                        // Must not be greater than outsideconeangle. Default = 360.
                        ECGui::DrawTextWidget<const char*>("Inner Cone Angle: ", "");
                        ECGui::DrawSliderFloatWidget("InnerConeAngle", &audio.InnerConeAngle, true, 0.f, 360.0f);

                        // Outside cone angle, in degrees, from 0 to 360. 
                        // This is the angle outside of which the sound is at its outside volume.
                        // Must not be less than insideconeangle. Default = 360.
                        ECGui::DrawTextWidget<const char*>("Outer Cone Angle: ", "");
                        ECGui::DrawSliderFloatWidget("OuterConeAngle", &audio.OuterConeAngle, true, 0.f, 360.f);

                        // The volume of the sound outside the outside angle of the sound projection cone.
                        ECGui::DrawTextWidget<const char*>("Outer Volume: ", "");
                        ECGui::DrawSliderFloatWidget("OuterVolume", &audio.OuterVolume, true, 0.f, 1.f);

                        ECGui::InsertHorizontalLineSeperator();

                        // Increase the mindistance of a sound to make it 'louder' in a 3D world, 
                        // and decrease it to make it 'quieter' in a 3D world.
                        // Maxdistance is effectively obsolete unless you need the sound to stop 
                        // fading out at a certain point. 
                        // Do not adjust this from the default if you dont need to.
                        // Summary:
                        // Min -> min dist where it will start growing louder when getting closer
                        // Max -> max dist where it will stop fading
                        ECGui::DrawTextWidget<const char*>("Sound Attenuation: ", "");
                        ECGui::DrawSliderFloatWidget("Min", &audio.Min, false, 0.f, 10000.f);
                        ECGui::DrawSliderFloatWidget("Max", &audio.Max, false, 0.f, 10000.f);
                    }
                }
                else
                {
                    if (ECGui::ButtonBool("Stop"))
                    {
                        engine->audioManager.UnloadSound(audio.AudioPath);
                        IsAudioPlaying = false;
                        EDITOR_LOG_INFO("Stopping sound...");
                    }
                }
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

                auto& _Collision = engine->world.GetComponent<CollisionComponent>(ID);

                switch (_Collision.shape.shape)
                {
                case PxShapeType::Px_CUBE:
                    ECGui::DrawTextWidget<const char*>("CUBE ", "");
                    ECGui::InsertHorizontalLineSeperator();

                    ECGui::DrawTextWidget<const char*>("Hx: ", "");
                    ECGui::InsertSameLine();
                    snprintf(hxValue, 256, "%f", _Collision.shape.hx);
                    ECGui::DrawInputTextWidget("Hx: ", hxValue, 256, ImGuiInputTextFlags_EnterReturnsTrue);
                    _Collision.shape.hx = static_cast<float>(atof(hxValue));
                    ECGui::DrawTextWidget<const char*>("Hy: ", "");
                    ECGui::InsertSameLine();
                    snprintf(hyValue, 256, "%f", _Collision.shape.hy);
                    ECGui::DrawInputTextWidget("Hy: ", hyValue, 256, ImGuiInputTextFlags_EnterReturnsTrue);
                    _Collision.shape.hy = static_cast<float>(atof(hyValue));
                    ECGui::DrawTextWidget<const char*>("Hz: ", "");
                    ECGui::InsertSameLine();
                    snprintf(hzValue, 256, "%f", _Collision.shape.hz);
                    ECGui::DrawInputTextWidget("Hz: ", hzValue, 256, ImGuiInputTextFlags_EnterReturnsTrue);
                    _Collision.shape.hz = static_cast<float>(atof(hzValue));
                    break;
                case PxShapeType::Px_SPHERE:
                    ECGui::DrawTextWidget<const char*>("SPHERE ", "");
                    ECGui::InsertHorizontalLineSeperator();
                    ECGui::DrawTextWidget<const char*>("Radius: ", "");
                    ECGui::InsertSameLine();
                    snprintf(radiusValue, 256, "%f", _Collision.shape.radius);
                    ECGui::DrawInputTextWidget("Radius: ", radiusValue, 256, ImGuiInputTextFlags_EnterReturnsTrue);
                    _Collision.shape.radius = atof(radiusValue);
                    break;
                }
            }
        }
        return false;
    }

    bool InspectorWindow::ShowPrefebProperty(const char* name, Entity ID, ImGuiTextFilter& filter)
    {
        if (engine->world.CheckComponent<PrefabComponent>(ID))
        {
            ECGui::InsertHorizontalLineSeperator();
            ECGui::DrawTextWidget<const char*>("Prefeb: ", "");
            ECGui::InsertSameLine();
            if (ECGui::ButtonBool("Apply changes to all"))
            {
                //do something
            }
            if (ECGui::IsItemHovered())
            {
                ECGui::BeginToolTip();
                ECGui::PushTextWrapPos(ECGui::GetFontSize() * 35.0f);
                ECGui::TextUnformatted("Apply changes to all prefeb instances.");
                ECGui::PopTextWrapPos();
                ECGui::EndTooltip();
            }
            ECGui::InsertHorizontalLineSeperator();
        }
        return false;
    }

    void InspectorWindow::AddComponentsController(Entity ID)
    {
        //ImVec2 buttonSize = { 180,20 };
        if (ECGui::ButtonBool(("Add Component")))
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
        //ImVec2 buttonSize = { 180,20 };

        if (ECGui::ButtonBool(("Remove Component")))
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
                    }
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
                    }
                }
            }
        }

    }

    void InspectorWindow::ChangeTextureController(MaterialComponent& Item)
    {
        ImVec2 buttonSize = { 180,20 };
        ECGui::DrawTextWidget<const char*>("Texture  ", "");
        ECGui::InsertSameLine();

        if (ECGui::ButtonBool((Item.TextureRef.c_str()), buttonSize) || (ECGui::IsItemClicked(0) && ECGui::IsItemHovered()))
        {
            ECGui::OpenPopup("Texture Changer");
        }
        if (ECGui::BeginPopup("Texture Changer"))
        {
            ECGui::SetScrollY(5);
            ChildSettings settings{ "Texture Changer", ImVec2{ 250,250 } };
            ECGui::DrawChildWindow<void(MaterialComponent&)>(settings, std::bind(&InspectorWindow::TextureList,
                this, std::placeholders::_1), Item);

            ECGui::EndPopup();
        }
    }

    void InspectorWindow::TextureList(MaterialComponent& Item)
    {
        static ImGuiTextFilter AddComponentFilter;
        MaterialComponent FolderIcon;
        //FolderIcon.textureRef = Graphics::textures.find("FolderIcon")->first;
        //use image button to change the Graphics::models.find["models"]->find;
        std::vector<std::string> textureNames;
        textureNames.reserve(Graphics::textures.size());
        MaterialComponent icon = FolderIcon;
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
        ECGui::DrawSliderFloatWidget("Size: ", &thumbnaimsize,false, 10, 200);
        ECGui::SetColumns(columncount, NULL, true);
        for (auto it : Graphics::textures)
        {
            textureNames.push_back(it.first);
        }

        for (int i = 0; i < textureNames.size(); ++i)
        {

            FolderIcon.TextureRef = Graphics::textures.find(textureNames[i].c_str())->first;
            MaterialComponent icon = FolderIcon;

            if (AddComponentFilter.PassFilter(textureNames[i].c_str()))
            {
                ECGui::ImageButton((void*)(intptr_t)Graphics::FindTextures((icon).TextureRef).GetHandle(),
                    { thumbnaimsize,thumbnaimsize },
                    { 1,0 },
                    { 2,1 });

                if (ECGui::IsItemClicked(0) && ECGui::IsItemHovered())
                {
                    Item.TextureRef = Graphics::textures.find((textureNames[i].c_str()))->first;
                    AddComponentFilter.Clear();
                    ECGui::CloseCurrentPopup();
                }

                ECGui::DrawTextWrappedWidget(textureNames[i].c_str(),"");
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
        ECGui::DrawSliderFloatWidget("Size: ", &thumbnaimsize,false, 10, 200);

        ECGui::SetColumns(columncount, NULL, true);

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
                        Item.modelRef = Graphics::models.find((engine->AssimpManager.GetPrimitiveNames()[i].c_str()))->first;
                        AddComponentFilter.Clear();
                    }

                    ECGui::DrawTextWrappedWidget(engine->AssimpManager.GetPrimitiveNames()[i].c_str(),"");
                    ECGui::NextColumn();
                }

            }
        }
        else
        {
            for (int i = 0; i < engine->AssimpManager.GetMeshNames().size(); ++i)
            {

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

                    ECGui::DrawTextWrappedWidget(engine->AssimpManager.GetMeshNames()[i].c_str(),"");
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
            engine->audioManager.LoadSound(audioCom.AudioPath,
                audioCom.Is3D, audioCom.IsLooping, false);
            engine->audioManager.SetSpeed(audioCom.AudioPath, audioCom.Speed);
            engine->audioManager.Set3DConeSettings(audioCom.AudioPath,
                &audioCom.InnerConeAngle, &audioCom.OuterConeAngle, &audioCom.OuterVolume);
            engine->audioManager.Set3DMinMaxSettings(audioCom.AudioPath,
                audioCom.Min, audioCom.Max);
            audioCom.ChannelID = engine->audioManager.Play3DSounds(audioCom.AudioPath, trans.position,
                audioCom.Volume, audioCom.IsLooping);
        }
        else
        {
            engine->audioManager.LoadSound(audioCom.AudioPath, audioCom.Is3D,
                audioCom.IsLooping, false);
            engine->audioManager.SetSpeed(audioCom.AudioPath, audioCom.Speed);
            audioCom.ChannelID = engine->audioManager.Play2DSounds(audioCom.AudioPath, audioCom.Volume,
                audioCom.IsLooping);
        }
    }

    template <typename TComponents>
    void InspectorWindow::AddComponentsFeedback(const char* Components, const std::string& name, Entity ID, bool exist)
    {
        if (!exist)
        {
            engine->world.AddComponent(ID, TComponents{});
            std::string Comp = my_strcat(std::string{ Components }, " Added For ", name, ID, " Add Succeed");
            EDITOR_LOG_INFO(Comp.c_str());
        }
        else
        {
            std::string Comp = my_strcat(std::string{ Components }, " Already Exists in ", name, ID, " Add Failed");
            EDITOR_LOG_WARN(Comp.c_str());
        }
    }

    template <typename TComponents>
    void InspectorWindow::RemoveComponentsFeedback(const char* Components, const std::string& name, Entity ID, bool exist)
    {
        if (exist)
        {
            std::string Comp = my_strcat(std::string{ Components }, " Removed For ", name, ID, " Remove Succeed");
            EDITOR_LOG_INFO(Comp.c_str());
            engine->world.DestroyComponent<TComponents>(ID);
        }
        else
        {
            std::string Comp = my_strcat(std::string{ Components }, " Does Not Exists in ", name, ID, " Remove Failed");
            EDITOR_LOG_WARN(Comp.c_str());
        }
    }
}
