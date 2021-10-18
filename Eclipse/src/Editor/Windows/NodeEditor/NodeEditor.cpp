#include "pch.h"
#include "NodeEditor.h"

namespace ed = ax::NodeEditor;


static NodeEditor::EditorContext* g_Context = nullptr;

void Eclipse::NodeEditorWindow::Update()
{
		if (IsVisible)
			ECGui::DrawMainWindow<void()>(WindowName, std::bind(&NodeEditorWindow::DrawImpl, this));
}

void Eclipse::NodeEditorWindow::Init()
{
	Type = EditorWindowType::EWT_NODEEDITOR;
	WindowName = "NodeEditor " ICON_MDI_FILE_IMAGE;
}

void Eclipse::NodeEditorWindow::Unload()
{
}

void Eclipse::NodeEditorWindow::DrawImpl()
{

	//ed::SetCurrentEditor(g_Context);
	ed::Begin("My Editor", ImVec2(0.0, 0.0f));
	//int uniqueId = 1;
	//// Start drawing nodes.
	//ed::BeginNode(uniqueId++);
	//ImGui::Text("Node A");
	//ed::BeginPin(uniqueId++, ed::PinKind::Input);
	//ImGui::Text("-> In");
	//ed::EndPin();
	//ImGui::SameLine();
	//ed::BeginPin(uniqueId++, ed::PinKind::Output);
	//ImGui::Text("Out ->");
	//ed::EndPin();
	//ed::EndNode();
	//ed::BeginNode(uniqueId++);
	//ImGui::Text("Node Test");
	//ed::BeginPin(uniqueId++, ed::PinKind::Input);
	//ImGui::Text("-> In");
	//ed::EndPin();
	//ImGui::SameLine();
	//ed::BeginPin(uniqueId++, ed::PinKind::Output);
	//ImGui::Text("Out ->");
	//ed::EndPin();
	//ed::EndNode();
	ed::End();
	//ed::SetCurrentEditor(nullptr);

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
