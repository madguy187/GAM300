#include "pch.h"
#include "NodeEditor.h"

Node* Eclipse::NodeEditorWindow::test()
{
    s_Nodes.emplace_back(GetNextId(), "MeString", ImColor(0, 195, 0));
    s_Nodes.back().Type = NodeType::test;
    s_Nodes.back().Outputs.emplace_back(GetNextId(), "", PinType::String);

    BuildNode(&s_Nodes.back());

    return &s_Nodes.back();
}

Node* Eclipse::NodeEditorWindow::testPrintOnConsole()
{
    s_Nodes.emplace_back(GetNextId(), "PrintStringOnConsole", ImColor(255, 128, 128));
    s_Nodes.back().Type = NodeType::test;
    s_Nodes.back().Inputs.emplace_back(GetNextId(), "InputString", PinType::String);
    BuildNode(&s_Nodes.back());
    return &s_Nodes.back();
}

Node* Eclipse::NodeEditorWindow::SpawnInputActionNode()
{
    s_Nodes.emplace_back(GetNextId(), "InputAction Fire", ImColor(255, 128, 128));
    s_Nodes.back().Outputs.emplace_back(GetNextId(), "", PinType::Delegate);
    s_Nodes.back().Outputs.emplace_back(GetNextId(), "Pressed", PinType::Flow);
    s_Nodes.back().Outputs.emplace_back(GetNextId(), "Released", PinType::Flow);

    BuildNode(&s_Nodes.back());

    return &s_Nodes.back();
}

Node* Eclipse::NodeEditorWindow::SpawnBranchNode()
{
    s_Nodes.emplace_back(GetNextId(), "Branch");
    s_Nodes.back().Inputs.emplace_back(GetNextId(), "", PinType::Flow);
    s_Nodes.back().Inputs.emplace_back(GetNextId(), "Condition", PinType::Bool);
    s_Nodes.back().Outputs.emplace_back(GetNextId(), "True", PinType::Flow);
    s_Nodes.back().Outputs.emplace_back(GetNextId(), "False", PinType::Flow);

    BuildNode(&s_Nodes.back());

    return &s_Nodes.back();
}

Node* Eclipse::NodeEditorWindow::SpawnOutputActionNode()
{
    s_Nodes.emplace_back(GetNextId(), "OutputAction");
    s_Nodes.back().Inputs.emplace_back(GetNextId(), "Sample", PinType::Float);
    s_Nodes.back().Outputs.emplace_back(GetNextId(), "Condition", PinType::Bool);
    s_Nodes.back().Inputs.emplace_back(GetNextId(), "Event", PinType::Delegate);

    BuildNode(&s_Nodes.back());

    return &s_Nodes.back();
}

Node* Eclipse::NodeEditorWindow::SpawnMultiplyNode()
{
    s_Nodes.emplace_back(GetNextId(), "Multiply", ImColor(128, 195, 248));
    s_Nodes.back().Type = NodeType::testmultiply;
    s_Nodes.back().Inputs.emplace_back(GetNextId(), "", PinType::Float);
    s_Nodes.back().Inputs.emplace_back(GetNextId(), "", PinType::Float);
    s_Nodes.back().Outputs.emplace_back(GetNextId(), "", PinType::Float);

    BuildNode(&s_Nodes.back());

    return &s_Nodes.back();
}

void Eclipse::NodeEditorWindow::Update()
{
		if (IsVisible)
			ECGui::DrawMainWindow<void()>(WindowName, std::bind(&NodeEditorWindow::DrawImpl, this));
}

void Eclipse::NodeEditorWindow::Init()
{
	Type = EditorWindowType::EWT_NODEEDITOR;
	WindowName = "NodeEditor " ICON_MDI_FILE_IMAGE;
	ed::Config config;
	config.SettingsFile = "Simple.json";
    IsVisible = false;
	g_Context = ed::CreateEditor(&config);

}

