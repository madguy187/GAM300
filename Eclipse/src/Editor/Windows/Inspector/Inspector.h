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
		bool ShowEditorCameraProperty(const char* name, Entity ID, ImGuiTextFilter& filter);

		void AddComponentsController(Entity ID);
		void RemoveComponentsController(Entity ID);
		void ShowAddComponentList(Entity ID);
		void ShowRemoveComponentList(Entity ID);

		template <typename TComponents>
		void AddComponentsFeedback(const char* Components, const std::string& name, Entity ID, bool exist);
		
		template <typename TComponents>
		void RemoveComponentsFeedback(const char* Components, const std::string& name, Entity ID, bool exist);

		template <typename TComponents>
		void ComponentRegistry(const char* CompName, Entity ID,
			const std::string EntityName, EditComponent method)
		{
			bool isExist = engine->world.CheckComponent<TComponents>(ID);

			if (method == EditComponent::EC_ADDCOMPONENT)
			{
				AddComponentsFeedback<TComponents>(CompName, EntityName, ID, isExist);
			}
			else
			{
				RemoveComponentsFeedback<TComponents>(CompName, EntityName, ID, isExist);
			}

			ImGui::CloseCurrentPopup();
		}

		static constexpr unsigned int str2int(const char* str, int h = 0)
		{
			return !str[h] ? 5381 : (str2int(str, h + 1) * 33) ^ str[h];
		}
	private:
		ECVec2 WindowSize_{};
	};

}
