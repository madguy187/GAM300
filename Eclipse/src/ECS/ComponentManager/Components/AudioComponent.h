#pragma once
#include "Library/Math/Vector.h"
#include "Reflection/Meta.h"
#include "Reflection/RemTypeQual.h"

namespace Eclipse
{
	struct AudioComponent
	{
		META_DATA(AudioComponent);
		std::string AudioPath{};
		AudioType AudioType_{ AudioType::AT_SFX };
		int ChannelID{ -1 };
		float Volume{ 0.5f };
		float Pitch{ 1.0f };
		float Speed{ 1.0f };
		bool IsLooping{ false };
		bool Is3D{ false };
		bool IsMuted{ false };

		// Specifically for 3D Settings
		float InnerConeAngle{ 360.f };
		float OuterConeAngle{ 360.f };
		float OuterVolume{ 1.f };
		float Min{ 1.f };
		float Max{ 500.f };

		// Special Effects
		bool HasRadioEffect{ false };
		float RadioDistortionLevel{ 0.5f };
		bool HasEchoEffect{ false };
		float EchoDelayTime{ 500.0f }; // in ms
	};
}