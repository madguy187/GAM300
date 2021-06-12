#pragma once

#include "../Interface/ECGuiWindow.h"

namespace Eclipse
{
	struct EntitySelectionTracker
	{
		Entity index{ 0 };
		std::string name{};
	};

	class Hierarchy final : public ECGuiWindow
	{
	public:
		void Update() override;
		Hierarchy();
		void DrawImpl();
	private:
	};
}
