#include "pch.h"
#include "Hierarchy.h"
#include "ECS/ComponentManager/Components/EntityComponent.h"
#include "ECS/ComponentManager/Components/TransformComponent.h"

namespace Eclipse
{
    void HierarchyWindow::Update()
    {
        if (IsVisible)
            ECGui::DrawMainWindow<void()>(WindowName, std::bind(&HierarchyWindow::DrawImpl, this));
    }

    void HierarchyWindow::Init()
    {
        Type = EditorWindowType::EWT_HIERARCHY;
        WindowName = "Hierarchy";

        // For Geometry
        TagList_.push_back(std::vector<std::string>());
        // For Lights
        TagList_.push_back(std::vector<std::string>());

        for (int index = 0; index != static_cast<int>(EntityType::ENT_LIGHT_POINT); ++index)
        {
            EntityType temp = static_cast<EntityType>(index);
            TagList_[0].push_back(lexical_cast_toStr<EntityType>(temp));
        }

        for (int index = static_cast<int>(EntityType::ENT_LIGHT_POINT);
            index != static_cast<int>(EntityType::ENT_GAMECAMERA); ++index)
        {
            EntityType temp = static_cast<EntityType>(index);
            TagList_[1].push_back(lexical_cast_toStr<EntityType>(temp));
        }
    }

    void HierarchyWindow::Unload()
    {
        CurrEnt_.Clear();
        PrevEnt_.Clear();
    }

    void HierarchyWindow::DrawImpl()
    {
        ////////////////////
        //TODO for darren
        if (engine->AssimpManager.CheckCompilers())
        {
            ComboListSettings settingsss = { "Models Testing" };
            std::vector<std::string> hiDarrenVector = engine->AssimpManager.GetMeshNames();
            static size_t comboindex = 0;
            ECGui::CreateComboList(settingsss, hiDarrenVector, comboindex);

            bool selected = false;

            if (ECGui::CreateSelectableButton(hiDarrenVector[comboindex].c_str(), &selected))
            {
                auto MeshID = engine->editorManager->CreateDefaultEntity(EntityType::ENT_UNASSIGNED);
                engine->AssimpManager.CreateModel(MeshID, hiDarrenVector[comboindex].c_str());
            }
        }
        /// ///////////////////

        PopUpButtonSettings settings{ "Add Entity", "EntityCreationListBegin" };
        ECGui::BeginPopUpButtonList<void()>(settings, std::bind(&HierarchyWindow::ShowEntityCreationList, this));
        ECGui::InsertHorizontalLineSeperator();

        ECGui::DrawTextWidget<size_t>("Entity Count", engine->editorManager->GetEntityListSize());
        ECGui::InsertHorizontalLineSeperator();

        static ImGuiTextFilter EntFilter;
        EntFilter.Draw();
        ECGui::InsertHorizontalLineSeperator();

        if (ECGui::BeginTreeNode("Default Scene"))
        {
            TrackEntitySelection(engine->editorManager->GetEntityListByConstRef(),
                PrevEnt_, CurrEnt_, EntFilter);

            ECGui::EndTreeNode();
        }
    }