void Eclipse::NodeEditorWindow::Unload()
{
	
}
void Eclipse::NodeEditorWindow::DrawImpl()
{
    UpdateTouch();

    auto& io = ImGui::GetIO();

    ImGui::Text("FPS: %.2f (%.2gms)", io.Framerate, io.Framerate ? 1000.0f / io.Framerate : 0.0f);

    ed::SetCurrentEditor(g_Context);

    ImGui::SameLine(0.0f, 12.0f);

    ed::Begin("Node editor");
    {
        auto cursorTopLeft = ImGui::GetCursorScreenPos();

        for (auto& node : s_Nodes)
        {

            bool hasOutputDelegates = false;
            for (auto& output : node.Outputs)
                if (output.Type == PinType::Delegate)
                    hasOutputDelegates = true;

            ed::BeginNode(node.ID);

            if (node.Type == NodeType::test)
            {
                for (auto& input : node.Inputs)
                {
                    if (!input.Name.empty())
                    {
                        ImGui::TextUnformatted(input.Name.c_str());
                    }

                    auto alpha = ImGui::GetStyle().Alpha;
                    if (newLinkPin && !CanCreateLink(newLinkPin, &input) && &input != newLinkPin)
                        alpha = alpha * (48.0f / 255.0f);

                    ImGui::PushStyleVar(ImGuiStyleVar_Alpha, alpha);
                    ed::BeginPin(input.ID, ed::PinKind::Output);
                    DrawPinIcon(input, IsPinLinked(input.ID), (int)(alpha * 255));
                    ed::EndPin();
                    ImGui::PopStyleVar();
                }

                for (auto& output : node.Outputs)
                {
                    auto alpha = ImGui::GetStyle().Alpha;
                    if (newLinkPin && !CanCreateLink(newLinkPin, &output) && &output != newLinkPin)
                        alpha = alpha * (48.0f / 255.0f);

                    ImGui::PushStyleVar(ImGuiStyleVar_Alpha, alpha);
                    if (output.Type == PinType::String)
                    {
                        ImGui::PushItemWidth(100.0f);
                        ImGui::InputText("##edit", output.Data, 128);
                        ImGui::PopItemWidth();

                        bool hasOutputDelegates = false;
                        for (auto& output : node.Outputs)
                            if (output.Type == PinType::Delegate)
                                hasOutputDelegates = true;
                        
                        if (node.Type == NodeType::test)
                        {
                            for (auto& link : s_Links)
                            {
                                if (output.ID == link.StartPinID)
                                {
                                    std::cout << output.Data;
                                }
                            }
                        
                        }
   
                    }
                    if (!output.Name.empty())
                    {
                        ImGui::TextUnformatted(output.Name.c_str());
                    }

                    ECGui::InsertSameLine();
                    ed::BeginPin(output.ID, ed::PinKind::Output);
                    DrawPinIcon(output, IsPinLinked(output.ID), (int)(alpha * 255));
                    ed::EndPin();
                    ImGui::PopStyleVar();
                }
            }

            /*if (node.Type == NodeType::testmultiply)
            {
                for (auto& input : node.Inputs)
                {
                    if (input.Type == PinType::Float)
                    {
                        static char in1[127];
                        static char in2[127];
                        ImGui::PushItemWidth(100.0f);
                        ImGui::InputText("##edit", in1, 128);
                        ImGui::PopItemWidth();
                        ImGui::PushItemWidth(100.0f);
                        ImGui::InputText("##edit", in2, 128);
                        ImGui::PopItemWidth();
                    }

                    if (!input.Name.empty())
                    {
                        ImGui::TextUnformatted(input.Name.c_str());
                    }

                    auto alpha = ImGui::GetStyle().Alpha;
                    if (newLinkPin && !CanCreateLink(newLinkPin, &input) && &input != newLinkPin)
                        alpha = alpha * (48.0f / 255.0f);

                    ImGui::PushStyleVar(ImGuiStyleVar_Alpha, alpha);
                    ed::BeginPin(input.ID, ed::PinKind::Output);
                    DrawPinIcon(input, IsPinLinked(input.ID), (int)(alpha * 255));
                    ed::EndPin();
                    ImGui::PopStyleVar();
                }

                for (auto& output : node.Outputs)
                {
                    auto alpha = ImGui::GetStyle().Alpha;
                    if (newLinkPin && !CanCreateLink(newLinkPin, &output) && &output != newLinkPin)
                        alpha = alpha * (48.0f / 255.0f);

                    ImGui::PushStyleVar(ImGuiStyleVar_Alpha, alpha);

                    if (output.Type == PinType::Float)
                    {
                        char out[127];
                        ImGui::PushItemWidth(100.0f);
                        ImGui::Text("##edit", out, 128);
                        ImGui::PopItemWidth();
                    }

                    if (!output.Name.empty())
                    {
                        ImGui::TextUnformatted(output.Name.c_str());
                    }
                    ECGui::InsertSameLine();
                    ed::BeginPin(output.ID, ed::PinKind::Output);
                    DrawPinIcon(output, IsPinLinked(output.ID), (int)(alpha * 255));
                    ed::EndPin();
                    ImGui::PopStyleVar();
                }
            }*/
            ed::EndNode();
        }


        for (auto& link : s_Links)
        {
            ed::Link(link.ID, link.StartPinID, link.EndPinID, link.Color, 2.0f);
        }

        if (!createNewNode)
        {
            if (ed::BeginCreate(ImColor(255, 255, 255), 2.0f))
            {
                auto showLabel = [](const char* label, ImColor color)
                {
                    ImGui::SetCursorPosY(ImGui::GetCursorPosY() - ImGui::GetTextLineHeight());
                    auto size = ImGui::CalcTextSize(label);

                    auto padding = ImGui::GetStyle().FramePadding;
                    auto spacing = ImGui::GetStyle().ItemSpacing;

                    ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(spacing.x, -spacing.y));

                    auto rectMin = ImGui::GetCursorScreenPos() - padding;
                    auto rectMax = ImGui::GetCursorScreenPos() + size + padding;

                    auto drawList = ImGui::GetWindowDrawList();
                    drawList->AddRectFilled(rectMin, rectMax, color, size.y * 0.15f);
                    ImGui::TextUnformatted(label);
                };

                ed::PinId startPinId = 0, endPinId = 0;
                if (ed::QueryNewLink(&startPinId, &endPinId))
                {
                    auto startPin = FindPin(startPinId);
                    auto endPin = FindPin(endPinId);

                    newLinkPin = startPin ? startPin : endPin;

                    if (startPin->Kind == PinKind::Input)
                    {
                        std::swap(startPin, endPin);
                        std::swap(startPinId, endPinId);
                    }

                    if (startPin && endPin)
                    {
                        if (endPin == startPin)
                        {
                            ed::RejectNewItem(ImColor(255, 0, 0), 2.0f);
                        }
                        else if (endPin->Kind == startPin->Kind)
                        {
                            showLabel("x Incompatible Pin Kind", ImColor(45, 32, 32, 180));
                            ed::RejectNewItem(ImColor(255, 0, 0), 2.0f);
                        }
                        //else if (endPin->Node == startPin->Node)
                        //{
                        //    showLabel("x Cannot connect to self", ImColor(45, 32, 32, 180));
                        //    ed::RejectNewItem(ImColor(255, 0, 0), 1.0f);
                        //}
                        else if (endPin->Type != startPin->Type)
                        {
                            showLabel("x Incompatible Pin Type", ImColor(45, 32, 32, 180));
                            ed::RejectNewItem(ImColor(255, 128, 128), 1.0f);
                        }
                        else
                        {
                            showLabel("+ Create Link", ImColor(32, 45, 32, 180));
                            if (ed::AcceptNewItem(ImColor(128, 255, 128), 4.0f))
                            {
                                s_Links.emplace_back(Link(GetNextId(), startPinId, endPinId));
                                s_Links.back().Color = GetIconColor(startPin->Type);
                            }
                        }
                    }
                }

                ed::PinId pinId = 0;
                if (ed::QueryNewNode(&pinId))
                {
                    newLinkPin = FindPin(pinId);
                    if (newLinkPin)
                        showLabel("+ Create Node", ImColor(32, 45, 32, 180));

                    if (ed::AcceptNewItem())
                    {
                        createNewNode = true;
                        newNodeLinkPin = FindPin(pinId);
                        newLinkPin = nullptr;
                        ed::Suspend();
                        ImGui::OpenPopup("Create New Node");
                        ed::Resume();
                    }
                }
            }
            else
                newLinkPin = nullptr;

            ed::EndCreate();

            if (ed::BeginDelete())
            {
                ed::LinkId linkId = 0;
                while (ed::QueryDeletedLink(&linkId))
                {
                    if (ed::AcceptDeletedItem())
                    {
                        auto id = std::find_if(s_Links.begin(), s_Links.end(), [linkId](auto& link) { return link.ID == linkId; });
                        if (id != s_Links.end())
                            s_Links.erase(id);
                    }
                }

                ed::NodeId nodeId = 0;
                while (ed::QueryDeletedNode(&nodeId))
                {
                    if (ed::AcceptDeletedItem())
                    {
                        auto id = std::find_if(s_Nodes.begin(), s_Nodes.end(), [nodeId](auto& node) { return node.ID == nodeId; });
                        if (id != s_Nodes.end())
                            s_Nodes.erase(id);
                    }
                }
            }
            ed::EndDelete();
        }

        ImGui::SetCursorScreenPos(cursorTopLeft);
    }

