//#pragma once

#include "../Interface/ECGuiWindow.h"

namespace Eclipse
{
	class InspectorWindow final : public ECGuiWindow
	{
	public:
		void Update() override;
		void Unload() override;
		InspectorWindow();
		void DrawImpl();

		bool ShowEntityProperty(const char* name, Entity ID, ImGuiTextFilter& filter);
		bool ShowTransformProperty(const char* name, Entity ID, ImGuiTextFilter& filter);
		bool ShowPointLightProperty(const char* name, Entity ID, ImGuiTextFilter& filter);
		bool ShowSpotLightProperty(const char* name, Entity ID, ImGuiTextFilter& filter);
		bool ShowDirectionalLightProperty(const char* name, Entity ID, ImGuiTextFilter& filter);
		bool ShowRigidBodyProperty(const char* name, Entity ID, ImGuiTextFilter& filter);

		void AddComponentsController(Entity ID);
		void RemoveComponentsController(Entity ID);
		void AddComponents(Entity ID);
		void AddComponentsSuccess(const char* Components, EntityComponent& entCom, Entity ID);
		void AddComponentsFailed(const char* Components,EntityComponent& entCom , Entity ID);
		void RemoveComponents(Entity ID);
		void RemoveComponentsSucess(const char* Components, EntityComponent& entCom, Entity ID);
		void RemoveComponentsFailed(const char* Components, EntityComponent& entCom, Entity ID);

		static constexpr unsigned int str2int(const char* str, int h = 0)
		{

			return !str[h] ? 5381 : (str2int(str, h + 1) * 33) ^ str[h];

		}
	private:
		ECVec2 WindowSize_{};
	};

}
