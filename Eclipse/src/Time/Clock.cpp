/**********************************************************************************
* \file Clock.cpp
* \brief Code for Clock
* \author Ong, Guan Hin, 100% Code Contribution
*
* (Optional) Code for Clock
*
* \copyright Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
#include"pch.h"
#include "Clock.h"

namespace Eclipse
{
	float Clock::get_ElapsedTime()
	{
		return elapsedTime;
	}

	float Clock::get_DeltaTime()
	{
		return deltaTime;
	}

	float Clock::get_fixedDeltaTime()
	{
		return fixeddeltaTime;
	}

	int Clock::get_timeSteps()
	{
		return timeSteps;
	}

	float Clock::getFPS()
	{
		return fps;
	}

	void Clock::set_ElapsedTime(float elapsed)
	{
		elapsedTime = elapsed;
	}

	void Clock::set_DeltaTime(float delta)
	{
		deltaTime = delta;
	}

	void Clock::set_timeSteps(int timestep)
	{
		timeSteps = timestep;
	}

	void Clock::setFPS(float count)
	{
		fps = count;
	}
}