# if 1
    auto openPopupPosition = ImGui::GetMousePos();
    ed::Suspend();
    if (ed::ShowNodeContextMenu(&contextNodeId))
        ImGui::OpenPopup("Node Context Menu");
    else if (ed::ShowPinContextMenu(&contextPinId))
        ImGui::OpenPopup("Pin Context Menu");
    else if (ed::ShowLinkContextMenu(&contextLinkId))
        ImGui::OpenPopup("Link Context Menu");
    else if (ed::ShowBackgroundContextMenu())
    {
        ImGui::OpenPopup("Create New Node");
        newNodeLinkPin = nullptr;
    }
    ed::Resume();

    ed::Suspend();
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(8, 8));
    if (ImGui::BeginPopup("Node Context Menu"))
    {
        auto node = FindNode(contextNodeId);

        ImGui::TextUnformatted("Node Context Menu");
        ImGui::Separator();
        if (node)
        {
            ImGui::Text("ID: %p", node->ID.AsPointer());
            //ImGui::Text("Type: %s", node->Type == NodeType::Blueprint ? "Blueprint" : (node->Type == NodeType::Tree ? "Tree" : "Comment"));
            ImGui::Text("Inputs: %d", (int)node->Inputs.size());
            ImGui::Text("Outputs: %d", (int)node->Outputs.size());
        }
        else
            ImGui::Text("Unknown node: %p", contextNodeId.AsPointer());
        ImGui::Separator();
        if (ImGui::MenuItem("Delete"))
            ed::DeleteNode(contextNodeId);
        ImGui::EndPopup();
    }

    if (ImGui::BeginPopup("Pin Context Menu"))
    {
        auto pin = FindPin(contextPinId);

        ImGui::TextUnformatted("Pin Context Menu");
        ImGui::Separator();
        if (pin)
        {
            ImGui::Text("ID: %p", pin->ID.AsPointer());
            if (pin->Node)
                ImGui::Text("Node: %p", pin->Node->ID.AsPointer());
            else
                ImGui::Text("Node: %s", "<none>");
        }
        else
            ImGui::Text("Unknown pin: %p", contextPinId.AsPointer());

        ImGui::EndPopup();
    }

    if (ImGui::BeginPopup("Link Context Menu"))
    {
        auto link = FindLink(contextLinkId);

        ImGui::TextUnformatted("Link Context Menu");
        ImGui::Separator();
        if (link)
        {
            ImGui::Text("ID: %p", link->ID.AsPointer());
            ImGui::Text("From: %p", link->StartPinID.AsPointer());
            ImGui::Text("To: %p", link->EndPinID.AsPointer());
        }
        else
            ImGui::Text("Unknown link: %p", contextLinkId.AsPointer());
        ImGui::Separator();
        if (ImGui::MenuItem("Delete"))
            ed::DeleteLink(contextLinkId);
        ImGui::EndPopup();
    }

    if (ImGui::BeginPopup("Create New Node"))
    {
        auto newNodePostion = openPopupPosition;
        //ImGui::SetCursorScreenPos(ImGui::GetMousePosOnOpeningCurrentPopup());

        //auto drawList = ImGui::GetWindowDrawList();
        //drawList->AddCircleFilled(ImGui::GetMousePosOnOpeningCurrentPopup(), 10.0f, 0xFFFF00FF);

        Node* node = nullptr;
        if (ImGui::MenuItem("Input Action"))
            node = SpawnInputActionNode();
        if (ImGui::MenuItem("f Action"))
            node = SpawnOutputActionNode();
        if (ImGui::MenuItem("Input m"))
            node = SpawnBranchNode();
        if (ImGui::MenuItem("String"))
            node = test();
        if (ImGui::MenuItem("PrintOnConsole"))
            node = testPrintOnConsole();
        if (ImGui::MenuItem("testMultiply"))
            node = SpawnMultiplyNode();
        if (node)
        {
            BuildNodes();

            createNewNode = false;

            ed::SetNodePosition(node->ID, newNodePostion);

            if (auto startPin = newNodeLinkPin)
            {
                auto& pins = startPin->Kind == PinKind::Input ? node->Outputs : node->Inputs;

                for (auto& pin : pins)
                {
                    if (CanCreateLink(startPin, &pin))
                    {
                        auto endPin = &pin;
                        if (startPin->Kind == PinKind::Input)
                            std::swap(startPin, endPin);

                        s_Links.emplace_back(Link(GetNextId(), startPin->ID, endPin->ID));
                        s_Links.back().Color = GetIconColor(startPin->Type);

                        break;
                    }
                }
            }
        }

        ImGui::EndPopup();
    }
    else
        createNewNode = false;
    ImGui::PopStyleVar();
    ed::Resume();
