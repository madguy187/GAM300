#pragma once

#include "Audio/FmodAPI/FmodAPI.h"

#define VOL_MAX 1.0f // Maximum volume
#define VOL_MIN 0.0f // Minimum volume

namespace Eclipse
{
	class AudioManager
	{
	public:
		void UpdateSystems();

		AudioManager() = default;
		~AudioManager();

		void Init();
		void DeserializeSounds();
		void SerializeSounds();
		void Terminate();

		void LoadSound(const std::string& sound_name, bool is3d = false, bool looping = false, bool stream = false);
		void LoadEvent(const std::string& event_name);
		void LoadBank(const std::string& bank_name, FMOD_STUDIO_LOAD_BANK_FLAGS flags);
		void UnloadSound(const std::string& sound_name);
		int Play2DSounds(const std::string& sound_name, float f_volume = 0.0f, bool looping = false);
		int Play3DSounds(const std::string& sound_name, const ECVec3& vPositionfloat = ECVec3{ 0, 0, 0 }, 
			float f_volume = 0.0f, bool looping = false);
		void PlayEvent(const std::string& event_name);
		//void Set3DListenerAndOrientation(const ECVec3& vPos = ECVec3{ 0, 0, 0 }, float fVolumedB = 0.0f);
		void StopChannel(int channel_id);
		void StopAllChannels();
		void StopEvent(const std::string& event_name, bool b_immediate = false);
		bool IsPlaying(int channel_id) const;
		bool IsEventPlaying(const std::string& event_name) const;
		bool IsEventStopped(const std::string& event_name) const;
		float DBtoVol(float f_db);
		float VoltoDB(float f_volume);
		FMOD_VECTOR VectorToFmod(const ECVec3& vPosition);

		void GetEventParameter(const std::string& event_name, const std::string& event_parameter, float* parameter);
		float GetBGMVolume() const;
		float& GetBGMVolumeByRef();
		float GetSFXVolume() const;
		float& GetSFXVolumeByRef();
		float IsBGMMuted() const;
		float IsSFXMuted() const;

		void SetChannel3DPosition(int nChannelId, const ECVec3& vPosition);
		void SetEventParameter(const std::string& event_name, const std::string& event_parameter, float f_value);
		void SetChannelVolume(int channel_id, float f_volume);
		void SetBGMVolume(float f_volume);
		void SetSFXVolume(float f_volume);
		void SetBGMMute(bool b_mute);
		void SetSFXMute(bool b_mute);
		void SetLoop(const std::string& sound_name, bool looping);
		void Set3DConeSettings(const std::string& sound_name, float* InnerConeAngle,
			float* OuterConeAngle, float* OuterVolume);
		void Set3DMinMaxSettings(const std::string& sound_name, float min, float max);

		// Don't use, for my own personal use
		float OldBGMVolumeToTrack() const;
		float OldSFXVolumeToTrack() const;
		void OldBGMVolumeToSet(float vol);
		void OldSFXVolumeToSet(float vol);
	private:
		std::unique_ptr<FmodAPI_Impl> FmodAPI;
	};
}