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
		static int i = 0;

		auto search = ProfilerWindow::time_container.find(inputTracker.SystemName_);

		inputTracker.system_offset = GetOffsetTime(inputTracker);

		++i;

		if (search != this->time_container.end())
		{
			//search->second.push_back(inputTracker.system_offset);
			if (i == 50)
			{
				search->second.push_back(inputTracker.system_offset);
				i = 0;
			}
			
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
		ECGui::BeginChildWindow({ "System Performance",ImVec2(0,500),true });
		ImGui::Dummy(ImVec2(0.0f, 10.0f));
		ImGui::TextColored(ImVec4(0.9f, 0.6f, 0.1f, 1.0f), "FPS: %.2f", GetFPS());

		//float values[100] = { 0 };
		//std::copy(ProfilerWindow::time_container[SystemName::RENDER].begin(), ProfilerWindow::time_container[SystemName::RENDER].end(), values);
		////ImGui::TextColored(ImVec4(0.9f, 0.6f, 0.1f, 1.0f), "%.2f %%", values[1]);
		//ImGui::TextColored(ImVec4(0.9f, 0.6f, 0.1f, 1.0f), "%.2f , %.2f", values[1]*100,engine_time*100);
		//ImGui::PlotHistogram("Render System", values, IM_ARRAYSIZE(values), 0, 0, 0.0f, 1.0f, ImVec2(0, 40.0f), sizeof(float));
		ECGui::PlotHistogram("Lighting System", ProfilerWindow::time_container[SystemName::LIGHTING], 0, NULL, 0.0f, 1.0f, ImVec2(0, 40.0f));
		ECGui::PlotHistogram("Render System", ProfilerWindow::time_container[SystemName::RENDER], 0, NULL, 0.0f, 1.0f, ImVec2(0, 40.0f));
		ECGui::PlotHistogram("Camera System", ProfilerWindow::time_container[SystemName::CAMERA], 0, NULL, 0.0f, 1.0f, ImVec2(0, 40.0f));
		ECGui::PlotHistogram("Grid System", ProfilerWindow::time_container[SystemName::GRID], 0, NULL, 0.0f, 1.0f, ImVec2(0, 40.0f));
		ECGui::PlotHistogram("Physics System", ProfilerWindow::time_container[SystemName::PHYSICS], 0, NULL, 0.0f, 1.0f, ImVec2(0, 40.0f));
		ECGui::PlotHistogram("Picking System", ProfilerWindow::time_container[SystemName::PICKING], 0, NULL, 0.0f, 1.0f, ImVec2(0, 40.0f));
		ECGui::PlotHistogram("Editor System", ProfilerWindow::time_container[SystemName::EDITOR], 0, NULL, 0.0f, 1.0f, ImVec2(0, 40.0f));
		ECGui::EndChildWindow();
	}

	void ProfilerWindow::SetName(SystemName key)
	{
		tracker.SystemName_ = key;
	}

	float ProfilerWindow::GetOffsetTime(TimerTracker inputTracker)
	{
		//inputTracker.engineTimerOffset = inputTracker.engineTimerEnd - inputTracker.engineTimerStart;
		inputTracker.system_offset = inputTracker.system_end - inputTracker.system_start;
		//float percentage = (inputTracker.systemOffset / inputTracker.engineTimerOffset) * 100.0f;

		return (inputTracker.system_offset)*1000;
		//return inputTracker.system_offset;
	}
	float ProfilerWindow::GetFPS()
	{
		return engine->Game_Clock.getFPS();
	}
	void ProfilerWindow::EngineTimer(TimerTracker timer)
	{
		engine_time = timer.system_end - timer.system_start;
	}
}
	
