#pragma once

namespace Eclipse
{
	class ParentSystem : public System
	{
		void Update() override;
		void UpdateChildPosition(Entity parentEnt, Entity childEnt);
	};
}