# endif

    /*
        cubic_bezier_t c;
        c.p0 = pointf(100, 600);
        c.p1 = pointf(300, 1200);
        c.p2 = pointf(500, 100);
        c.p3 = pointf(900, 600);

        auto drawList = ImGui::GetWindowDrawList();
        auto offset_radius = 15.0f;
        auto acceptPoint = [drawList, offset_radius](const bezier_subdivide_result_t& r)
        {
            drawList->AddCircle(to_imvec(r.point), 4.0f, IM_COL32(255, 0, 255, 255));

            auto nt = r.tangent.normalized();
            nt = pointf(-nt.y, nt.x);

            drawList->AddLine(to_imvec(r.point), to_imvec(r.point + nt * offset_radius), IM_COL32(255, 0, 0, 255), 1.0f);
        };

        drawList->AddBezierCurve(to_imvec(c.p0), to_imvec(c.p1), to_imvec(c.p2), to_imvec(c.p3), IM_COL32(255, 255, 255, 255), 1.0f);
        cubic_bezier_subdivide(acceptPoint, c);
    */

    ed::End();


    ed::SetCurrentEditor(nullptr);

    //ImGui::ShowTestWindow();
    //ImGui::ShowMetricsWindow();
}

Eclipse::NodeEditorWindow::~NodeEditorWindow()
{
    ed::DestroyEditor(g_Context);
}


