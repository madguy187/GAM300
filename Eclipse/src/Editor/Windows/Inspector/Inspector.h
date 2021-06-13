//#pragma once

#include "../Interface/ECGuiWindow.h"

namespace Eclipse
{
	class Inspector final : public ECGuiWindow
	{
	public:
		void Update() override;
		Inspector();
		void DrawImpl();

		bool ShowEntityProperty(Entity ID);
		bool ShowTransformProperty(Entity ID);
	private:
		ECVec2 WindowSize_{};
	};
}
