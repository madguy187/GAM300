//#pragma once

#include "../Interface/ECGuiWindow.h"

namespace Eclipse
{
	class InspectorWindow final : public ECGuiWindow
	{
	public:
		void Update() override;
		void Init() override;
		void Unload() override;
		void DrawImpl();

		bool ShowEntityProperty(const char* name, Entity ID, ImGuiTextFilter& filter);
		bool ShowTransformProperty(const char* name, Entity ID, ImGuiTextFilter& filter);
		bool ShowChildTransformProperty(const char* name, Entity ID, ImGuiTextFilter& filter);
		bool ShowPointLightProperty(const char* name, Entity ID, ImGuiTextFilter& filter);
		bool ShowSpotLightProperty(const char* name, Entity ID, ImGuiTextFilter& filter);
		bool ShowDirectionalLightProperty(const char* name, Entity ID, ImGuiTextFilter& filter);
		bool ShowRigidBodyProperty(const char* name, Entity ID, ImGuiTextFilter& filter);
		bool ShowEditorCameraProperty(const char* name, Entity ID, ImGuiTextFilter& filter);
		bool ShowTextureProperty(const char* name, Entity ID, ImGuiTextFilter& filter);
		bool ShowRenderProperty(const char* name, Entity ID, ImGuiTextFilter& filter);
		bool ShowMaterialProperty(const char* name, Entity ID, ImGuiTextFilter& filter);
		bool ShowMesh3DProperty(const char* name, Entity ID, ImGuiTextFilter& filter);
		bool ShowModelInfoProperty(const char* name, Entity ID, ImGuiTextFilter& filter);
		bool ShowScriptProperty(const char* name, Entity ID, ImGuiTextFilter& filter);

		void AddComponentsController(Entity ID);
		void RemoveComponentsController(Entity ID);
		void ShowAddComponentList(Entity ID);
		void ShowRemoveComponentList(Entity ID);

		void ChangeTextureController(MaterialComponent& Item);
		void TextureList(MaterialComponent& Item);
		
		void ChangeMeshController(MeshComponent& Item);
		void MeshList(MeshComponent& Item);
		void RemoveElementFromVectorStringList(std::vector<std::string> & vecList);
		
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
		std::vector<std::string> ScriptListGuiTest;
		bool IsRemovingScripts{ false };
	};

}
