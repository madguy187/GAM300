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
		int Play3DSounds(const std::string& sound_name, const ECVec3& vPositionfloat = ECVec3{ 0.0f, 0.0f, 0.0f }, 
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

		void SetEventParameter(const std::string& event_name, const std::string& event_parameter, float f_value);
		void SetChannelVolume(int channel_id, float f_volume);
		void SetBGMVolume(float f_volume);
		void SetSFXVolume(float f_volume);
		void SetBGMMute(bool b_mute);
		void SetSFXMute(bool b_mute);
		void SetLoop(const std::string& sound_name, bool looping);

		/*************************************************************************/
		/*                  SETTING AUDIO SOURCE DIRECTION                       */
		/*************************************************************************/
		// Setting the audio source's position and velocity
		void SetChannel3DPosition(int nChannelId, const ECVec3& vPosition, const ECVec3& vVelocity = ECVec3{ 1.0f, 0.0f, 0.0f });
		// Setting the audio source's direction
		void Set3DConeSettings(int ChannelID, float* InnerConeAngle, float* OuterConeAngle, float* OuterVolume);
		void Set3DConeOrientation(int ChannelID, const ECVec3& vOrientation = ECVec3{ 1.0f, 2.0f, 3.0f });
		// Setting the audio source's range
		void Set3DMinMaxSettings(int ChannelID, float min, float max);
		/*************************************************************************/

		/*************************************************************************/
		/*                  SETTING AUDIO SOURCE DIRECTION                       */
		/*************************************************************************/
		// Usually should be the camera...
		void Set3DListenerAttributes(Entity camEntity, const ECVec3& vVelocity = ECVec3{ 1.0f, 0.0f, 0.0f });
		/*************************************************************************/

		void SetPitch(const std::string& sound_name, float pitch);
		void SetSpeed(const std::string& sound_name, float speed);
		/*void SetLoop(const std::string& sound_name, bool looping);
		void Set3DConeSettings(const std::string& sound_name, float* InnerConeAngle,
			float* OuterConeAngle, float* OuterVolume);
		void Set3DMinMaxSettings(const std::string& sound_name, float min, float max);
		void SetPitch(const std::string& sound_name, float pitch);
		void SetSpeed(const std::string& sound_name, float speed);*/

		// Don't use, for my own personal use
		float OldBGMVolumeToTrack() const;
		float OldSFXVolumeToTrack() const;
		void OldBGMVolumeToSet(float vol);
		void OldSFXVolumeToSet(float vol);
	private:
		std::unique_ptr<FmodAPI_Impl> FmodAPI;
	};
}