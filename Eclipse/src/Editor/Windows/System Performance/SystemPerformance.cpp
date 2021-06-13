#include "pch.h"

void Eclipse::PerformanceTimer::Update()
{
	if (IsVisible)
		ECGui::DrawMainWindow<void()>(WindowName, std::bind(&PerformanceTimer::DrawImpl, this));
}

Eclipse::PerformanceTimer::PerformanceTimer()
{
	Type = EditorWindowType::SYSPERFORMANCE;
	WindowName = "Performance";
}

void Eclipse::PerformanceTimer::containerAddTime(TimerTracker inputTracker)
{
	auto search =PerformanceTimer::timeContainer.find(inputTracker.SystemName_);

 	inputTracker.systemOffset = getOffsetTime(inputTracker);
	
		if (search != this->timeContainer.end())
		{
			search->second.push_back(inputTracker.systemOffset);
			if(search->second.size() == 100)
			{
				search->second.clear();
			}
		}
		else
		{

			PerformanceTimer::timeContainer.emplace(inputTracker.SystemName_, std::vector<float>{inputTracker.systemOffset});
		}
	
}

void Eclipse::PerformanceTimer::DrawImpl()
{
	//static float* pvalues= &PerformanceTimer::timeContainer[SystemName::egame][0] ;
	ECGui::BeginChildWindow({ "System Performance",ImVec2(0,500),true });
	ImGui::Dummy(ImVec2(0.0f, 10.0f));
	ECGui::PlotHistogram("testing functionnality", PerformanceTimer::timeContainer[SystemName::test], 0, NULL, 0.0f, 1.0f, ImVec2(0, 80.0f));
	ECGui::EndChildWindow();
}

void Eclipse::PerformanceTimer::setName(SystemName key)
{
	tracker.SystemName_ = key;
}

float Eclipse::PerformanceTimer::getOffsetTime(TimerTracker inputTracker)
{
	//inputTracker.engineTimerOffset = inputTracker.engineTimerEnd - inputTracker.engineTimerStart;
	inputTracker.systemOffset =  inputTracker.systemEnd - inputTracker.systemStart;
	//float percentage = (inputTracker.systemOffset / inputTracker.engineTimerOffset) * 100.0f;
	
	return inputTracker.systemOffset*10000;
}

