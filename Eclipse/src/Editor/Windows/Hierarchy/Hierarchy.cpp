#include "pch.h"
#include "Hierarchy.h"
#include "ECS/ComponentManager/Components/EntityComponent.h"
#include "ECS/ComponentManager/Components/TransformComponent.h"
#include "ECS/ComponentManager/Components/ParentComponent.h"
#include "ECS/ComponentManager/Components/ChildComponent.h"
#include "../Inspector/Inspector.h"

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

        if (ECGui::BeginTreeNode(SceneManager::GetCurrentSceneName().c_str()))
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

                if (!engine->world.CheckComponent<ParentComponent>(list[index]) && !engine->world.CheckComponent<ChildComponent>(list[index]))
                {
                    if (ECGui::CreateSelectableButton(entityName.c_str(), &entCom.IsActive))
                    {
                        entCom.IsActive = true;
                        engine->editorManager->SetGlobalIndex(index);
                        UpdateEntityTracker(engine->editorManager->GetEntityID(static_cast<int>(index)));


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

                if (engine->world.CheckComponent<ParentComponent>(list[index]))
                {
                    if (!engine->world.CheckComponent<ChildComponent>(list[index]))
                    {
                        ParentRecursion(list[index], list, prev, curr);    
                    }
                }

            }

        }
    }

    void HierarchyWindow::ParentRecursion(Entity Num, const std::vector<Entity>& list, EntitySelectionTracker& prev, EntitySelectionTracker& curr)
    {
        std::string entityName{};
        auto& entCom = engine->world.GetComponent<EntityComponent>(Num);
        float indentValue = entCom.ImguiIndentValue;
        ImDrawList* draw_list = ImGui::GetWindowDrawList();
       // bool highlight = false;
        if (engine->world.CheckComponent<ParentComponent>(Num))
        {
            auto& parent2 = engine->world.GetComponent<ParentComponent>(Num);

            entityName = my_strcat(entCom.Name, " ", Num);
            std::string ButtonName;
            if (!engine->world.CheckComponent<ChildComponent>(Num))
            {
                ButtonName = ICON_MDI_DOTS_VERTICAL + entityName;
            }
            else
            {
                ButtonName = ICON_MDI_SUBDIRECTORY_ARROW_RIGHT + entityName;
            }
            if (ECGui::CreateSelectableButton(ButtonName.c_str(), &entCom.IsActive))
            {
                if (engine->world.CheckComponent<ChildComponent>(Num))
                {
                    auto& entCom_Child = engine->world.GetComponent<ChildComponent>(Num);
                    if (!entCom_Child.hasParent)
                    {
                        unhighlightParent(entCom_Child.parentIndex);
                    }
                    engine->world.GetComponent<EntityComponent>(Num).hightLightChild = true;
                }

                entCom.IsActive = true;
                engine->editorManager->SetGlobalIndex(engine->editorManager->GetEntityIndex(Num));
                UpdateEntityTracker(Num);

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
                curr.index = Num;

                if (!isChild(parent2.child, prev.index))
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
                                prevEntCom2.IsActive = false;
                            }
                        }
                    }
                }

            }

            engine->editorManager->DragAndDropInst_.IndexPayloadSource("Entity",
                 engine->editorManager->GetEntityIndex(Num), PayloadSourceType::PST_ENTITY, curr.index);

            engine->editorManager->DragAndDropInst_.IndexPayloadTarget("Entity",
                engine->editorManager->GetEntityIndex(Num), entCom.IsActive);



                auto& entCom_Child = engine->world.GetComponent<EntityComponent>(Num);

                if (entCom_Child.hightLightChild)
                {
                    draw_list->AddRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax(), IM_COL32(0, 0, 255, 255));
                }

            for (auto& it : parent2.child)
            {
               // auto& parent2Com = engine->world.GetComponent<EntityComponent>(it);
                ImGui::Indent(indentValue);
                ParentRecursion(it, list, prev, curr);
                ImGui::Unindent(indentValue);
            }
        }
        else
        {
            entityName = my_strcat(entCom.Name, " ", Num);

            std::string ButtonName = ICON_MDI_SUBDIRECTORY_ARROW_RIGHT + entityName;

            if (ECGui::CreateSelectableButton(ButtonName.c_str(), &entCom.IsActive))
            {

                entCom.IsActive = true;
                engine->editorManager->SetGlobalIndex(engine->editorManager->GetEntityIndex(Num));
                UpdateEntityTracker(Num);


                if (!curr.name.empty())
                {
                    prev.name = curr.name;
                    prev.index = curr.index;
                }
                
                curr.name = entityName;
                curr.index = Num;
                
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
                
                for (size_t index = 0; index < list.size(); ++index)
                {
                    if (engine->world.CheckComponent<ChildComponent>(list[index]))
                    {
                        auto& parentCom = engine->world.GetComponent<ChildComponent>(list[index]);

                        if (list[index] != Num)
                        {
                            parentCom.IsAChild = false;
                        }
                    }
                }
            }


                auto& parentCom = engine->world.GetComponent<EntityComponent>(Num);

                if (parentCom.hightLightChild)
                {
                    draw_list->AddRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax(), IM_COL32(0, 0, 255, 255));
                }
            

            engine->editorManager->DragAndDropInst_.IndexPayloadTarget("Entity",
                engine->editorManager->GetEntityIndex(Num), entCom.IsActive);


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
            auto* insp = engine->editorManager->GetEditorWindow<InspectorWindow>();
            insp->ClearEntityName();

            if (engine->world.CheckComponent<EntityComponent>(EntTracker_.CurrEnt_.index))
            {
                auto& prevEntCom = engine->world.GetComponent<EntityComponent>(EntTracker_.CurrEnt_.index);
                prevEntCom.IsActive = false;
                EntTracker_.PrevEnt_.name = EntTracker_.CurrEnt_.name;
                EntTracker_.PrevEnt_.index = EntTracker_.CurrEnt_.index;

               if (engine->world.CheckComponent<ParentComponent>(EntTracker_.PrevEnt_.index))
               {
                   auto& parent = engine->world.GetComponent<ParentComponent>(EntTracker_.PrevEnt_.index);

                   prevEntCom.hightLightChild = false;

                   for (auto& it : parent.child)
                   {
                       highlightChild(it, false);
                   }
               }
               else   //check if its the base child unhighlight 
                   if (engine->world.CheckComponent<ChildComponent>(EntTracker_.PrevEnt_.index) && !engine->world.CheckComponent<ParentComponent>(EntTracker_.PrevEnt_.index))
                   {
                       prevEntCom.hightLightChild = false;
                   }

               //check if base child highlight
               if (engine->world.CheckComponent<ChildComponent>(ID) && !engine->world.CheckComponent<ParentComponent>(ID))
               {
                   entCom.hightLightChild = true;
               }

               if (engine->world.CheckComponent<MaterialComponent>(EntTracker_.PrevEnt_.index))
               {
                   engine->MaterialManager.UnHighlight(EntTracker_.PrevEnt_.index);
               }

               if (engine->world.CheckComponent<ParentComponent>(ID))
               {
                   auto& parent = engine->world.GetComponent<ParentComponent>(ID);

                   auto& parCom = engine->world.GetComponent<EntityComponent>(ID);

                   parCom.hightLightChild = true;

                   for (auto& it : parent.child)
                   {
                       highlightChild(it, true);
                   }
               }

            }

            EntTracker_.CurrEnt_.name = my_strcat(entCom.Name, " ", ID);
            EntTracker_.CurrEnt_.index = ID;
            entCom.IsActive = true;
            engine->gPicker.SetCurrentCollisionID(ID);
            insp->SetCurrentEntityName(entCom.Name.c_str());

            if (engine->world.CheckComponent<ScriptComponent>(ID))
            {
                auto& scriptCom = engine->world.GetComponent<ScriptComponent>(ID);
                insp->LoadScriptBitset(scriptCom);
            }
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

    void HierarchyWindow::unhighlightParent(Entity Parent)
    {
        auto& parentEntCom = engine->world.GetComponent<EntityComponent>(Parent);

        parentEntCom.hightLightChild = false;

        if (engine->world.CheckComponent<ChildComponent>(Parent))
        {

            auto& ChildCom = engine->world.GetComponent<ChildComponent>(Parent);

            if (ChildCom.hasParent)
            {
                unhighlightParent(ChildCom.parentIndex);
            }
        }
    }

    void HierarchyWindow::highlightChild(Entity CurrID, bool hightlight)
    {
        if (engine->world.CheckComponent<ChildComponent>(CurrID))
        {
           // ImDrawList* draw_list = ImGui::GetWindowDrawList();

            auto& entCom = engine->world.GetComponent<EntityComponent>(CurrID);

            if (hightlight)
            {
                if (engine->world.CheckComponent<MaterialComponent>(CurrID))
                    engine->MaterialManager.HighlightClick(CurrID);
                entCom.hightLightChild = true;
            }
            else
            {
                if (engine->world.CheckComponent<MaterialComponent>(CurrID))
                    engine->MaterialManager.UnHighlight(CurrID);
                entCom.hightLightChild = false;
            }

            if (engine->world.CheckComponent<ParentComponent>(CurrID))
            {
                auto& parent = engine->world.GetComponent<ParentComponent>(CurrID);

                for (auto& it : parent.child)
                {
                    highlightChild(it, hightlight);
                }

            }
        }
    }


}