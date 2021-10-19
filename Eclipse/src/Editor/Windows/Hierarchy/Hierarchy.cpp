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
        CurrEnt_.Clear();
        PrevEnt_.Clear();
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
                entityName = my_strcat(entCom.Name, " ", list[index]);

                if (!entCom.Child.empty())
                {
                    TreeNodeRecursion(entityName, entCom, prev, curr, index);
                }

                if (entCom.Child.empty() && !entCom.IsAChild)
                {
                    if (ECGui::CreateSelectableButton(entityName.c_str(), &entCom.IsActive))
                    {
                        for (size_t index = 0; index < list.size(); ++index)
                        {
                            auto& entComn = engine->world.GetComponent<EntityComponent>(list[index]);

                            if (entComn.Name != entCom.Name)
                            {
                                entComn.IsActive = false;
                                HightLightParentAndChild(entComn);
                            }

                        }
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
                }

                    engine->editorManager->DragAndDropInst_.IndexPayloadSource("Entity",
                        static_cast<int>(index), PayloadSourceType::PST_ENTITY, curr.index);
                    engine->editorManager->DragAndDropInst_.IndexPayloadTarget("Entity",
                        static_cast<int>(index), entCom.IsActive);
                }
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

        if (ID != CurrEnt_.index)
        {
            auto& entCom = engine->world.GetComponent<EntityComponent>(ID);

            if (engine->world.CheckComponent<EntityComponent>(CurrEnt_.index))
            {
                auto& prevEntCom = engine->world.GetComponent<EntityComponent>(CurrEnt_.index);
                prevEntCom.IsActive = false;
                PrevEnt_.name = CurrEnt_.name;
                PrevEnt_.index = CurrEnt_.index;

                if (engine->world.CheckComponent<MaterialComponent>(CurrEnt_.index))
                    engine->MaterialManager.UnHighlight(CurrEnt_.index);
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
            Parent.TreeactiveFlag |= ImGuiTreeNodeFlags_Selected;
        }
        else
        {
            Parent.TreeactiveFlag = ImGuiTreeNodeFlags_OpenOnDoubleClick;
        }
    }

    void HierarchyWindow::TreeNodeRecursion(std::string parent, EntityComponent& entCom, EntitySelectionTracker& prev, EntitySelectionTracker& curr, size_t index)
    {

        bool nodeOpen = ImGui::TreeNodeEx(parent.c_str(), entCom.TreeactiveFlag);

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
                        size_t currIndex = ConvertEntityStringtoNumber(GetEntityComponentEntityNumber(name));

                        //curr.index = entity id
                        entCom.IsActive = false;
                        entCom1.IsActive = true;
                        HightLightParentAndChild(entCom);
                        HightLightParentAndChild(entCom1);

                        engine->editorManager->SetGlobalIndex(GetEntityGlobalIndex(currIndex));
                        UpdateEntityTracker(engine->editorManager->GetEntityID(static_cast<int>(GetEntityGlobalIndex(static_cast<size_t>(currIndex)))));

                    }

                    engine->editorManager->DragAndDropInst_.IndexPayloadTarget("Entity",
                       GetListPos(ConvertEntityStringtoNumber(GetEntityComponentEntityNumber(name))), entCom.IsActive);
                }

            }
            ECGui::EndTreeNode();
        }

        //auto& prevCom = engine->world.GetComponent<EntityComponent>(prev.index);
        //bool nodeOpen = ImGui::TreeNodeEx(parent.c_str(), entCom.TreeactiveFlag);
        //if (nodeOpen)
        //{
        //    size_t currIndex = ConvertEntityStringtoNumber(GetEntityComponentEntityNumber(parent.c_str()));

        //    prevCom.IsActive = false;

        //    if (ECGui::IsMouseClicked(0))
        //    {
        //        //curr.index = entity id
        //        engine->editorManager->SetGlobalIndex(static_cast<size_t>(GetListPos(currIndex)));
        //        UpdateEntityTracker(engine->editorManager->GetEntityID(GetListPos(currIndex)));
        //
        //    }

        //    HightLightParentAndChild(prevCom);
        //    HightLightParentAndChild(entCom);

        //    for (size_t i = 0; i < entCom.Child.size(); ++i)
        //    {
        //        auto& entCom1 = engine->world.GetComponent<EntityComponent>(entCom.Child[i]);
        //        std::string name = my_strcat(entCom1.Name, " ", entCom.Child[i]);
        //
        //        bool node = ImGui::TreeNodeEx(name.c_str(), entCom1.TreeactiveFlag);
        //    
        //        if (node)
        //        {
        //            size_t currIndex = ConvertEntityStringtoNumber(GetEntityComponentEntityNumber(name));
        //            if (ECGui::IsMouseClicked(0))
        //            {
        //                //curr.index = entity id
        //                entCom.IsActive = false;
        //                entCom1.IsActive = true;
        //                HightLightParentAndChild(entCom);
        //                HightLightParentAndChild(entCom1);
        //                engine->editorManager->SetGlobalIndex(static_cast<size_t>(GetListPos(currIndex)));
        //                UpdateEntityTracker(engine->editorManager->GetEntityID(GetListPos(currIndex)));
        //
        //            }
        //            //engine->editorManager->SetGlobalIndex(engine->editorManager->GetEntityID(GetListPos(currIndex)));
        //            //UpdateEntityTracker(engine->editorManager->GetEntityID(static_cast<int>(engine->editorManager->GetEntityIndex(GetListPos(currIndex)))));
        //
        //            if (!entCom1.Child.empty())
        //            {
        //                for (size_t x = 0; x < entCom1.Child.size(); ++x)
        //                {
        //                    auto& tempchild = engine->world.GetComponent<EntityComponent>(entCom1.Child[x]);
        //                    std::string names = my_strcat(tempchild.Name, " ", entCom1.Child[x]);

        //                    TreeNodeRecursion(names, tempchild, prev, curr, GetListPos(currIndex));
        //                }
        //            }
        //            ECGui::EndTreeNode();
        //        }
        //        else
        //        {
        //            for (size_t i = 0; i < entCom1.Child.size(); ++i)
        //            {
        //                auto& child = engine->world.GetComponent<EntityComponent>(entCom1.Child[i]);
        //                child.IsActive = false;
        //                HightLightParentAndChild(child);
        //            }
        //
        //             entCom1.IsActive = false;
        //             entCom.IsActive = true;
        //             HightLightParentAndChild(entCom1);
        //             HightLightParentAndChild(entCom);
        //             engine->editorManager->SetGlobalIndex(static_cast<size_t>(GetListPos(currIndex)));
        //             UpdateEntityTracker(engine->editorManager->GetEntityID(GetListPos(currIndex)));

        //            //size_t currIndex = ConvertEntityStringtoNumber(GetEntityComponentEntityNumber(parent));
        //
        //            //engine->editorManager->SetGlobalIndex(index);
        //            //UpdateEntityTracker(engine->editorManager->GetEntityID(engine->editorManager->GetEntityIndex(index)));
        //
        //            //engine->editorManager->DragAndDropInst_.IndexPayloadSource("Entity",
        //            //    static_cast<int>(engine->editorManager->GetEntityID(GetListPos(currIndex))), PayloadSourceType::PST_ENTITY, curr.index);
        //            //engine->editorManager->DragAndDropInst_.IndexPayloadTarget("Entity",
        //            //    static_cast<int>(engine->editorManager->GetEntityID(GetListPos(currIndex))), entCom.IsActive);
        //
        //              //if (ECGui::IsItemClicked(0))
        //              //{
        //              //    size_t currIndex = ConvertEntityStringtoNumber(GetEntityComponentEntityNumber(parent));
        //              //    engine->editorManager->SetGlobalIndex(engine->editorManager->GetEntityIndex(static_cast<Entity>(currIndex)));
        //              //    UpdateEntityTracker(engine->editorManager->GetEntityID(engine->editorManager->GetEntityIndex(static_cast<Entity>(currIndex))));
        //              //
        //              //    engine->editorManager->DragAndDropInst_.IndexPayloadSource("Entity",
        //              //        static_cast<int>(index), PayloadSourceType::PST_ENTITY, curr.index);
        //              //    engine->editorManager->DragAndDropInst_.IndexPayloadTarget("Entity",
        //              //        static_cast<int>(index), entcom1.IsActive);
        //              //}
        //        }
        //
        //
        //    }

        //    //engine->editorManager->DragAndDropInst_.IndexPayloadSource("Entity",
        //    //    static_cast<int>(index), PayloadSourceType::PST_ENTITY, currIndex);
        //    //engine->editorManager->DragAndDropInst_.IndexPayloadTarget("Entity",
        //    //    static_cast<int>(index), entCom.IsActive);
        //
        //    ECGui::EndTreeNode();
        //}
        //else
        //{
        //    //if parent node not open
        //
        //    for (size_t i = 0; i < entCom.Child.size(); ++i)
        //    {
        //        auto& child = engine->world.GetComponent<EntityComponent>(entCom.Child[i]);
        //        child.IsActive = false;
        //        HightLightParentAndChild(child);
        //    }

        //    // if (ECGui::IsItemClicked(0))
        //    // {
        //    /*HightLightParentAndChild(entCom);*/
        //    //size_t currIndex = ConvertEntityStringtoNumber(GetEntityComponentEntityNumber(parent));
        //    //engine->editorManager->SetGlobalIndex(static_cast<size_t>(GetListPos(currIndex)));
        //    //UpdateEntityTracker(engine->editorManager->GetEntityID(GetListPos(currIndex)));
        //    //engine->editorManager->SetGlobalIndex(engine->editorManager->GetEntityIndex(currIndex));
        //    //UpdateEntityTracker(engine->editorManager->GetEntityID(engine->editorManager->GetEntityIndex(currIndex)));
        //   //engine->editorManager->DragAndDropInst_.IndexPayloadSource("Entity",
        //   //    static_cast<int>(index), PayloadSourceType::PST_ENTITY, currIndex);
        //   //engine->editorManager->DragAndDropInst_.IndexPayloadTarget("Entity",
        //   //    static_cast<int>(index), entCom.IsActive);
        //    // }
        //}
        //
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
    int HierarchyWindow::GetListPos(size_t currIndex)
    {
        int pos = 0;

        for (auto& it : engine->editorManager->GetEntityListByConstRef())
        {

            if (it == currIndex)
            {
                return pos;
            }
            pos++;
        }

        return 0;
    }

