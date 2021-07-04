#pragma once
#include "imgui.h"
#include "../Interface/ECGuiWindow.h"

namespace Eclipse
{
	class LoggerWindow : public ECGuiWindow , public Log
	{
		Log log;
		std::vector<std::string>messages;
		bool ispushedd = false;
		ImGuiTextBuffer     Buf;
		ImGuiTextFilter     Filter;
		ImVector<int>       LineOffsets;
		bool                AutoScroll = false;
		void    Clear();
		void    AddLog(const char* fmt, ...) IM_FMTARGS(2);

		void pushMessage(const char* message);

		void isPushed();
	public:
		void Update() override;
		LoggerWindow();
		void DrawImpl();
	};

}