int Eclipse::NodeEditorWindow::GetNextId()
{
	return s_NextId++;
}

ed::LinkId Eclipse::NodeEditorWindow::GetNextLinkId()
{
	return ed::LinkId(GetNextId());
}

void Eclipse::NodeEditorWindow::TouchNode(ed::NodeId id)
{
	s_NodeTouchTime[id] = s_TouchTime;
}

float Eclipse::NodeEditorWindow::GetTouchProgress(ed::NodeId id)
{
	auto it = s_NodeTouchTime.find(id);
	if (it != s_NodeTouchTime.end() && it->second > 0.0f)
	{
		return (s_TouchTime - it->second) / s_TouchTime;
	}
	else
	{
		return 0.0f;
	}
}

void Eclipse::NodeEditorWindow::UpdateTouch()
{
	const auto deltaTime = ImGui::GetIO().DeltaTime;
	for (auto& entry : s_NodeTouchTime)
	{
		if (entry.second > 0.0f)
		{
			entry.second -= deltaTime;
		}
	}
}

void Eclipse::NodeEditorWindow::BuildNode(Node* node)
{
	for (auto& input : node->Inputs)
	{
		input.Node = node;
		input.Kind = PinKind::Input;
	}

	for (auto& output : node->Outputs)
	{
		output.Node = node;
		output.Kind = PinKind::Output;
	}
}

