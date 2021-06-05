/**********************************************************************************
* \file Clock.h
* \brief Header for clock
* \author Ong, Guan Hin, 100% Code Contribution
*
* (Optional) Code for Clock
*
* \copyright Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
#pragma once
namespace Eclipse
{
	class Clock
	{
		float elapsedTime{ 0.0f };
		float deltaTime{ 0.0f };
		const float fixeddeltaTime{ 1.0f / 60.0f };
		int timeSteps;
		float fps = 0.0f;
	public:
		float get_ElapsedTime();
		float get_DeltaTime();
		float get_fixedDeltaTime();
		int get_timeSteps();
		float getFPS();


		void set_ElapsedTime(float elapsed);
		void set_DeltaTime(float delta);
		void set_timeSteps(int timestep);
		void setFPS(float count);
	};
}
