#include "pch.h"
#include "AudioSystem.h"

namespace Eclipse
{
	void AudioSystem::Update()
	{
		engine->Timer.SetName({ SystemName::AUDIO });
		engine->Timer.tracker.system_start = glfwGetTime();

		if (engine->audioManager.GetBGMVolume() != engine->audioManager.OldBGMVolumeToTrack())
		{
			engine->audioManager.SetBGMVolume(engine->audioManager.GetBGMVolume());
			engine->audioManager.OldBGMVolumeToSet(engine->audioManager.GetBGMVolume());
		}

		/*if (engine->IsScenePlaying())
		{

		}
		else
		{
			engine->audioManager.Terminate();
		}*/

		engine->audioManager.UpdateSystems();


		engine->Timer.tracker.system_end = glfwGetTime();
		engine->Timer.UpdateTimeContainer(engine->Timer.tracker);
	}
}
