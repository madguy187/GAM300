#include "pch.h"
#include "AudioManager.h"

namespace Eclipse
{
	void AudioManager::UpdateSystems()
	{
		FmodAPI->Update();
		FmodAPI->m_System->update();
	}

	AudioManager::~AudioManager()
	{
		// SerializeSounds();
		//StopAllChannels();
	}

	void AudioManager::Init()
	{
		FmodAPI = std::make_unique<FmodAPI_Impl>();

		// Should get from a config file but since it does not exist
		DeserializeSounds();
		// Will set the volume here for now
		FmodAPI->SFX_Vol = 0.5f;
		FmodAPI->m_Current_Vol = 0.5f;

		/*LoadBank("Assets/Audio/Master Bank.bank", FMOD_STUDIO_LOAD_BANK_NORMAL);
		LoadBank("Assets/Audio/Master Bank.strings.bank", FMOD_STUDIO_LOAD_BANK_NORMAL);*/

		ENGINE_LOG_ASSERT(FmodAPI.get(), "Failed to instantiate Audio Manager");
	}

	void AudioManager::DeserializeSounds() {}

	void AudioManager::SerializeSounds() {}

	void AudioManager::Terminate()
	{
		StopAllChannels();
		for (auto& elem : FmodAPI->m_Events)
			FmodAPI->ErrorCheck(elem.second->stop(FMOD_STUDIO_STOP_IMMEDIATE));
	}

	void AudioManager::LoadSound(const std::string& sound_name, bool is3d, bool looping, bool stream)
	{
		auto sound_it = FmodAPI->m_Sounds.find(sound_name);

		if (sound_it != FmodAPI->m_Sounds.end() && !looping)
			return;

		FMOD_MODE mode = FMOD_DEFAULT;
		mode |= is3d ? FMOD_3D : FMOD_2D;
		mode |= looping ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF;
		mode |= stream ? FMOD_CREATESTREAM : FMOD_CREATECOMPRESSEDSAMPLE; 

		FMOD::Sound* sound_ptr = nullptr;

		FmodAPI->ErrorCheck(FmodAPI->m_System->createSound(sound_name.c_str(), mode, 0, &sound_ptr));

		if (sound_ptr)
			FmodAPI->m_Sounds[sound_name] = sound_ptr;
	}

	void AudioManager::LoadEvent(const std::string& event_name)
	{
		auto event_it = FmodAPI->m_Events.find(event_name);

		if (event_it != FmodAPI->m_Events.end() && FmodAPI->m_Events[event_name])
			return;

		FMOD::Studio::EventDescription* event_desc_ptr = nullptr;
		FmodAPI->ErrorCheck(FmodAPI->m_StudioSystem->getEvent(event_name.c_str(), &event_desc_ptr));

		if (event_desc_ptr)
		{
			FMOD::Studio::EventInstance* event_instance_ptr = nullptr;
			FmodAPI->ErrorCheck(event_desc_ptr->createInstance(&event_instance_ptr));

			if (event_instance_ptr)
				FmodAPI->m_Events[event_name] = event_instance_ptr;
		}
	}

	void AudioManager::LoadBank(const std::string& bank_name, FMOD_STUDIO_LOAD_BANK_FLAGS flags)
	{
		auto bank_it = FmodAPI->m_Banks.find(bank_name);

		if (bank_it != FmodAPI->m_Banks.end())
			return;

		FMOD::Studio::Bank* bank_ptr = nullptr;
		FmodAPI->ErrorCheck(FmodAPI->m_StudioSystem->loadBankFile(bank_name.c_str(), flags, &bank_ptr));

		if (bank_ptr)
			FmodAPI->m_Banks[bank_name] = bank_ptr;
	}

	void AudioManager::UnloadSound(const std::string& sound_name)
	{
		auto sound_it = FmodAPI->m_Sounds.find(sound_name);

		if (sound_it == FmodAPI->m_Sounds.end())
			return;

		FmodAPI->ErrorCheck(sound_it->second->release());
		FmodAPI->m_Sounds.erase(sound_it);
	}

