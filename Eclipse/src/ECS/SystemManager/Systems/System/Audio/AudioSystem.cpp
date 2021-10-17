#include "pch.h"
#include "AudioSystem.h"

namespace Eclipse
{
	void AudioSystem::Update()
	{
		ZoneScopedN("Audio System")
		engine->Timer.SetName({ SystemName::AUDIO });
		engine->Timer.tracker.system_start = static_cast<float>(glfwGetTime());

		if (engine->audioManager.GetBGMVolume() != engine->audioManager.OldBGMVolumeToTrack())
		{
			engine->audioManager.SetBGMVolume(engine->audioManager.GetBGMVolume());
			engine->audioManager.OldBGMVolumeToSet(engine->audioManager.GetBGMVolume());
		}

		for (auto const& entity : mEntities)
		{
			auto& audioCom = engine->world.GetComponent<AudioComponent>(entity);
			auto& transCom = engine->world.GetComponent<TransformComponent>(entity);

			if (engine->audioManager.IsPlaying(audioCom.ChannelID) && audioCom.Is3D)
				engine->audioManager.SetChannel3DPosition(audioCom.ChannelID, transCom.position);
		}

		engine->audioManager.Set3DListenerAttributes(engine->gCamera.GetEditorCameraID());
		engine->audioManager.UpdateSystems();

		engine->Timer.tracker.system_end = static_cast<float>(glfwGetTime());
		engine->Timer.UpdateTimeContainer(engine->Timer.tracker);
		FrameMark
	}
}
