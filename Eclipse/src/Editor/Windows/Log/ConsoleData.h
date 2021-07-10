#pragma once

struct ConsoleData
{
    ImGuiTextBuffer _Buffer;
    ImGuiTextFilter _Filter;
    // Index to lines offset. We maintain this with AddLog() calls.
    std::vector<int> _LineOffsets;
    // Keep scrolling if already at the bottom.
    bool _AutoScroll;

    ConsoleData()
    {
        _AutoScroll = true;
        Clear();
    }

    void Clear()
    {
        _Buffer.clear();
        _LineOffsets.clear();
        _LineOffsets.push_back(0);
    }

    std::string currentDateTime()
    {
        time_t     now = time(0);
        struct tm  tstruct;
        char       buf[80];
        tstruct = *localtime(&now);
        strftime(buf, sizeof(buf), "[%m/%d/%Y %X]", &tstruct);

        return buf;
    }

    void AddLogs(const char* fmt, ...) IM_FMTARGS(2)
    {
        // Set to SPDLOG Format
        std::string logmsg = currentDateTime();
        logmsg += " [EDITOR]";
        logmsg += " [info] ";
        logmsg += fmt;
        logmsg += "\n";
        const char* msg = logmsg.c_str();

        int old_size = _Buffer.size();
        va_list args;
        va_start(args, msg);
        _Buffer.appendfv(msg, args);
        va_end(args);
        for (int new_size = _Buffer.size(); old_size < new_size; old_size++)
            if (_Buffer[old_size] == '\n')
                _LineOffsets.push_back(old_size + 1);
    }

    void DrawItems(const char* title, bool* p_open = NULL)
    {
        if (!ImGui::Begin(title, p_open))
        {
            ImGui::End();
            return;
        }

        // Options menu
        if (ImGui::BeginPopup("Options"))
        {
            ImGui::Checkbox("Auto-scroll", &_AutoScroll);
            ImGui::EndPopup();
        }

        // Main window
        if (ImGui::Button("Options"))
            ImGui::OpenPopup("Options");

        ImGui::SameLine();
        bool clear = ImGui::Button("Clear");
        ImGui::SameLine();
        bool copy = ImGui::Button("Copy");
        ImGui::SameLine();
        _Filter.Draw("Filter", -100.0f);

        ImGui::Separator();
        ImGui::BeginChild("scrolling", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);

        if (clear)
            Clear();
        if (copy)
            ImGui::LogToClipboard();

        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
        const char* buf = _Buffer.begin();
        const char* buf_end = _Buffer.end();

        if (_Filter.IsActive())
        {
            for (size_t line_no = 0; line_no < _LineOffsets.size(); line_no++)
            {
                const char* line_start = buf + _LineOffsets[line_no];
                const char* line_end = (line_no + 1 < _LineOffsets.size()) ? (buf + _LineOffsets[line_no + 1] - 1) : buf_end;
                if (_Filter.PassFilter(line_start, line_end))
                    ImGui::TextUnformatted(line_start, line_end);
            }
        }
        else
        {
            ImGuiListClipper clipper;
            clipper.Begin(static_cast<int>(_LineOffsets.size()));
            while (clipper.Step())
            {
                for (size_t line_no = clipper.DisplayStart; line_no < clipper.DisplayEnd; line_no++)
                {
                    const char* line_start = buf + _LineOffsets[line_no];
                    const char* line_end = (line_no + 1 < _LineOffsets.size()) ? (buf + _LineOffsets[line_no + 1] - 1) : buf_end;
                    ImGui::TextUnformatted(line_start, line_end);
                }
            }
            clipper.End();
        }
        ImGui::PopStyleVar();

        if (_AutoScroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
            ImGui::SetScrollHereY(1.0f);

        ImGui::EndChild();
        ImGui::End();
    }
};