	int AudioManager::Play2DSounds(const std::string& sound_name, float f_volume, bool looping)
	{
		int channel_id = FmodAPI->m_NextChannel_ID++;
		auto sound_it = FmodAPI->m_Sounds.find(sound_name);

		if (sound_it == FmodAPI->m_Sounds.end() || looping)
		{
			LoadSound(sound_name, false, looping);
			sound_it = FmodAPI->m_Sounds.find(sound_name);

			if (sound_it == FmodAPI->m_Sounds.end())
				return channel_id;
		}

		FMOD::Channel* channel_ptr = nullptr;
		FmodAPI->ErrorCheck(FmodAPI->m_System->playSound(sound_it->second, nullptr, true, &channel_ptr));

		if (channel_ptr)
		{
			FMOD_MODE currMode;
			sound_it->second->getMode(&currMode);

			if (f_volume <= 0.0f)
			{
				f_volume = FmodAPI->SFX_Vol;
			}

			FmodAPI->ErrorCheck(channel_ptr->setVolume(f_volume));
			FmodAPI->ErrorCheck(channel_ptr->setPaused(false));

			FmodAPI->m_Channels[channel_id] = channel_ptr;
		}

		return channel_id;
	}

	int AudioManager::Play3DSounds(const std::string& sound_name, const ECVec3& vPosition, 
		float f_volume, bool looping)
	{
		int channel_id = FmodAPI->m_NextChannel_ID++;
		auto sound_it = FmodAPI->m_Sounds.find(sound_name);

		if (sound_it == FmodAPI->m_Sounds.end() /*|| looping*/)
		{
			LoadSound(sound_name, true, looping);
			sound_it = FmodAPI->m_Sounds.find(sound_name);

			if (sound_it == FmodAPI->m_Sounds.end())
				return channel_id;
		}

		FMOD::Channel* channel_ptr = nullptr;
		FmodAPI->ErrorCheck(FmodAPI->m_System->playSound(sound_it->second, nullptr, true, &channel_ptr));

		if (channel_ptr)
		{
			FMOD_MODE currMode;
			sound_it->second->getMode(&currMode);

			if (currMode & FMOD_3D)
			{
				FMOD_VECTOR position = VectorToFmod(vPosition);
				FmodAPI->ErrorCheck(channel_ptr->set3DAttributes(&position, nullptr));
			}

			if (f_volume <= 0.0f)
			{
				f_volume = FmodAPI->SFX_Vol;
			}

			FmodAPI->ErrorCheck(channel_ptr->setVolume(f_volume));
			FmodAPI->ErrorCheck(channel_ptr->setPaused(false));

			FmodAPI->m_Channels[channel_id] = channel_ptr;
		}

		return channel_id;
	}

	void AudioManager::PlayEvent(const std::string& event_name)
	{
		auto event_it = FmodAPI->m_Events.find(event_name);
		if (event_it == FmodAPI->m_Events.end())
		{
			LoadEvent(event_name);
			event_it = FmodAPI->m_Events.find(event_name);
			if (event_it == FmodAPI->m_Events.end())
				return;
		}

		event_it->second->setVolume(FmodAPI->m_BGMMute ? 0.0f : FmodAPI->m_Current_Vol);
		event_it->second->start();
	}

	void AudioManager::StopChannel(int channel_id)
	{
		if (IsPlaying(channel_id))
			FmodAPI->m_Channels[channel_id]->stop();
	}

	void AudioManager::StopAllChannels()
	{
		for (auto& elem : FmodAPI->m_Channels)
			StopChannel(elem.first);
	}

	void AudioManager::StopEvent(const std::string& event_name, bool b_immediate)
	{
		auto event_it = FmodAPI->m_Events.find(event_name);

		if (event_it != FmodAPI->m_Events.end())
		{
			FMOD_STUDIO_STOP_MODE mode;
			mode = b_immediate ? FMOD_STUDIO_STOP_IMMEDIATE : FMOD_STUDIO_STOP_ALLOWFADEOUT;
			FmodAPI->ErrorCheck(event_it->second->stop(mode));
		}
	}

