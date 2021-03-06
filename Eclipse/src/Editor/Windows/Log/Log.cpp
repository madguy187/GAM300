#include "pch.h"
#include "Log.h"

namespace Eclipse
{
	std::shared_ptr<ConsoleData> LoggerWindow::EditorLog_;

	void LoggerWindow::Update()
	{
		if (IsVisible)
			ECGui::DrawMainWindow<void()>(WindowName, std::bind(&LoggerWindow::DrawImpl, this));
	}

	void LoggerWindow::Init()
	{
		Type = EditorWindowType::EWT_LOGGER;
		WindowName = "Log " ICON_MDI_POST;
		EditorLog_ = std::make_shared<ConsoleData>();
	}

	void LoggerWindow::Unload()
	{
	}

	void LoggerWindow::DrawImpl()
	{
		bool open = true;
		EditorLog_->DrawItems(WindowName, &open);
	}

	std::shared_ptr<ConsoleData>& LoggerWindow::GetEditorLogger()
	{
		return EditorLog_;
	}
}
	