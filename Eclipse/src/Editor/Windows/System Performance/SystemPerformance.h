#pragma once
#include "../Interface/ECGuiWindow.h"

namespace Eclipse
{
	enum SystemName
	{
		test,
		counter
	};
	class PerformanceTimer final : public ECGuiWindow
	{
		ECVec2 WindowSize_{};
		
	public:
		inline static std::map<SystemName, std::vector<float>> timeContainer;
		
		void Update() override;
		
		PerformanceTimer();

		void DrawImpl();

		struct TimerTracker
		{
			SystemName SystemName_;
			float systemStart = 0.0f;
			float systemEnd = 0.0f;
			float systemOffset = 0.0f;
			float engineTimerStart = 0.0f;
			float engineTimerEnd = 0.0f;
			float engineTimerOffset = 0.0f;
		};
		TimerTracker tracker;
		
		void containerAddTime(TimerTracker inputTracker);
		
		void setName(SystemName key);
		
		float getOffsetTime(TimerTracker inputTracker);
	};


}