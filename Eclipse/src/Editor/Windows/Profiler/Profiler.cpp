#include "pch.h"

namespace Eclipse
{
	void ProfilerWindow::Update()
	{
		if (IsVisible)
			ECGui::DrawMainWindow<void()>(WindowName, std::bind(&ProfilerWindow::DrawImpl, this));
	}

	void ProfilerWindow::Init()
	{
		Type = EditorWindowType::EWT_PROFILER;
		WindowName = "Profiler";
		IsVisible = false;
	}

	void ProfilerWindow::Unload()
	{
	}

	void ProfilerWindow::ContainerAddTime(TimerTracker inputTracker)
	{
	
		auto search = ProfilerWindow::time_container.find(inputTracker.SystemName_);

		inputTracker.system_offset = GetOffsetTime(inputTracker);

		if (search != this->time_container.end())
		{
			//std::cout << inputTracker.system_offset<<std::endl;
			search->second.push_back(inputTracker.system_offset);

			if (search->second.size() == 50)
			{
				search->second.clear();
			}
		}
		else
		{
			ProfilerWindow::time_container.emplace(inputTracker.SystemName_, std::vector<float>{inputTracker.system_offset});
		}

	}

	void ProfilerWindow::DrawImpl()
	{
		//static float* pvalues= &PerformanceTimer::timeContainer[SystemName::egame][0] ;
		ECGui::BeginChildWindow({ "System Performance",ImVec2(0,800),true });
		ImGui::Dummy(ImVec2(0.0f, 10.0f));
		ImGui::TextColored(ImVec4(0.9f, 0.6f, 0.1f, 1.0f), "FPS: %.2f", GetFPS());
		ECGui::InsertHorizontalLineSeperator();
		//ImGui::Columns(2, NULL, true);

		EngineTimer();

		ECGui::PlotHistogram("Lighting System", ProfilerWindow::time_container[SystemName::LIGHTING], 0, NULL, 0.0f, 1.0f, ImVec2(0, 40.0f));
		ECGui::InsertHorizontalLineSeperator();
		ECGui::PlotHistogram("Render System", ProfilerWindow::time_container[SystemName::RENDER], 0, NULL, 0.0f, 1.0f, ImVec2(0, 40.0f));
		ECGui::InsertHorizontalLineSeperator();
		ECGui::PlotHistogram("Camera System", ProfilerWindow::time_container[SystemName::CAMERA], 0, NULL, 0.0f, 1.0f, ImVec2(0, 40.0f));
		ECGui::InsertHorizontalLineSeperator();
		ECGui::PlotHistogram("Grid System", ProfilerWindow::time_container[SystemName::GRID], 0, NULL, 0.0f, 1.0f, ImVec2(0, 40.0f));
		ECGui::InsertHorizontalLineSeperator();
		ECGui::PlotHistogram("Physics System", ProfilerWindow::time_container[SystemName::PHYSICS], 0, NULL, 0.0f, 1.0f, ImVec2(0, 40.0f));
		ECGui::InsertHorizontalLineSeperator();
		ECGui::PlotHistogram("Picking System", ProfilerWindow::time_container[SystemName::PICKING], 0, NULL, 0.0f, 1.0f, ImVec2(0, 40.0f));
		ECGui::InsertHorizontalLineSeperator();
		ECGui::PlotHistogram("Editor System", ProfilerWindow::time_container[SystemName::EDITOR], 0, NULL, 0.0f, 1.0f, ImVec2(0, 40.0f));
		ECGui::InsertHorizontalLineSeperator();
		ECGui::PlotHistogram("Material System", ProfilerWindow::time_container[SystemName::MATERIAL], 0, NULL, 0.0f, 1.0f, ImVec2(0, 40.0f));
		ECGui::InsertHorizontalLineSeperator();
		ECGui::PlotHistogram("Audio System", ProfilerWindow::time_container[SystemName::AUDIO], 0, NULL, 0.0f, 1.0f, ImVec2(0, 40.0f));
		ECGui::InsertHorizontalLineSeperator();
		ECGui::PlotHistogram("Mono System", ProfilerWindow::time_container[SystemName::MONO], 0, NULL, 0.0f, 1.0f, ImVec2(0, 40.0f));
		ECGui::InsertHorizontalLineSeperator();
		ECGui::PlotHistogram("FileWatch System", ProfilerWindow::time_container[SystemName::FILEWATCH], 0, NULL, 0.0f, 1.0f, ImVec2(0, 40.0f));
		//ImGui::NextColumn();
		//ImGui::TextColored(ImVec4(0.9f, 0.6f, 0.1f, 1.0f), "%.2f %%", ((ProfilerWindow::time_container[SystemName::LIGHTING][0]) / (engine_time)));
		ECGui::EndChildWindow();

		if (ECGui::ButtonBool("More Details"))
		{
			system("start src/Tracy_Server/Tracy.exe");
		}

	}

	void ProfilerWindow::SetName(SystemName key)
	{
		tracker.SystemName_ = key;
	}

	float ProfilerWindow::GetOffsetTime(TimerTracker inputTracker)
	{
		//inputTracker.engineTimerOffset = inputTracker.engineTimerEnd - inputTracker.engineTimerStart;
		inputTracker.system_offset = inputTracker.system_end - inputTracker.system_start;

		engine_time = inputTracker.system_offset*1000;
		//float percentage = (inputTracker.systemOffset / inputTracker.engineTimerOffset) * 100.0f;

		return engine_time;
		//return inputTracker.system_offset;
	}
	void ProfilerWindow::UpdateTimeContainer(TimerTracker inputTracker)
	{

		ContainerAddTime(inputTracker);
	}
	float ProfilerWindow::GetFPS()
	{
		return engine->Game_Clock.getFPS();
	}

	void ProfilerWindow::EngineTimer()
	{
		for(auto& it : time_container)
		{
			if(!it.second.empty())
			{
				engine_time += it.second[0];
			}
		}
	}


	void ProfilerWindow::PrintCpuPercentage(float value)
	{
		std::stringstream stream;
		float temp = value / ProfilerWindow::engine_time * 100.f;
		stream << std::fixed << std::setprecision(2) << temp;
		std::string tempstr = { "CUP USAGE :" };
		tempstr += stream.str() + "%";
		ImGui::TextColored(ImVec4(0.9f, 0.6f, 0.1f, 1.0f), tempstr.c_str());
	}
}
	
