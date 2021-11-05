#include "pch.h"
#include "NodeEditorWindow.h"

int Eclipse::NodeEditor::findNodePos(NodeEditor editor, int id)
{
    int pos = 0;
    for (auto& it : editor.nodes)
    {
        if (it->nodeId == id)
        {
            return pos;
        }
        pos++;
    }

    return -1;
}

Eclipse::NodeEditorWindow::NodeEditorWindow()
{
}

void Eclipse::NodeEditor::DrawNodeEditor(const char* graphName, NodeEditor& editor)
{
    if (!editor.initialized)
    {
        initialized = true;
        editor.context = ImNodes::EditorContextCreate();
        ImNodes::PushAttributeFlag(ImNodesAttributeFlags_EnableLinkDetachWithDragClick);
        ImNodesIO& io = ImNodes::GetIO();
        io.LinkDetachWithModifierClick.Modifier = &ImGui::GetIO().KeyCtrl;
    }

    ImNodes::EditorContextSet(editor.context);

    ImNodes::BeginNodeEditor();

    bool context_open = false;
    bool create_ent = false;
    if (ImNodes::IsEditorHovered() && ImGui::IsMouseClicked(1)) {
        ImGui::OpenPopup("context_menu");
        context_open = true;
    }

    if (ImGui::BeginPopup("context_menu"))
    {
        if (ImGui::MenuItem("ENT NODE"))
        {
            create_ent = true;
        }
        if (ImGui::MenuItem("Transform NODE"))
        {
            NodeEditor::addNode(NodeEditor::Node::NodeType::TRANSFORM);
        }

        ImGui::EndPopup();
    }

    if (create_ent)
    {
        ImGui::OpenPopup("Entity Creation");
    }

    if (ImGui::BeginPopup("Entity Creation"))
    {
        for (int i = 0; i < engine->editorManager->GetEntityListSize(); ++i)
        {
            Entity ent = engine->editorManager->GetEntityID(i);

            auto entComp = engine->world.GetComponent<EntityComponent>(ent);
            if (ImGui::MenuItem(entComp.Name.c_str()))
            {
                NodeEditor::addEntityNode(ent);
                create_ent = false;
            }
            else
            {
                create_ent = false;
            }
        }
        ImGui::EndPopup();
    }




    for (std::shared_ptr<NodeEditor::Node>& n : editor.nodes)
    {
        n->nodeOnGui();
    }

    for (const const NodeEditor::Link& link : editor.links)
    {
        ImNodes::Link(link.linkId, link.startPoint, link.endPoint);
    }

    ImNodes::EndNodeEditor();

    for (std::shared_ptr<NodeEditor::Node>& n : editor.nodes)
    {
        ImVec2 p = ImNodes::GetNodeEditorSpacePos(n->nodeId);

        if (p.x != n->editorSpacePos.x || p.y != n->editorSpacePos.y) 
        {
            n->editorSpacePos = p;
        }
    }

    if (context_open) {
        m_context_link = -1;
        ImNodes::IsLinkHovered(&m_context_link);
        m_context_node = -1;
        ImNodes::IsNodeHovered(&m_context_node);
    }

    {
        Link links;
        if (ImNodes::IsLinkCreated(&links.startPoint, &links.endPoint))
        {
            int from = links.startPoint ^ (5 << 13);
            int to = links.endPoint ^ (5 << 16);
            int pos = findNodePos(editor, from);

            int pos2 = findNodePos(editor, to);

            NodeEditor::Node::NodeType type1 = editor.nodes[pos]->getType();
            NodeEditor::Node::NodeType type2 = editor.nodes[pos2]->getType();
            const bool linked = type1 != type2;

            if (linked)
            {
               /* if (type1 == NodeEditor::Node::NodeType::ENTITY)
                {
                    if (type2 == NodeEditor::Node::NodeType::TRANSFORM)
                    {

                        std::dynamic_pointer_cast<TransformNode>(editor.nodes[pos2])->ID
                            = std::dynamic_pointer_cast<EnitityNode>(editor.nodes[pos])->ID;

                    }
                }*/

                LinkNodes<NodeEditor::Node>(editor.nodes[pos], editor.nodes[pos2], true);
            }
            

            editor.links.push_back(links);
            editor.links.back().linkId = editor.generatedId();
            editor.links.back().startPoint = links.startPoint;
            editor.links.back().endPoint = links.endPoint;

        }
    }

    {
        int link_id;
        if (ImNodes::IsLinkDestroyed(&link_id))
        {
            auto iter = std::find_if(
                editor.links.begin(), editor.links.end(), [link_id](const Link& link) -> bool
                {
                    return link.linkId == link_id;
                });

            assert(iter != editor.links.end());

            int from = iter->startPoint ^ (5 << 13);
            int to = iter->endPoint ^ (5 << 16);
            int pos = findNodePos(editor, from);

            int pos2 = findNodePos(editor, to);
 
            NodeEditor::Node::NodeType type1 = editor.nodes[pos]->getType();
            NodeEditor::Node::NodeType type2 = editor.nodes[pos2]->getType();
            const bool notlinked = type1 != type2;
            if (notlinked)
            {
                LinkNodes<NodeEditor::Node>(editor.nodes[pos], editor.nodes[pos2], false);
            }
            editor.links.erase(iter);
        }
    }


   // {
   //     Link link;
   //     if (ImNodes::IsLinkCreated(&link.startPoint, &link.endPoint))
   //     {
   //         int pos = findNodePos(editor, link.startPoint);
   // 
   //         int pos2 = findNodePos(editor, link.endPoint);
   // 
   //         const bool linked = editor.nodes[pos].nodeType != editor.nodes[pos2].nodeType;
   // 
   //         if (linked)
   //         {
   //             editor.nodes[pos2].value = editor.nodes[pos].value;
   //         }
   //         link.linkId = ++editor.current_id;
   //         editor.links.push_back(link);
   //     }
   // }
   //
   // {
   //     int link_id;
   //     if (ImNodes::IsLinkDestroyed(&link_id))
   //     {
   //         auto iter = std::find_if(
   //             editor.links.begin(), editor.links.end(), [link_id](const Link& link) -> bool {
   //                 return link.linkId == link_id;
   //             });
   //         assert(iter != editor.links.end());
   //         editor.links.erase(iter);
   //     }
   // }
}

void Eclipse::NodeEditorWindow::Update()
{
	if (IsVisible)
		ECGui::DrawMainWindow<void()>(WindowName, std::bind(&NodeEditorWindow::DrawImpl, this));
}


void Eclipse::NodeEditorWindow::Init()
{
    IsVisible = false;
	WindowName = "NodeEditor " ICON_MDI_FILE_IMAGE;
}

void Eclipse::NodeEditorWindow::Unload()
{
    ImNodes::PopAttributeFlag();
    ImNodes::EditorContextFree(sceneEditor.context);
}

void Eclipse::NodeEditorWindow::DrawImpl()
{

    sceneEditor.DrawNodeEditor("editor1", sceneEditor);

}

Eclipse::NodeEditorWindow::~NodeEditorWindow()
{

}