void Eclipse::NodeEditorWindow::BuildNodes()
{
    for (auto& node : s_Nodes)
    {
        BuildNode(&node);
    }
}

Node* Eclipse::NodeEditorWindow::FindNode(ed::NodeId id)
{
	for (auto& node : s_Nodes)
	{
		if (node.ID == id)
		{
			return &node;
		}
	}

	return nullptr;
}

Link* Eclipse::NodeEditorWindow::FindLink(ed::LinkId id)
{
	for (auto& link : s_Links)
	{
		if (link.ID == id)
		{
			return &link;
		}
	}

	return nullptr;
}

Pin* Eclipse::NodeEditorWindow::FindPin(ed::PinId id)
{
	if (!id)
	{
		return nullptr;
	}

	for (auto& node : s_Nodes)
	{
		for (auto& pin : node.Inputs)
		{
			if (pin.ID == id)
			{
				return &pin;
			}
		}

		for (auto& pin : node.Outputs)
		{
			if (pin.ID == id)
			{
				return &pin;
			}
		}
	}

	return nullptr;
}

bool Eclipse::NodeEditorWindow::IsPinLinked(ed::PinId id)
{
	if (!id)
	{
		return false;
	}

	for (auto& link : s_Links)
	{
		if (link.StartPinID == id || link.EndPinID == id)
		{
			return true;
		}
	}

	return false;
}

bool Eclipse::NodeEditorWindow::CanCreateLink(Pin* a, Pin* b)
{
	if (!a || !b || a == b || a->Kind == b->Kind || 
			a->Type != b->Type || a->Node == b->Node)
	{
		return false;
	}

	return true;
}

void Eclipse::NodeEditorWindow::DrawPinIcon(const Pin& pin, bool connected, int alpha)
{
    IconType iconType;
    ImColor  color = GetIconColor(pin.Type);
    color.Value.w = alpha / 255.0f;
    switch (pin.Type)
    {
    case PinType::Flow:     iconType = IconType::Flow;   break;
    case PinType::Bool:     iconType = IconType::Circle; break;
    case PinType::Int:      iconType = IconType::Circle; break;
    case PinType::Float:    iconType = IconType::Circle; break;
    case PinType::String:   iconType = IconType::Circle; break;
    case PinType::Object:   iconType = IconType::Circle; break;
    case PinType::Function: iconType = IconType::Circle; break;
    case PinType::Delegate: iconType = IconType::Square; break;
    default:
        return;
    }

    ax::Widgets::Icon(ImVec2(s_PinIconSize, s_PinIconSize), iconType, connected, color, ImColor(32, 32, 32, alpha));
}

ImColor Eclipse::NodeEditorWindow::GetIconColor(PinType type)
{
    switch (type)
    {
    default:
    case PinType::Flow:     return ImColor(255, 255, 255);
    case PinType::Bool:     return ImColor(220, 48, 48);
    case PinType::Int:      return ImColor(68, 201, 156);
    case PinType::Float:    return ImColor(147, 226, 74);
    case PinType::String:   return ImColor(124, 21, 153);
    case PinType::Object:   return ImColor(51, 150, 215);
    case PinType::Function: return ImColor(218, 0, 183);
    case PinType::Delegate: return ImColor(255, 48, 48);
    }
};

bool Eclipse::NodeEditorWindow::Splitter(bool split_vertically, float thickness, float* size1, float* size2, float min_size1, float min_size2, float splitter_long_axis_size)
{
    using namespace ImGui;
    ImGuiContext& g = *GImGui;
    ImGuiWindow* window = g.CurrentWindow;
    ImGuiID id = window->GetID("##Splitter");
    ImRect bb;
    bb.Min = window->DC.CursorPos + (split_vertically ? ImVec2(*size1, 0.0f) : ImVec2(0.0f, *size1));
    bb.Max = bb.Min + CalcItemSize(split_vertically ? ImVec2(thickness, splitter_long_axis_size) : ImVec2(splitter_long_axis_size, thickness), 0.0f, 0.0f);
    return SplitterBehavior(bb, id, split_vertically ? ImGuiAxis_X : ImGuiAxis_Y, size1, size2, min_size1, min_size2, 0.0f);
}
