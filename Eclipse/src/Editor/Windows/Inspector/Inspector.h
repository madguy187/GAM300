//#pragma once

#include "../Interface/ECGuiWindow.h"

namespace Eclipse
{
	class InspectorWindow final : public ECGuiWindow
	{
	public:
		void Update() override;
		InspectorWindow();
		void DrawImpl();

		bool ShowEntityProperty(Entity ID);
		bool ShowTransformProperty(Entity ID);
	private:
		ECVec2 WindowSize_{};
	};
}