	bool AudioManager::IsPlaying(int channel_id) const
	{
		bool result = false;

		if (FmodAPI->m_Channels.find(channel_id) != FmodAPI->m_Channels.end())
			FmodAPI->m_Channels[channel_id]->isPlaying(&result);

		return result;
	}

	bool AudioManager::IsEventPlaying(const std::string& event_name) const
	{
		auto event_it = FmodAPI->m_Events.find(event_name);
		if (event_it == FmodAPI->m_Events.end())
			return false;

		FMOD_STUDIO_PLAYBACK_STATE state;
		event_it->second->getPlaybackState(&state);

		if (state == FMOD_STUDIO_PLAYBACK_PLAYING)
			return true;

		return false;
	}

	bool AudioManager::IsEventStopped(const std::string& event_name) const
	{
		auto event_it = FmodAPI->m_Events.find(event_name);

		if (event_it == FmodAPI->m_Events.end())
			return false;

		FMOD_STUDIO_PLAYBACK_STATE state;
		event_it->second->getPlaybackState(&state);

		if (state == FMOD_STUDIO_PLAYBACK_STOPPED)
			return true;

		return false;
	}

	float AudioManager::DBtoVol(float f_db)
	{
		return std::powf(10.0f, 0.05f * f_db);
	}

	float AudioManager::VoltoDB(float f_volume)
	{
		return 20.0f * log10f(f_volume);
	}

	FMOD_VECTOR AudioManager::VectorToFmod(const ECVec3& vPosition)
	{
		FMOD_VECTOR fVec;
		fVec.x = vPosition.getX();
		fVec.y = vPosition.getY();
		fVec.z = vPosition.getZ();
		return fVec;
	}

	void AudioManager::GetEventParameter(const std::string& event_name, const std::string& event_parameter, float* parameter)
	{
		auto event_it = FmodAPI->m_Events.find(event_name);

		if (event_it != FmodAPI->m_Events.end())
		{
			FMOD::Studio::ParameterInstance* param_ptr = nullptr;
			FmodAPI->ErrorCheck(event_it->second->getParameter(event_parameter.c_str(), &param_ptr));
			FmodAPI->ErrorCheck(param_ptr->getValue(parameter));
		}
	}

	float AudioManager::GetBGMVolume() const
	{
		return FmodAPI->m_Current_Vol;
	}

	float& AudioManager::GetBGMVolumeByRef()
	{
		return FmodAPI->m_Current_Vol;
	}

	float AudioManager::GetSFXVolume() const
	{
		return FmodAPI->SFX_Vol;
	}

	float& AudioManager::GetSFXVolumeByRef()
	{
		return FmodAPI->SFX_Vol;
	}

	float AudioManager::IsBGMMuted() const
	{
		return FmodAPI->m_BGMMute;
	}

	float AudioManager::IsSFXMuted() const
	{
		return FmodAPI->m_SFXMute;
	}

	void AudioManager::SetChannel3DPosition(int nChannelId, const ECVec3& vPosition)
	{
		auto tFoundIt = FmodAPI->m_Channels.find(nChannelId);
		if (tFoundIt == FmodAPI->m_Channels.end())
			return;

		FMOD_VECTOR position = VectorToFmod(vPosition);
		FmodAPI->ErrorCheck(tFoundIt->second->set3DAttributes(&position, NULL));
	}

	void AudioManager::SetEventParameter(const std::string& event_name, const std::string& event_parameter, float f_value)
	{
		auto event_it = FmodAPI->m_Events.find(event_name);

		if (event_it != FmodAPI->m_Events.end())
		{
			FMOD::Studio::ParameterInstance* param_ptr = nullptr;
			FmodAPI->ErrorCheck(event_it->second->getParameter(event_parameter.c_str(), &param_ptr));
			FmodAPI->ErrorCheck(param_ptr->setValue(f_value));
		}
	}

