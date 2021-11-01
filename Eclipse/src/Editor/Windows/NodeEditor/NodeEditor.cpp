#include "pch.h"
#include "NodeEditor.h"

void Eclipse::NodeEditorWindow::CreateInputFloatNode()
{
    editor1.nodes.emplace_back(Node("FloatNode",++editor1.current_id,0.0f,NodeType::FLOAT));


}

void Eclipse::NodeEditorWindow::CreatePrintNode()
{
    editor1.nodes.emplace_back(Node("PrintNode", ++editor1.current_id, 0.0f, NodeType::PRINT));
}

void Eclipse::NodeEditorWindow::DrawNodeEditor(const char* graphName, Editor_& editor)
{
    ImNodes::EditorContextSet(editor.context);

    ImNodes::BeginNodeEditor();

    for (Node& node : editor.nodes)
    {
        if (node.nodeType == NodeType::FLOAT)
        {
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
    }

    for (const Link& link : editor.links)
    {
        ImNodes::Link(link.linkId, link.startPoint, link.endPoint);
    }

    ImNodes::EndNodeEditor();

    {
        Link link;
        if (ImNodes::IsLinkCreated(&link.startPoint, &link.endPoint))
        {
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

    if (ImGui::GetIO().MouseClicked[1])
    {
        ImGui::OpenPopup("Test creation menu");
    }
    if (ImGui::BeginPopup("Test creation menu"))
    {
        if (ImGui::MenuItem("Input Action"))
        {
            CreateInputFloatNode();
        }

        if (ImGui::MenuItem("Print"))
        {
            CreatePrintNode();
        }

        ImGui::EndPopup();
    }

}

void Eclipse::NodeEditorWindow::Update()
{
	if (IsVisible)
		ECGui::DrawMainWindow<void()>(WindowName, std::bind(&NodeEditorWindow::DrawImpl, this));
}


void Eclipse::NodeEditorWindow::Init()
{
	WindowName = "NodeEditor " ICON_MDI_FILE_IMAGE;

	editor1.context = ImNodes::EditorContextCreate();
	ImNodes::PushAttributeFlag(ImNodesAttributeFlags_EnableLinkDetachWithDragClick);

	ImNodesIO& io = ImNodes::GetIO();
	io.LinkDetachWithModifierClick.Modifier = &ImGui::GetIO().KeyCtrl;
}

void Eclipse::NodeEditorWindow::Unload()
{
}

void Eclipse::NodeEditorWindow::DrawImpl()
{

	DrawNodeEditor("editor1", editor1);

}

Eclipse::NodeEditorWindow::~NodeEditorWindow()
{
	ImNodes::PopAttributeFlag();
	ImNodes::EditorContextFree(editor1.context);
}
