#include "pch.h"
#include "NodeEditor.h"

int Eclipse::NodeEditorWindow::findNodePos(Editor_ editor, int id)
{
    int pos = 0;
    for (auto& it : editor.nodes)
    {
        if (it.nodeId == id)
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

void Eclipse::NodeEditorWindow::CreateInputFloatNode(Editor_& editor)
{
    editor.nodes.emplace_back(Node("FloatNode",++editor.current_id,NodeType::FLOAT));


}

void Eclipse::NodeEditorWindow::CreatePrintNode(Editor_& editor)
{
    editor.nodes.emplace_back(Node("PrintNode", ++editor.current_id,NodeType::PRINT));
}

void Eclipse::NodeEditorWindow::DrawNodeEditor(const char* graphName, Editor_& editor)
{
    if (!initialized)
    {
        initialized = true;
        editor1.context = ImNodes::EditorContextCreate();
        ImNodes::PushAttributeFlag(ImNodesAttributeFlags_EnableLinkDetachWithDragClick);
        ImNodesIO& io = ImNodes::GetIO();
        io.LinkDetachWithModifierClick.Modifier = &ImGui::GetIO().KeyCtrl;
    }

    ImNodes::EditorContextSet(editor.context);

    ImNodes::BeginNodeEditor();

    for (Node& node : editor.nodes)
    {
        if (node.nodeType == NodeType::FLOAT)
        {
            ImNodes::PushColorStyle(
                ImNodesCol_TitleBar, IM_COL32(0, 109, 0, 255));
            ImNodes::PushColorStyle(
                ImNodesCol_TitleBarSelected, IM_COL32(0, 148, 0, 255));
            ImNodes::BeginNode(node.nodeId);
            ImNodes::BeginNodeTitleBar();
            ImGui::TextUnformatted("Float");
            ImNodes::EndNodeTitleBar();
            ImNodes::BeginStaticAttribute(node.nodeId);
            ImGui::PushItemWidth(120.0f);
            ImGui::DragFloat("value", &node.value, 0.01f);
            ImGui::PopItemWidth();
            ImNodes::EndStaticAttribute();
            ImNodes::BeginOutputAttribute(node.nodeId);
            const float text_width = ImGui::CalcTextSize("output").x;
            ImGui::Indent(120.f + ImGui::CalcTextSize("value").x - text_width);
            ImNodes::EndOutputAttribute();

            ImNodes::EndNode();

            ImNodes::PopColorStyle();
            ImNodes::PopColorStyle();
        }
        if (node.nodeType == NodeType::PRINT)
        {
            ImNodes::BeginNode(node.nodeId);
            ImNodes::BeginNodeTitleBar();
            ImGui::TextUnformatted("Display");
            ImNodes::EndNodeTitleBar();
            ImNodes::BeginStaticAttribute(node.nodeId);
            ImGui::PushItemWidth(120.0f);
            ImGui::DragFloat("value", &node.value, 0.01f);
            ImGui::PopItemWidth();
            ImNodes::EndStaticAttribute();
            ImNodes::BeginInputAttribute(node.nodeId);
            const float text_width = ImGui::CalcTextSize("output").x;
            ImGui::Indent(120.f + ImGui::CalcTextSize("value").x - text_width);
            ImNodes::EndInputAttribute();

            ImNodes::EndNode();
        }

        if (node.nodeType == NodeType::ENTITY)
        {
            ImNodes::BeginNode(node.nodeId);
            ImNodes::BeginNodeTitleBar();
            ImGui::TextUnformatted(node.name.c_str());
            ImNodes::EndNodeTitleBar();
            ImNodes::BeginOutputAttribute(node.nodeId);
            ImNodes::EndOutputAttribute();
            ImNodes::EndNode();
        }
    }

    for (const Link& link : editor.links)
    {
        ImNodes::Link(link.linkId, link.startPoint, link.endPoint);

    }

    ImNodes::EndNodeEditor();

    static bool createEntityNode;
    if (ImGui::GetIO().MouseClicked[1])
    {
        ImGui::OpenPopup("Test creation menu");
    }
    if (ImGui::BeginPopup("Test creation menu"))
    {
        if (ImGui::MenuItem("Input Action"))
        {
            CreateInputFloatNode(editor);
        }

        if (ImGui::MenuItem("Print"))
        {
            CreatePrintNode(editor);
        }

        if (ImGui::MenuItem("Entity"))
        {
            createEntityNode = true;
        }

        if (ImGui::MenuItem("TransformNode"))
        {
            // CreateTransFormNode(editor);
        }

        ImGui::EndPopup();
    }

    if (createEntityNode)
    {
        ImGui::OpenPopup("Select Entity");
    }

    if (ImGui::BeginPopup("Select Entity"))
    {
        for (int i = 0; i < engine->editorManager->GetEntityListSize(); ++i)
        {
            Entity ent = engine->editorManager->GetEntityID(i);

            auto entComp = engine->world.GetComponent<EntityComponent>(ent);
            if (ImGui::MenuItem(entComp.Name.c_str()))
            {
                CreateEnitityNode(editor, entComp.Name);
                createEntityNode = false;
            }
            else
            {
                createEntityNode = false;
            }
        }

        ImGui::EndPopup();
    }

    {
        Link link;
        if (ImNodes::IsLinkCreated(&link.startPoint, &link.endPoint))
        {
            int pos = findNodePos(editor, link.startPoint);

            int pos2 = findNodePos(editor, link.endPoint);

            const bool linked = editor.nodes[pos].nodeType != editor.nodes[pos2].nodeType;

            if (linked)
            {
                editor.nodes[pos2].value = editor.nodes[pos].value;
            }
            link.linkId = ++editor.current_id;
            editor.links.push_back(link);
        }
    }

    {
        int link_id;
        if (ImNodes::IsLinkDestroyed(&link_id))
        {
            auto iter = std::find_if(
                editor.links.begin(), editor.links.end(), [link_id](const Link& link) -> bool {
                    return link.linkId == link_id;
                });
            assert(iter != editor.links.end());
            editor.links.erase(iter);
        }
    }
}

void Eclipse::NodeEditorWindow::CreateEnitityNode(Editor_& editor, std::string name)
{
    editor.nodes.emplace_back(Node(name.c_str(), ++editor.current_id, NodeType::ENTITY));
}


void Eclipse::NodeEditorWindow::Update()
{
	if (IsVisible)
		ECGui::DrawMainWindow<void()>(WindowName, std::bind(&NodeEditorWindow::DrawImpl, this));
}


void Eclipse::NodeEditorWindow::Init()
{
	WindowName = "NodeEditor " ICON_MDI_FILE_IMAGE;
}

void Eclipse::NodeEditorWindow::Unload()
{
    ImNodes::PopAttributeFlag();
    ImNodes::EditorContextFree(editor1.context);
}

void Eclipse::NodeEditorWindow::DrawImpl()
{

	DrawNodeEditor("editor1", editor1);

}

Eclipse::NodeEditorWindow::~NodeEditorWindow()
{

}