    void HierarchyWindow::TrackEntitySelection(const std::vector<Entity>& list, EntitySelectionTracker& prev,
        EntitySelectionTracker& curr, ImGuiTextFilter& filter)
    {
        std::string entityName{};

        for (size_t index = 0; index < list.size(); ++index)
        {
            auto& entCom = engine->world.GetComponent<EntityComponent>(list[index]);

            if (filter.PassFilter(entCom.Name.c_str()))
            {
                temp = 0;
                entityName = my_strcat(entCom.Name, " ", list[index]);

                if (!entCom.IsAChild && !entCom.Child.empty())
                {
                    TreeNodeRecursion(entityName, entCom, prev, curr, index);
                }

                if (entCom.Child.empty() && !entCom.IsAChild
                    && ECGui::CreateSelectableButton(entityName.c_str(), &entCom.IsActive))
                {
                    if (curr.index == list[index])
                    {
                        entCom.IsActive = true;
                        engine->editorManager->SetGlobalIndex(index);
                        continue;
                    }

                    if (!curr.name.empty())
                    {
                        prev.name = curr.name;
                        prev.index = curr.index;
                    }

                    curr.name = entityName;
                    curr.index = list[index];

                    if (!prev.name.empty() && curr.name != prev.name)
                    {
                        bool deleted = true;

                        if (std::find(list.begin(), list.end(), prev.index) != list.end())
                        {
                            deleted = false;
                        }

                        if (!deleted)
                        {
                            auto& prevEntCom = engine->world.GetComponent<EntityComponent>(prev.index);
                            prevEntCom.IsActive = false;
                        }
                    }
                    engine->editorManager->SetGlobalIndex(index);
                    UpdateEntityTracker(engine->editorManager->GetEntityID(static_cast<int>(index)));

                }
                if (!entCom.IsAChild)
                {
                    engine->editorManager->DragAndDropInst_.IndexPayloadSource("HierarchyIndexEdit",
                        static_cast<int>(index));
                    engine->editorManager->DragAndDropInst_.IndexPayloadTarget("HierarchyIndexEdit",
                        static_cast<int>(index), entCom.IsActive);
                }

                engine->editorManager->DragAndDropInst_.IndexPayloadSource("PrefabGeneration", static_cast<int>(curr.index), PayloadSourceType::PST_ENTITY);
            }
        }
    }

    void HierarchyWindow::ShowEntityCreationList()
    {
        for (size_t i = 0; i < TagList_.size(); ++i)
        {
            switch (i)
            {
            case 0:
            {
                if (ECGui::BeginTreeNode("Geometry"))
                {
                    for (size_t j = 0; j < TagList_[i].size(); ++j)
                    {
                        bool selected = false;

                        if (ECGui::CreateSelectableButton(TagList_[i][j].c_str(), &selected))
                        {
                            Entity ID = engine->editorManager->CreateDefaultEntity(lexical_cast_toEnum<EntityType>(TagList_[i][j]));
                            engine->GraphicsManager.CreatePrimitives(ID, static_cast<int>(i * TagList_.size() + j));
                            UpdateEntityTracker(ID);
                        }
                    }

                    ECGui::EndTreeNode();
                }

                break;
            }
            case 1:
            {
                if (ECGui::BeginTreeNode("Lights"))
                {
                    for (size_t j = 0; j < TagList_[i].size(); ++j)
                    {
                        bool selected = false;

                        if (ECGui::CreateSelectableButton(TagList_[i][j].c_str(), &selected))
                        {
                            Entity ID = engine->editorManager->CreateDefaultEntity(lexical_cast_toEnum<EntityType>(TagList_[i][j]));
                            engine->GraphicsManager.CreatePrimitives(ID, static_cast<int>(i * TagList_[i - 1].size() + j));
                            UpdateEntityTracker(ID);
                        }
                    }

                    ECGui::EndTreeNode();
                }
                break;
            }
            default:
                break;
            }
        }
    }
    void HierarchyWindow::UpdateEntityTracker(Entity ID)
    {
        if (ID != CurrEnt_.index)
        {
            auto& entCom = engine->world.GetComponent<EntityComponent>(ID);

            if (engine->world.CheckComponent<EntityComponent>(CurrEnt_.index))
            {
                auto& prevEntCom = engine->world.GetComponent<EntityComponent>(CurrEnt_.index);
                prevEntCom.IsActive = false;
                PrevEnt_.name = CurrEnt_.name;
                PrevEnt_.index = CurrEnt_.index;
            }

            CurrEnt_.name = my_strcat(entCom.Name, " ", ID);
            CurrEnt_.index = ID;
            entCom.IsActive = true;
        }
    }

