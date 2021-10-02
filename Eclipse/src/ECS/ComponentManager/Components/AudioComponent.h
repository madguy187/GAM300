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
		int ChannelID{ -1 };
		float Volume{ 0.5f };
		float Pitch{ 0.5f };
		float Speed{ 0.f };
		float Length{ 0.f };
		bool IsLooping{ false };
		bool Is3D{ false };

		// Specifically for 3D Settings
		float InnerConeAngle{ 0.f };
		float OuterConeAngle{ 0.f };
		float OuterVolume{ 0.f };
		float Min{ 0.f };
		float Max{ 0.f };
	};
}