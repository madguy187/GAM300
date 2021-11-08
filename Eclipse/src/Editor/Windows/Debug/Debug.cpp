#include "pch.h"
#include "Debug.h"
#include "../Scene/SceneView.h"
#include "../Inspector/Inspector.h"

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
        WindowName = "Project Settings " ICON_MDI_ACCOUNT_COG;
        IsVisible = false;

        SelectionList.push_back({ "Graphics", true });
        SelectionList.push_back({ "Input Manager", false });
        SelectionList.push_back({ "Tags & Layers", false });

        if (!engine->szManager.LoadLayersConfigFile(LayerList, LayerListSize))
            DefaultLayerInit();

        // Deserialize the map here into KeyMappings
        //engine->InputManager->InputCompiler_.Load();
        //engine->InputManager->InputCompiler_.ProvideContainer(KeyMappings);
    }

    void DebugWindow::Unload()
    {
    }

    void DebugWindow::DrawImpl()
    {
        WindowSize.setX(ECGui::GetWindowSize().x);
        WindowSize.setY(ECGui::GetWindowSize().y);

        static ImGuiTextFilter SettingsFilter;
        ECGui::PushItemWidth(WindowSize.getX());
        SettingsFilter.Draw(EMPTY_STRING);

        ChildSettings settings{ "Settings Selection",
            ImVec2{ WindowSize.getX() / 4.0f, WindowSize.getY() * 0.89f}, true };
        ECGui::DrawChildWindow<void(ImGuiTextFilter&)>(settings, std::bind(&DebugWindow::RunSettingsSelection,
            this, std::placeholders::_1), SettingsFilter);

        ECGui::InsertSameLine();

        ChildSettings settings2{ "Settings Details",
            ImVec2{ WindowSize.getX() * (2.98f / 4.0f), WindowSize.getY() * 0.89f}, true };
        ECGui::DrawChildWindow<void()>(settings2,
            std::bind(&DebugWindow::RunSettingsDetails, this));
    }

    void DebugWindow::RunSettingsSelection(ImGuiTextFilter& filter)
    {
        for (size_t i = 0; i < SelectionList.size(); ++i)
        {
            if (filter.PassFilter(SelectionList[i].name.c_str()))
            {
                if (ECGui::CreateSelectableButton(SelectionList[i].name.c_str(), &SelectionList[i].active))
                {
                    if (CurrentSelection != SelectionList[i].name)
                        UpdateSelectionTrackerID(SelectionList[i], static_cast<int>(i));
                    else
                        SelectionList[i].active = true;
                }
            }
        }
    }

    void DebugWindow::RunSettingsDetails()
    {
        ImGui::SetWindowFontScale(1.5f);
        ECGui::DrawTextWidget<const char*>(CurrentSelection.c_str(), EMPTY_STRING);
        ImGui::SetWindowFontScale(1.0f);
        ECGui::InsertHorizontalLineSeperator();

        if (CurrentSelection == "Graphics")
        {
            OnGraphicsUpdate();
        }
        else if (CurrentSelection == "Input Manager")
        {
            OnInputUpdate();
        }
        else if (CurrentSelection == "Tags & Layers")
        {
            OnTagsAndLayersUpdate();
        }
    }

    void DebugWindow::UpdateSelectionTrackerID(sSelection& s, int index)
    {
        CurrentSelection = s.name;
        s.active = true;
        SelectionList[PreviousIndex].active = false;
        PreviousIndex = index;
    }

    void DebugWindow::AddInputController()
    {
        if (ECGui::ButtonBool("Add Input"))
        {
            ECGui::OpenPopup("Add Input");
        }

        if (ECGui::BeginPopup("Add Input"))
        {
            ECGui::SetScrollY(5);

            ChildSettings settings{ "Add Input", ImVec2{ 250.0f, 200.0f } };
            ECGui::DrawChildWindow<void()>(settings,
                std::bind(&DebugWindow::ShowInputList, this));

            ECGui::EndPopup();
        }
    }

    void DebugWindow::ShowInputList()
    {
        static ImGuiTextFilter AddInputFilter;
        AddInputFilter.Draw(EMPTY_STRING, 220.0f);

        for (const auto& str : engine->InputManager->GetAllKeys())
        {
            if (AddInputFilter.PassFilter(str.c_str()))
            {
                if (ECGui::ButtonBool(str.c_str(), ImVec2(220.0f, 0.0f)))
                {
                    std::string temp;
                    temp.reserve(256);
                    KeyMappings[str] = temp;
                    AddInputFilter.Clear();
                    ECGui::CloseCurrentPopup();
                }
            }
        }
    }

    void DebugWindow::DefaultLayerInit()
    {
        for (int i = 0; i < MAX_LAYER_SIZE; ++i)
        {
            std::string name;
            name.reserve(256);

            switch (i)
            {
            case 0: name = "Default"; break;
            case 1: name = "Nothing"; break;
            case 2: name = "Everything"; break;
            default: break;
            }

            LayerList[i] = name;
        }
    }

    void DebugWindow::OnGraphicsUpdate()
    {
        ECGui::CheckBoxBool("Draw Grid", &engine->GridManager->Visible, false);
        ECGui::InsertSameLine();
        ECGui::CheckBoxBool("PostProcess", &engine->gFrameBufferManager->PostProcess->AllowPostProcess, false);
        ECGui::InsertSameLine();
        ECGui::CheckBoxBool("Draw Normals", &engine->GraphicsManager.VisualizeNormalVectors, false);
        ECGui::InsertSameLine();
        ECGui::CheckBoxBool("Draw DebugShapes", &engine->gDebugDrawManager->Visible, false);

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

            if (engine->gFrameBufferManager->PostProcess->AllowPostProcess)
            {
                std::vector<std::string> Methods = { "NONE" , "INVERSE" , "GREYSCALE" ,"KERNEL" , "BLUR" , "SOBEL" , "BLEEDING"};
                ComboListSettings settingsss = { "PostProcess Methods" };
                static size_t comboindex = 0;
                ECGui::DrawTextWidget<const char*>("PostProcess Types:", EMPTY_STRING);
                ECGui::CreateComboList(settingsss, Methods, comboindex);
                engine->gFrameBufferManager->PostProcess->PPType_ = static_cast<FrameBuffer::PostProcessType>(comboindex);
            }

        if (engine->GraphicsManager.VisualizeNormalVectors)
        {
            ECGui::DrawTextWidget<const char*>("Normals Length:", EMPTY_STRING);
            ECGui::DrawSliderFloatWidget("Normals Length", &engine->GraphicsManager.Magnitude, true, 0.2f, 1.0f);
        }
    }

    void DebugWindow::OnInputUpdate()
    {
        ECGui::SetColumns(2, nullptr, false);
        ECGui::SetColumnOffset(1, 140);

        for (auto& pair : KeyMappings)
        {
            ECGui::DrawTextWidget<const char*>(pair.first.c_str(), EMPTY_STRING);
            ECGui::NextColumn();

            ECGui::DrawInputTextHintWidget(pair.first.c_str(), "Enter KeyCode",
                const_cast<char*>(pair.second.c_str()),
                256, true, ImGuiInputTextFlags_EnterReturnsTrue);

            ECGui::InsertSameLine();

            if (ECGui::ButtonBool(my_strcat("Remove", " ", pair.first).c_str()))
            {
                ToBeRemoved = pair.first;
            }

            ECGui::NextColumn();
        }

        ECGui::InsertHorizontalLineSeperator();
        AddInputController();

        ECGui::InsertSameLine();

        if (ECGui::ButtonBool("Save"))
        {
            engine->InputManager->InputCompiler_.ReceiveMapping(KeyMappings);
            engine->InputManager->InputCompiler_.Write();
        }

        if (!ToBeRemoved.empty())
        {
            KeyMappings.erase(ToBeRemoved);
            ToBeRemoved.clear();
        }
    }

    void DebugWindow::OnTagsAndLayersUpdate()
    {
        if (ECGui::BeginTreeNode(TagsAndLayersHeaders[0]))
        {
            LayerListSize = 0;
            ECGui::SetColumns(2, nullptr, false);
            ECGui::SetColumnOffset(1, 140);

            for (auto& pair : LayerList)
            {
                if (pair.first == 0 || pair.first == 1 || pair.first == 2)
                {
                    ECGui::DrawTextWidget<const char*>(my_strcat("Built-In Layer ", pair.first).c_str(), EMPTY_STRING);
                    ECGui::NextColumn();

                    ECGui::DrawInputTextHintWidget(lexical_cast<std::string>(pair.first).c_str(), const_cast<char*>(pair.second.c_str()),
                        EMPTY_STRING, 0, true, ImGuiInputTextFlags_ReadOnly);

                    ECGui::NextColumn();

                    continue;
                }

                ECGui::DrawTextWidget<const char*>(my_strcat("User Layer ", pair.first).c_str(), EMPTY_STRING);
                ECGui::NextColumn();

                ECGui::DrawInputTextHintWidget(lexical_cast<std::string>(pair.first).c_str(), "Enter Layer Name",
                    const_cast<char*>(pair.second.c_str()),
                    256, true, ImGuiInputTextFlags_EnterReturnsTrue);

                if (strcmp(pair.second.c_str(), EMPTY_STRING))
                    LayerListSize++;

                ECGui::NextColumn();
            }

            if (ECGui::ButtonBool("Save"))
            {
                auto* insp = engine->editorManager->GetEditorWindow<InspectorWindow>();
                insp->SetCollisionLayerTracker(LayerList);
                engine->szManager.SaveLayersConfigFile(LayerList, LayerListSize);

                EDITOR_LOG_INFO("Layer configuration successfully saved.");
            }

            ECGui::EndTreeNode();
        }
    }

    const std::unordered_map<int, std::string>& DebugWindow::GetLayerList()
    {
        return LayerList;
    }

    size_t DebugWindow::GetLayerListSize() const
    {
        return LayerListSize;
    }

    const std::string& DebugWindow::GetStringLayer(int index)
    {
        return LayerList[index];
    }

    int DebugWindow::GetIndexLayer(const std::string& str)
    {
        for (const auto& [key, val] : LayerList)
        {
            if (!strcmp(str.c_str(), val.c_str()))
                return key;
        }

        return -1;
    }

    void DebugWindow::SetLayerListSize(size_t size)
    {
        LayerListSize = size;
    }
}
