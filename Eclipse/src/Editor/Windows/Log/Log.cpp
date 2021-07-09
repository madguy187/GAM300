#include "pch.h"
#include "Log.h"
namespace Eclipse
{

	LoggerWindow::LoggerWindow()
	{

		Type = EditorWindowType::LOGGER;
		WindowName = "Log";
		AutoScroll = false;
		Clear();
	}

	void LoggerWindow::DrawImpl()
	{
		ImGui::PushID(this);
		if (ECGui::BeginPopUpButtonList({ "Options" , "Options" }))
		{
			ECGui::CheckBoxBool("Auto-scroll", &AutoScroll);
			ECGui::EndPopUpButtonList();
		}

		//// Main window
		//if (ECGui::ButtonBool("Options"))
		//{
		//	ImGui::OpenPopup("Options");
		//}
		ECGui::InsertSameLine();

		clear = ECGui::ButtonBool("Clear");

		ECGui::InsertSameLine();

		copy = ECGui::ButtonBool("Copy");

		ECGui::InsertSameLine();

		Filter.Draw("Filter", -100.0f);

		ECGui::InsertHorizontalLineSeperator();
		
		ECGui::DrawChildWindow<void()>({ "scrolling" ,ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar }, std::bind(&LoggerWindow::MainFunc, this));

		ImGui::PopID();
	}

	void LoggerWindow::MainFunc()
	{
		if (clear)
		{
			Clear();
			ENGINE_CORE_INFO("Log Clear");
		}

		if (copy)
		{
			ImGui::LogToClipboard();
		}

		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
		const char* buf = Buf.begin();
		const char* buf_end = Buf.end();
		if (Filter.IsActive())
		{
			for (int line_no = 0; line_no < LineOffsets.Size; line_no++)
			{
				const char* line_start = buf + LineOffsets[line_no];
				const char* line_end = (line_no + 1 < LineOffsets.Size) ? (buf + LineOffsets[line_no + 1] - 1) : buf_end;
				if (Filter.PassFilter(line_start, line_end))
					ImGui::TextUnformatted(line_start, line_end);
			}
		}
		else
		{
			ImGuiListClipper clipper;
			clipper.Begin(LineOffsets.Size);
			while (clipper.Step())
			{
				for (int line_no = clipper.DisplayStart; line_no < clipper.DisplayEnd; line_no++)
				{
					const char* line_start = buf + LineOffsets[line_no];
					const char* line_end = (line_no + 1 < LineOffsets.Size) ? (buf + LineOffsets[line_no + 1] - 1) : buf_end;
					ImGui::TextUnformatted(line_start, line_end);
				}
			}
			clipper.End();
		}
		ImGui::PopStyleVar();

		if (AutoScroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
			ImGui::SetScrollHereY(1.0f);
	}


	void LoggerWindow::Update()
	{
		if (IsVisible)
			ECGui::DrawMainWindow<void()>(WindowName, std::bind(&LoggerWindow::DrawImpl, this));

		for (static int n = 0; n < this->messages.size(); n++)
		{
			this->AddLog("%s", "test");
		}
		//this->AddLog("%s", "test");
		isPushed();
	}

	void LoggerWindow::Clear()
	{
		Buf.clear();
		LineOffsets.clear();
		LineOffsets.push_back(0);
	}

	void LoggerWindow::AddLog(const char* fmt, ...)
	{
		int old_size = Buf.size();
		va_list args;
		va_start(args, fmt);
		Buf.appendfv(fmt, args);
		va_end(args);
		for (int new_size = Buf.size(); old_size < new_size; old_size++)
		{
			if (Buf[old_size] == '\n')
			{
				LineOffsets.push_back(old_size + 1);
			}
		}
	}

	void LoggerWindow::pushMessage(const char* message)
	{
		//this->messages.push_back();
		//isPushed();
	}

	void LoggerWindow::isPushed()
	{
		if (this->ispushedd)
		{
			this->ispushedd = false;
		}
	}
}
	