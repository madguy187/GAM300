#pragma once

#include "../Interface/ECGuiWindow.h"
#include "ConsoleData.h"

namespace Eclipse
{
	class LoggerWindow : public ECGuiWindow
	{
	public:
		void Update() override;
		void Init() override;
		void Unload() override;
		void DrawImpl();

		static std::shared_ptr<ConsoleData>& GetEditorLogger();
	private:
		static std::shared_ptr<ConsoleData> EditorLog_;
	};
}

/**************************************************************************************/
/*             USE THIS WHEN THINGS ARE DONE AFTER EDITOR INITIALIZATION              */
/**************************************************************************************/
// For normal messages like initialization or creating entities, etc
#define EDITOR_LOG_INFO(...) { if(engine->GetEditorState()) { ENGINE_CORE_INFO(__VA_ARGS__); Eclipse::LoggerWindow::GetEditorLogger()->AddInfoLogs(__VA_ARGS__); } }

// To predict undefined behaviour such as undefined tags, nan values
// Basically things that don't make the engine crash but may give undesirable results
#define EDITOR_LOG_WARN(...) { if(engine->GetEditorState()) { ENGINE_CORE_WARN(__VA_ARGS__); Eclipse::LoggerWindow::GetEditorLogger()->AddWarnLogs(__VA_ARGS__); } }