#pragma once

#include "../Interface/ECGuiWindow.h"
#include "../Debug/Debug.h"
#include "ECS/ComponentManager/Components/AudioComponent.h"
#include "Editor/Windows/NodeEditor/NodeEditor.h"
namespace Eclipse
{
	struct CollisionMatrixTracker
	{
		std::unordered_map<int, bool> IndexActiveList;
		std::set<int> UnLayerTracker;
		bool IsEverything{ false };
		bool IsNothing{ true };

		void Clear()
		{
			IndexActiveList.clear();
			UnLayerTracker.clear();
			IsEverything = false;
			IsNothing = true;
		}
	};

	struct CollisionMatrixGroup
	{
		CollisionMatrixTracker Current;
	};

	class InspectorWindow final : public ECGuiWindow
	{
	public:
		void Update() override;
		void Init() override;
		void Unload() override;
		void DrawImpl();

		bool ShowTransformProperty(const char* name, Entity ID, ImGuiTextFilter& filter, bool IsNotInScene = false);
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
		bool ShowAudioProperty(const char* name, Entity ID, ImGuiTextFilter& filter);
		bool ShowCollisionProperty(const char* name, Entity ID, ImGuiTextFilter& filter);
		bool ShowPrefebProperty(Entity ID);
		bool ShowAIProperty(const char* name, Entity ID, ImGuiTextFilter& filter);
		bool ShowBPProperty(const char* name, Entity ID, ImGuiTextFilter& filter);
		bool ShowAnimationProperty(const char* name, Entity ID, ImGuiTextFilter& filter);
		bool ShowNavMeshProperty(const char* name, Entity ID, ImGuiTextFilter& filter);

		void AddComponentsController(Entity ID);
		void RemoveComponentsController(Entity ID);
		void ShowAddComponentList(Entity ID);
		void ShowRemoveComponentList(Entity ID);

		void ChangeTextureController(MaterialComponent& Item, Entity ID);
		void TextureList(MaterialComponent& Item);
		void ModelTextureList(MaterialComponent& Item);
		bool ShowParentProperty(const char* name, Entity ID, ImGuiTextFilter& filter);
		bool ShowChildProperty(const char* name, Entity ID, ImGuiTextFilter& filter);
		void ChangeMeshController(Entity ID);
		void MeshList(Entity ID);

		void SimulateAudio(Entity ID, AudioComponent& audioCom);
		void AddWaypointController(std::string& currentSelection);
		void ChangeShapeController(std::string& currentSelection);

		template <typename TComponents>
		void AddComponentsFeedback(const char* Components, const std::string& name, Entity ID, bool exist);
		
		template <typename TComponents>
		void RemoveComponentsFeedback(const char* Components, const std::string& name, Entity ID, bool exist);

		template <typename TComponents>
		void ComponentRegistry(const char* CompName, Entity ID,
			const std::string EntityName, EditComponent method);

		void OnCollisionMatrixUpdate(Entity ID);
		void UpdateCollisionLayerTracker(DebugWindow* dw, int ClickedIndex);
		void SetCollisionLayerTracker(const std::unordered_map<int, std::string>& layerlist);
		void SetScriptBitset(ScriptComponent& scriptCom, EntityComponent& entcom);
		void OnLayerListUpdate(EntityComponent& entcom);
		void LoadScriptBitset(ScriptComponent& scriptCom);

		void SetCurrentEntityName(const char* name);
		void ClearEntityName();

		static constexpr unsigned int str2int(const char* str, int h = 0);
	private:
		ECVec2 WindowSize_{};
		bool IsRemovingScripts{ false };
		char EntNameInput[256] = { 0 };
		CollisionMatrixGroup CollisionLayerChecker;
	};

	template <typename TComponents>
	void InspectorWindow::ComponentRegistry(const char* CompName, Entity ID,
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

	inline void InspectorWindow::SetCurrentEntityName(const char* name)
	{
		strcpy(EntNameInput, name);
	}

	inline void InspectorWindow::ClearEntityName()
	{
		memset(EntNameInput, 0, 255);
	}

	constexpr unsigned int InspectorWindow::str2int(const char* str, int h)
	{
		return !str[h] ? 5381 : (str2int(str, h + 1) * 33) ^ str[h];
	}
}
