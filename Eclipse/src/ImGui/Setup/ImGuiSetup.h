#pragma once

namespace Eclipse
{
	class ImGuiSetup
	{
	public:
		static void Init(bool state);
		static void Begin(bool state);
		static void End(bool state);
		static void Destroy(bool state);
	};
}