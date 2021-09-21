#pragma once
#include "../Interface/ECGuiWindow.h"

namespace Eclipse
{
	enum SystemName
	{
		LIGHTING,
		RENDER,
		CAMERA,
		GRID,
		PHYSICS,
		PICKING,
		EDITOR,
		COUNTER
	};

	class ProfilerWindow final : public ECGuiWindow
	{
		inline static float engine_time = 0.0f;
		
	public:

		unsigned int it = 0;

		inline static std::map<SystemName, std::vector<float>> time_container;

	/*	inline static std::vector<float> Totaltime_container;*/
		
		void Update() override;
		void Init() override;
		void Unload() override;

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
		
		float GetOffsetTime(TimerTracker inputTracker);

		void UpdateTimeContainer(TimerTracker inputTracker);

		float GetFPS();

		void PrintCpuPercentage(float value);
		
		void EngineTimer(TimerTracker timer);

	};


}