#pragma once

namespace Eclipse
{
	class ParentSystem : public System
	{
		void Update() override;
	public:
		void UpdateChildPosition(Entity parentEnt, Entity childEnt);
	};
}