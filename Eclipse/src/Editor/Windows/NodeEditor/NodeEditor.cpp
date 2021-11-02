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
    }

    for (const Link& link : editor.links)
    {
        ImNodes::Link(link.linkId, link.startPoint, link.endPoint);

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

    ImNodes::EndNodeEditor();

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

    //{
    //    const int num_selected = ImNodes::NumSelectedLinks();
    //    if (num_selected > 0 && ECGui::IsKeyPressed(ECGui::GetKeyIndex(ImGuiKey_Delete)))
    //    {
    //        static std::vector<int> selected_links;
    //        selected_links.resize(static_cast<size_t>(num_selected));
    //        ImNodes::GetSelectedLinks(selected_links.data());
    //        for (const int edge_id : selected_links)
    //        {
    //            auto iter = std::find_if(
    //                editor.links.begin(), editor.links.end(), [edge_id](const Link& link) -> bool {
    //                    return link.linkId == edge_id;
    //                });
    //            assert(iter != editor.links.end());
    //            editor.links.erase(iter);
    //        }
    //    }
    //}


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
