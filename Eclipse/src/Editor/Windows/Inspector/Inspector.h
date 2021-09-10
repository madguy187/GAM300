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
		void AddComponentsSuccess(const char* Components, const std::string& name, Entity ID);
		void AddComponentsFailed(const char* Components, const std::string& name, Entity ID);
		void ShowRemoveComponentList(Entity ID);
		void RemoveComponentsSuccess(const char* Components, const std::string& name, Entity ID);
		void RemoveComponentsFailed(const char* Components, const std::string& name, Entity ID);

		template <typename TComponents>
		void ComponentRegistry(const char* CompName, Entity ID,
			const std::string EntityName, EditComponent method)
		{
			if (method == EditComponent::EC_ADDCOMPONENT)
			{
				if (!engine->world.CheckComponent<TComponents>(ID))
				{
					AddComponentsSuccess(CompName, EntityName, ID);
					engine->world.AddComponent(ID, TComponents{});
				}
				else
				{
					AddComponentsFailed(CompName, EntityName, ID);
				}
			}
			else
			{
				if (engine->world.CheckComponent<TComponents>(ID))
				{
					RemoveComponentsSuccess(CompName, EntityName, ID);
					engine->world.DestroyComponent<TComponents>(ID);
				}
				else
				{
					RemoveComponentsFailed(CompName, EntityName, ID);
				}
			}
		}

		static constexpr unsigned int str2int(const char* str, int h = 0)
		{
			return !str[h] ? 5381 : (str2int(str, h + 1) * 33) ^ str[h];
		}
	private:
		ECVec2 WindowSize_{};
	};

}
