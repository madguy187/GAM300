#include "pch.h"
#include "AudioSystem.h"

namespace Eclipse
{
	void AudioSystem::Update()
	{
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
			engine->audioManager.TerminateChannelsAndEvents();
		}*/

		engine->audioManager.UpdateSystems();
	}
}
