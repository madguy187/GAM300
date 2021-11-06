#include "pch.h"
#include "NodeEditorWindow.h"

Eclipse::NodeEditorWindow::NodeEditorWindow()
{
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
	sceneEditor.context = ImNodes::EditorContextCreate();
	ImNodes::PushAttributeFlag(ImNodesAttributeFlags_EnableLinkDetachWithDragClick);
	ImNodesIO& io = ImNodes::GetIO();
	io.LinkDetachWithModifierClick.Modifier = &ImGui::GetIO().KeyCtrl;
	initialized = true;
}

void Eclipse::NodeEditorWindow::Unload()
{
	initialized = true;
	ImNodes::PopAttributeFlag();
	ImNodes::EditorContextFree(sceneEditor.context);

	if (initialized)
	{
		sceneEditor.context = ImNodes::EditorContextCreate();
		ImNodes::PushAttributeFlag(ImNodesAttributeFlags_EnableLinkDetachWithDragClick);
		ImNodesIO& io = ImNodes::GetIO();
		io.LinkDetachWithModifierClick.Modifier = &ImGui::GetIO().KeyCtrl;
		
		initialized = false;
	}
}

void Eclipse::NodeEditorWindow::DrawImpl()
{

    sceneEditor.DrawNodeEditor("editor1", sceneEditor);

}

Eclipse::NodeEditorWindow::~NodeEditorWindow()
{
}
