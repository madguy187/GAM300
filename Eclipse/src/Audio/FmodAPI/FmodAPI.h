#pragma once

#include "Global.h"
#include "fmod_studio.hpp"
#include "fmod.hpp"

namespace Eclipse
{
	struct FmodAPI_Impl
	{
		void Update();

		FmodAPI_Impl();
		~FmodAPI_Impl();
		int ErrorCheck(FMOD_RESULT result);

		FMOD::Studio::System* m_StudioSystem;
		FMOD::System* m_System;
		FMOD::ChannelGroup* channelGroup = nullptr;

		int m_NextChannel_ID;
		float m_Current_Vol;
		float m_ogCurr_Vol;
		float SFX_Vol;
		float oldSFX_Vol;
		bool m_BGMMute;
		bool m_SFXMute;

		std::map<std::string, FMOD::Sound*> m_Sounds;
		std::map<int, FMOD::Channel*> m_Channels;
		std::map<std::string, FMOD::Studio::EventInstance*> m_Events;
		std::map<std::string, FMOD::Studio::Bank*> m_Banks;
	};
}