    size_t HierarchyWindow::GetEntityGlobalIndex(size_t data)
    {
        auto it = std::find(engine->editorManager->GetEntityListByConstRef().begin(), engine->editorManager->GetEntityListByConstRef().end(), data);

        if (it != engine->editorManager->GetEntityListByConstRef().end())
        {
            size_t index = it - engine->editorManager->GetEntityListByConstRef().begin();

            return index;
        }
        return 0;
    }

    std::string HierarchyWindow::GetEntityComponentEntityNumber(std::string EntityName)
    {
        size_t bracked = EntityName.find_last_of(" ");

        if (bracked == std::string::npos)
        {
            return EntityName;
        }

        return EntityName.substr(bracked + 1);
    }

    size_t HierarchyWindow::ConvertEntityStringtoNumber(std::string EntityNumber)
    {
        size_t result;
        std::stringstream sstream(EntityNumber);
        sstream >> result;
        return result;
    }

    void HierarchyWindow::HightLightParentAndChild(EntityComponent& Parent)
    {
        if (Parent.IsActive)
        {
            if (!Parent.Child.empty())
            {
                temp |= ImGuiTreeNodeFlags_Selected;
                for (auto& it : Parent.Child)
                {
                    auto& child = engine->world.GetComponent<EntityComponent>(it);
                    child.IsActive = true;
                }
            }
        }
        else
        {
            for (auto& it : Parent.Child)
            {
                auto& child = engine->world.GetComponent<EntityComponent>(it);
                child.IsActive = false;
            }
        }

    }

    void HierarchyWindow::TreeNodeRecursion(std::string parent, EntityComponent& entCom, EntitySelectionTracker& prev, EntitySelectionTracker& curr, size_t index)
    {
        if (entCom.IsActive)
        {
            temp |= ImGuiTreeNodeFlags_Selected;
        }

        HightLightParentAndChild(entCom);

        bool nodeOpen = ImGui::TreeNodeEx(parent.c_str(), temp);

        if (nodeOpen)
        {
            for (size_t i = 0; i < entCom.Child.size(); ++i)
            {
                auto& entCom1 = engine->world.GetComponent<EntityComponent>(entCom.Child[i]);
                std::string name = my_strcat(entCom1.Name, " ", entCom.Child[i]);

                if (!entCom1.Child.empty())
                {
                    TreeNodeRecursion(name, entCom1, prev, curr, index);
                }
                else
                {
                    if (ECGui::CreateSelectableButton(name.c_str(), &entCom1.IsActive))
                    {
                        if (curr.index == entCom.Child[i])
                        {
                            entCom.IsActive = true;
                            engine->editorManager->SetGlobalIndex(GetEntityGlobalIndex(curr.index));
                            continue;
                        }

                        if (!curr.name.empty())
                        {
                            prev.name = curr.name;
                            prev.index = curr.index;
                        }

                        curr.name = name;
                        curr.index = entCom.Child[i];

                        if (!prev.name.empty() && curr.name != prev.name)
                        {
                            bool deleted = true;

                            if (std::find(entCom.Child.begin(), entCom.Child.end(), prev.index) != entCom.Child.end())
                                deleted = false;

                            if (!deleted)
                            {
                                auto& prevEntCom = engine->world.GetComponent<EntityComponent>(prev.index);
                                prevEntCom.IsActive = false;
                            }
                        }
                        entCom.IsActive = false;
                        engine->editorManager->SetGlobalIndex(GetEntityGlobalIndex(curr.index));
                        UpdateEntityTracker(engine->editorManager->GetEntityID(GetEntityGlobalIndex(static_cast<size_t>(curr.index))));
                    }

                }
            }
            ECGui::EndTreeNode();
        }
        else
        {
            if (ImGui::IsItemClicked(0))
            {
                size_t currIndex = ConvertEntityStringtoNumber(GetEntityComponentEntityNumber(parent));
                engine->editorManager->SetGlobalIndex(engine->editorManager->GetEntityIndex(static_cast<Entity>(currIndex)));
                UpdateEntityTracker(engine->editorManager->GetEntityID(engine->editorManager->GetEntityIndex(static_cast<Entity>(currIndex))));
            }
        }
    }
}
