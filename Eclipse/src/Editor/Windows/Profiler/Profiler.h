#pragma once
#include "../Interface/ECGuiWindow.h"

namespace Eclipse
{
	enum SystemName
	{
		LIGHTING,
		RENDER,
		CAMERA,
		COUNTER
	};
	
	class ProfilerWindow final : public ECGuiWindow
	{
		inline static float engine_time = 0.0f;
		
	public:

		Clock clock;
		
		inline static std::map<SystemName, std::vector<float>> time_container;
		
		void Update() override;
		
		ProfilerWindow();

		void DrawImpl();

		struct TimerTracker
		{
			SystemName SystemName_;
			float system_start = 0.0f;
			float system_end = 0.0f;
			float system_offset = 0.0f;
		};

		TimerTracker tracker;
		
		void ContainerAddTime(TimerTracker inputTracker);
		
		void SetName(SystemName key);
		
		float GetOffsetTime(TimerTracker inputTracker, float systemTime);

		float GetFPS();
		
		void EngineTimer(ProfilerWindow timer);
	};


}