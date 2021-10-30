#include "pch.h"
#include "Hierarchy.h"
#include "ECS/ComponentManager/Components/EntityComponent.h"
#include "ECS/ComponentManager/Components/TransformComponent.h"
#include "ECS/ComponentManager/Components/ParentComponent.h"
#include "ECS/ComponentManager/Components/ChildComponent.h"
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
        WindowName = "Hierarchy " ICON_MDI_FILE_TREE;

        // For Geometry
        TagList_.push_back(std::vector<std::string>());
        // For Lights
        TagList_.push_back(std::vector<std::string>());
        // For Target Point
        TagList_.push_back(std::vector<std::string>());

        for (int index = 0; index != static_cast<int>(EntityType::ENT_LIGHT_POINT); ++index)
        {
            EntityType temps = static_cast<EntityType>(index);
            TagList_[0].push_back(lexical_cast_toStr<EntityType>(temps));
        }

        for (int index = static_cast<int>(EntityType::ENT_LIGHT_POINT);
            index != static_cast<int>(EntityType::ENT_GAMECAMERA); ++index)
        {
            EntityType temps = static_cast<EntityType>(index);
            TagList_[1].push_back(lexical_cast_toStr<EntityType>(temps));
        }

        EntityType tp = EntityType::ENT_TARGETPOINT;
        TagList_[2].push_back(lexical_cast_toStr<EntityType>(tp));
    }

    void HierarchyWindow::Unload()
    {
        EntTracker_.CurrEnt_.Clear();
        EntTracker_.PrevEnt_.Clear();
    }

    void HierarchyWindow::DrawImpl()
    {
        PopUpButtonSettings EntSettings{ "Add Entity" ICON_MDI_PENCIL_PLUS, "EntityCreationListBegin" };
        PopUpButtonSettings ModSettings{ "Create Model" ICON_MDI_DOMAIN_PLUS,  "ModelCreationListBegin" };
        ECGui::BeginPopUpButtonList<void()>(EntSettings, std::bind(&HierarchyWindow::ShowEntityCreationList, this));
        ECGui::InsertSameLine();
        ECGui::BeginPopUpButtonList<void()>(ModSettings, std::bind(&HierarchyWindow::ShowCreateModelList, this));
        ECGui::InsertHorizontalLineSeperator();

        ECGui::DrawTextWidget<size_t>("Entity Count", engine->editorManager->GetEntityListSize());
        ECGui::InsertHorizontalLineSeperator();

        static ImGuiTextFilter EntFilter;
        EntFilter.Draw();
        ECGui::InsertHorizontalLineSeperator();

        if (ECGui::BeginTreeNode("Default Scene"))
        {
            TrackEntitySelection(engine->editorManager->GetEntityListByConstRef(),
                EntTracker_.PrevEnt_, EntTracker_.CurrEnt_, EntFilter);
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

                entityName = my_strcat(entCom.Name, " ", list[index]);

                if (engine->world.CheckComponent<ParentComponent>(list[index]) && entCom.Parent.empty())
                {

                    auto& parent = engine->world.GetComponent<ParentComponent>(list[index]);

                    entityName = my_strcat(entCom.Name, " ", list[index]);

                    if (ECGui::CreateSelectableButton(entityName.c_str(), &entCom.IsActive))
                    {

                        if (curr.index == list[index])
                        {
                            entCom.IsActive = true;
                            engine->editorManager->SetGlobalIndex(index);                  
                            UpdateEntityTracker(engine->editorManager->GetEntityID(static_cast<int>(index)));
                            for (auto& it : parent.child)
                            {
                                auto& parent2Com = engine->world.GetComponent<EntityComponent>(it);

                                auto& tempent = engine->world.GetComponent<EntityComponent>(list[engine->editorManager->GetEntityIndex(it)]);
                                auto& entCom2 = engine->world.GetComponent<EntityComponent>(it);
                                std::string name = my_strcat(entCom2.Name, " ", list[engine->editorManager->GetEntityIndex(it)]);
                                EntitySelectionTracker ent = { it , name };
                                if (!exist(EntTracker_.CurrParent_, ent))
                                {
                                    EntTracker_.CurrParent_.emplace_back(ent);
                                }

                                ParentRecursion(parent2Com, it, list, prev, curr);

                                entCom2.IsActive = true;
                                UpdateEntityTracker(it);
                            }


                            continue;
                        }

                        if (!curr.name.empty())
                        {
                            prev.name = curr.name;
                            prev.index = curr.index;
                        }

                        if (!EntTracker_.CurrParent_.empty())
                        {
                            EntTracker_.PrevParent_ = EntTracker_.CurrParent_;
                            EntTracker_.CurrParent_.clear();
                        }

                        curr.name = entityName;
                        curr.index = list[index];

                        if (!isChild(entCom.Child, prev.index))
                        {
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

                                    for (auto& it : EntTracker_.PrevParent_)
                                    {
                                        auto& prevEntCom2 = engine->world.GetComponent<EntityComponent>(it.index);

                                        prevEntCom.IsActive = false;
                                    }
                                }
                            }
                        }
                        entCom.IsActive = true;
                        engine->editorManager->SetGlobalIndex(index);
                        UpdateEntityTracker(engine->editorManager->GetEntityID(static_cast<int>(index)));

                        for (auto& it : parent.child)
                        {       
                            auto& entCom2 = engine->world.GetComponent<EntityComponent>(it);
                            std::string name = my_strcat(entCom2.Name, " ", list[engine->editorManager->GetEntityIndex(it)]);
                            EntitySelectionTracker ent = { it , name };
                            if (!exist(EntTracker_.CurrParent_, ent))
                            {
                                EntTracker_.CurrParent_.emplace_back(ent);
                            }

                            entCom2.IsActive = true;
                            UpdateEntityTracker(it);
                        }

                        engine->editorManager->DragAndDropInst_.IndexPayloadSource("Entity",
                            static_cast<int>(index), PayloadSourceType::PST_ENTITY, curr.index);
                        engine->editorManager->DragAndDropInst_.IndexPayloadTarget("Entity",
                            static_cast<int>(index), entCom.IsActive);           
                    }

                    for (auto& it : parent.child)
                    {
                        auto& parent2Com = engine->world.GetComponent<EntityComponent>(it);
                        ParentRecursion(parent2Com, it, list, prev, curr);
                    }


                    engine->editorManager->DragAndDropInst_.IndexPayloadSource("Entity",
                        static_cast<int>(index), PayloadSourceType::PST_ENTITY, curr.index);
                }


                if (!engine->world.CheckComponent<ParentComponent>(list[index]) && !engine->world.CheckComponent<ChildComponent>(list[index]))
                {
                    if (ECGui::CreateSelectableButton(entityName.c_str(), &entCom.IsActive))
                    {
                        if (curr.index == list[index])
                        {
                            entCom.IsActive = true;
                            engine->editorManager->SetGlobalIndex(index);
                            UpdateEntityTracker(engine->editorManager->GetEntityID(static_cast<int>(index)));

                            continue;
                        }

                        if (!curr.name.empty())
                        {
                            prev.name = curr.name;
                            prev.index = curr.index;
                        }

                        if (!EntTracker_.CurrParent_.empty())
                        {
                            EntTracker_.PrevParent_ = EntTracker_.CurrParent_;
                            EntTracker_.CurrParent_.clear();
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

                                for (auto& it : EntTracker_.PrevParent_)
                                {
                                    auto& prevEntCom2 = engine->world.GetComponent<EntityComponent>(it.index);

                                    prevEntCom2.IsActive = false;
                                }
                            }
                        }
                    }
                    engine->editorManager->DragAndDropInst_.IndexPayloadSource("Entity",
                        static_cast<int>(index), PayloadSourceType::PST_ENTITY, curr.index);
                    engine->editorManager->DragAndDropInst_.IndexPayloadTarget("Entity",
                        static_cast<int>(index), entCom.IsActive);
                }

            }

        }
    }

    void HierarchyWindow::ParentRecursion(EntityComponent& entCom, Entity Num, const std::vector<Entity>& list, EntitySelectionTracker& prev, EntitySelectionTracker& curr)
    {
        std::string entityName{};
        if (engine->world.CheckComponent<ParentComponent>(Num))
        {
            auto& parent2 = engine->world.GetComponent<ParentComponent>(Num);

            entityName = my_strcat(entCom.Name, " ", Num);

            if (ECGui::CreateSelectableButton(entityName.c_str(), &entCom.IsActive))
            {

               // if (!curr.name.empty())
               // {
               //     prev.name = curr.name;
               //     prev.index = curr.index;
               // }
               //
               // curr.name = entityName;
               // curr.index = Num;
               //
               // if (!prev.name.empty() && curr.name != prev.name)
               // {
               //     bool deleted = true;
               //
               //     if (std::find(list.begin(), list.end(), prev.index) != list.end())
               //     {
               //         deleted = false;
               //     }
               //
               //     if (!deleted)
               //     {
               //         auto& prevEntCom = engine->world.GetComponent<EntityComponent>(prev.index);
               //         prevEntCom.IsActive = false;
               //     }
               // }
               //
               // entCom.IsActive = true;
               // engine->editorManager->SetGlobalIndex(engine->editorManager->GetEntityIndex(Num));
               // UpdateEntityTracker(Num);
               //
               // for (auto& it : parent2.child)
               // {
               //
               //     auto& parent2Com = engine->world.GetComponent<EntityComponent>(it);
               //
               //     ParentRecursion(parent2Com, it, list, prev, curr);
               //
               // }

            }
        }
        else
        {
            entityName = my_strcat(entCom.Name, " ", Num);

            if (ECGui::CreateSelectableButton(entityName.c_str(), &entCom.IsActive))
            {

                //if (!curr.name.empty())
                //{
                //    prev.name = curr.name;
                //    prev.index = curr.index;
                //}
                //
                //curr.name = entityName;
                //curr.index = Num;
                //
                //if (!prev.name.empty() && curr.name != prev.name)
                //{
                //    bool deleted = true;
                //
                //    if (std::find(list.begin(), list.end(), prev.index) != list.end())
                //    {
                //        deleted = false;
                //    }
                //
                //    if (!deleted)
                //    {
                //        auto& prevEntCom = engine->world.GetComponent<EntityComponent>(prev.index);
                //        prevEntCom.IsActive = false;
                //    }
                //}
                //
                entCom.IsActive = true;
                engine->editorManager->SetGlobalIndex(engine->editorManager->GetEntityIndex(Num));
                UpdateEntityTracker(Num);
                engine->editorManager->DragAndDropInst_.IndexPayloadSource("Entity",
                    engine->editorManager->GetEntityIndex(Num), PayloadSourceType::PST_ENTITY, curr.index);
                engine->editorManager->DragAndDropInst_.IndexPayloadTarget("Entity",
                    engine->editorManager->GetEntityIndex(Num), entCom.IsActive);
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
            case 2:
            {
                bool selected = false;

                if (ECGui::BeginTreeNode("Other Actors"))
                {
                    if (ECGui::CreateSelectableButton(TagList_[i][0].c_str(), &selected))
                    {
                        Entity ID = engine->editorManager->CreateDefaultEntity(lexical_cast_toEnum<EntityType>(TagList_[i][0]));
                        engine->gAI.AddTargetPointEntity(ID);
                        UpdateEntityTracker(ID);
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
        if (engine->world.CheckComponent<MaterialComponent>(ID))
            engine->MaterialManager.HighlightClick(ID);

        if (ID != EntTracker_.CurrEnt_.index)
        {
            auto& entCom = engine->world.GetComponent<EntityComponent>(ID);

            if (engine->world.CheckComponent<EntityComponent>(EntTracker_.CurrEnt_.index))
            {
                auto& prevEntCom = engine->world.GetComponent<EntityComponent>(EntTracker_.CurrEnt_.index);
                prevEntCom.IsActive = false;
                EntTracker_.PrevEnt_.name = EntTracker_.CurrEnt_.name;
                EntTracker_.PrevEnt_.index = EntTracker_.CurrEnt_.index;

                if (engine->world.CheckComponent<MaterialComponent>(EntTracker_.CurrEnt_.index))
                    engine->MaterialManager.UnHighlight(EntTracker_.CurrEnt_.index);
            }

            EntTracker_.CurrEnt_.name = my_strcat(entCom.Name, " ", ID);
            EntTracker_.CurrEnt_.index = ID;
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
            Parent.TreeactiveFlag |= ImGuiTreeNodeFlags_Selected;
        }
        else
        {
            Parent.TreeactiveFlag = ImGuiTreeNodeFlags_OpenOnDoubleClick;
        }
    }
    void HierarchyWindow::ShowCreateModelList()
    {
        for (size_t i = 0; i < engine->AssimpManager.GetMeshNames().size(); ++i)
        {
            bool selected = false;

            if (ECGui::CreateSelectableButton(engine->AssimpManager.GetMeshNames()[i].c_str(), &selected))
            {
                Entity ID = engine->AssimpManager.CreateModel(0, engine->AssimpManager.GetMeshNames()[i]);
                UpdateEntityTracker(ID);
            }
        }
    }

    bool HierarchyWindow::isChild(std::vector<Entity> vec, const Entity& elem)
    {
        bool result = false;
        for (auto& x : vec)
        {
            if (x == elem)
            {
                result = true;
                break;
            }
        }
        return result;
    }

    bool HierarchyWindow::exist(std::vector<EntitySelectionTracker> vec, const EntitySelectionTracker& elem)
    {
        bool result = false;
        for (auto& x : vec)
        {
            if (x.index == elem.index)
            {
                result = true;
                break;
            }
        }
        return result;
    }


}