	void AudioManager::SetChannelVolume(int channel_id, float f_volume)
	{
		auto channel_it = FmodAPI->m_Channels.find(channel_id);

		if (channel_it != FmodAPI->m_Channels.end())
			FmodAPI->ErrorCheck(channel_it->second->setVolume(DBtoVol(f_volume)));
	}

	void AudioManager::SetBGMVolume(float f_volume)
	{
		if (f_volume >= VOL_MAX)
			f_volume = VOL_MAX;
		else if (f_volume <= VOL_MIN)
			f_volume = VOL_MIN;

		FmodAPI->m_Current_Vol = f_volume;

		for (auto& elem : FmodAPI->m_Events)
		{
			FmodAPI->ErrorCheck(elem.second->setVolume(FmodAPI->m_BGMMute ? 0.0f : f_volume));
		}
	}

	void AudioManager::SetSFXVolume(float f_volume)
	{
		if (f_volume >= VOL_MAX)
			f_volume = VOL_MAX;
		else if (f_volume <= VOL_MIN)
			f_volume = VOL_MIN;

		FmodAPI->SFX_Vol = f_volume;
		FmodAPI->oldSFX_Vol = f_volume;
	}

	void AudioManager::SetBGMMute(bool b_mute)
	{
		if (FmodAPI.get())
		{
			FmodAPI->m_BGMMute = b_mute;

			for (auto& elem : FmodAPI->m_Events)
			{
				FmodAPI->ErrorCheck(elem.second->setVolume(b_mute ? 0.0f : FmodAPI->m_Current_Vol));
			}
		}
	}

	void AudioManager::SetSFXMute(bool b_mute)
	{
		if (FmodAPI.get())
		{
			FmodAPI->m_SFXMute = b_mute;

			if (b_mute)
			{
				FmodAPI->SFX_Vol = 0.0f;
			}
			else
			{
				FmodAPI->SFX_Vol = FmodAPI->oldSFX_Vol;
			}
		}
	}

	void AudioManager::SetLoop(const std::string& sound_name, bool looping)
	{
		auto sound_it = FmodAPI->m_Sounds.find(sound_name);

		if (sound_it != FmodAPI->m_Sounds.end())
		{
			FMOD_MODE* mode = nullptr;
			sound_it->second->getMode(mode);

			if (mode != nullptr)
			{
				*mode |= looping ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF;
			}
		}
		else
		{
			ENGINE_CORE_WARN("Sound does not exist.");
		}
	}

	void AudioManager::Set3DConeSettings(const std::string& sound_name, float* InnerConeAngle, 
		float* OuterConeAngle, float* OuterVolume)
	{
		FmodAPI->m_Sounds[sound_name]->get3DConeSettings(InnerConeAngle, OuterConeAngle, OuterVolume);
	}

	void AudioManager::Set3DMinMaxSettings(const std::string& sound_name, float min, float max)
	{
		FmodAPI->m_Sounds[sound_name]->set3DMinMaxDistance(min, max);
	}

	void AudioManager::SetPitch(const std::string& sound_name, float pitch)
	{
		(void)sound_name;
		(void)pitch;
	}

	void AudioManager::SetSpeed(const std::string& sound_name, float speed)
	{
		FmodAPI->m_Sounds[sound_name]->setMusicSpeed(speed);
	}

	float AudioManager::OldBGMVolumeToTrack() const
	{
		return FmodAPI->m_ogCurr_Vol;
	}

	float AudioManager::OldSFXVolumeToTrack() const
	{
		return FmodAPI->oldSFX_Vol;
	}

	void AudioManager::OldBGMVolumeToSet(float vol)
	{
		FmodAPI->m_ogCurr_Vol = vol;
	}

	void AudioManager::OldSFXVolumeToSet(float vol)
	{
		FmodAPI->oldSFX_Vol = vol;
	}
}
