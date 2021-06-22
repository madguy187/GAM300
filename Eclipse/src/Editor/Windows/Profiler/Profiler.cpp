#include "pch.h"

void Eclipse::Profiler::Update()
{
	if (IsVisible)
		ECGui::DrawMainWindow<void()>(WindowName, std::bind(&Profiler::DrawImpl, this));
}

Eclipse::Profiler::Profiler()
{
	Type = EditorWindowType::PROFILER;
	WindowName = "Profiler";
}

void Eclipse::Profiler::ContainerAddTime(TimerTracker inputTracker)
{
	auto search = Profiler::time_container.find(inputTracker.SystemName_);

 	inputTracker.system_offset = GetOffsetTime(inputTracker, engine_time);
	
		if (search != this->time_container.end())
		{
			search->second.push_back(inputTracker.system_offset);
			if(search->second.size() == 100)
			{
				search->second.clear();
			}
		}
		else
		{
			Profiler::time_container.emplace(inputTracker.SystemName_, std::vector<float>{inputTracker.system_offset});
		}
	
}

void Eclipse::Profiler::DrawImpl()
{
	//static float* pvalues= &PerformanceTimer::timeContainer[SystemName::egame][0] ;
	ECGui::BeginChildWindow({ "System Performance",ImVec2(0,500),true });
	ImGui::Dummy(ImVec2(0.0f, 10.0f));
	ImGui::TextColored(ImVec4(0.9f, 0.6f, 0.1f, 1.0f), "FPS: %.2f", GetFPS());
	ECGui::PlotHistogram("Lighting System", Profiler::time_container[SystemName::LIGHTING], 0, NULL, 0.0f, 1.0f, ImVec2(0, 40.0f));
	ECGui::PlotHistogram("Render System", Profiler::time_container[SystemName::RENDER], 0, NULL, 0.0f, 1.0f, ImVec2(0, 40.0f));
	ECGui::PlotHistogram("Camera System", Profiler::time_container[SystemName::CAMERA], 0, NULL, 0.0f, 1.0f, ImVec2(0, 40.0f));
	ECGui::EndChildWindow();
}

void Eclipse::Profiler::SetName(SystemName key)
{
	tracker.SystemName_ = key;
}

float Eclipse::Profiler::GetOffsetTime(TimerTracker inputTracker,float systemTime)
{
	//inputTracker.engineTimerOffset = inputTracker.engineTimerEnd - inputTracker.engineTimerStart;
	inputTracker.system_offset =  inputTracker.system_end - inputTracker.system_start;
	//float percentage = (inputTracker.systemOffset / inputTracker.engineTimerOffset) * 100.0f;
	
	return (inputTracker.system_offset / systemTime)*100;
}
float Eclipse::Profiler::GetFPS()
{
	
	return clock.getFPS();
}
void Eclipse::Profiler::EngineTimer(Profiler timer)
{
	timer.tracker.system_offset = timer.tracker.system_end - timer.tracker.system_start;
	engine_time = timer.tracker.system_offset;
}

