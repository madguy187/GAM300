#include "pch.h"
#include "FmodAPI.h"

namespace Eclipse
{
	void FmodAPI_Impl::Update()
	{
		std::vector<std::map<int, FMOD::Channel*>::iterator> StoppedChannels;

		for (auto start = m_Channels.begin(), end = m_Channels.end(); start != end;
			++start)
		{
			bool b_isPlaying = false;
			start->second->isPlaying(&b_isPlaying);
			if (!b_isPlaying)
				StoppedChannels.push_back(start);
		}

		for (auto& elem : StoppedChannels)
		{
			m_Channels.erase(elem);
		}

		ErrorCheck(m_StudioSystem->update());
	}

	FmodAPI_Impl::FmodAPI_Impl() :
		m_StudioSystem{ nullptr }, m_System{ nullptr }, m_NextChannel_ID{ 0 }, 
		m_Current_Vol{ 0.0f }, m_ogCurr_Vol{ 0.0f }, SFX_Vol{ 0.0f },
		oldSFX_Vol{ 0.0f }, m_BGMMute{ false }, m_SFXMute{ false }
	{
		ErrorCheck(FMOD::Studio::System::create(&m_StudioSystem, 0x00011003));
		ErrorCheck(m_StudioSystem->initialize(64, FMOD_STUDIO_INIT_NORMAL, FMOD_INIT_NORMAL, nullptr));
		ErrorCheck(FMOD::System_Create(&m_System));
		ErrorCheck(m_System->init(512, FMOD_INIT_NORMAL | FMOD_INIT_3D_RIGHTHANDED, nullptr));
	}

	FmodAPI_Impl::~FmodAPI_Impl()
	{
		ErrorCheck(m_StudioSystem->unloadAll());
		ErrorCheck(m_StudioSystem->release());
	}

	int FmodAPI_Impl::ErrorCheck(FMOD_RESULT result)
	{
		if (result != FMOD_OK)
		{
			std::cout << "FMOD ERROR" << " " << result << std::endl;
			return 1;
		}

		return 0;
	}
}