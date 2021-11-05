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

}

void Eclipse::NodeEditorWindow::DrawImpl()
{

    sceneEditor.DrawNodeEditor("editor1", sceneEditor);

}

Eclipse::NodeEditorWindow::~NodeEditorWindow()
{
    ImNodes::PopAttributeFlag();
    ImNodes::EditorContextFree(sceneEditor.context);
}
