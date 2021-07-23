#include "pch.h"
#include "SerializationManager.h"

namespace Eclipse
{
	SerializationManager::SerializationManager() {}

	void SerializationManager::SerializeEntity(const Entity& ent, const size_t& counter)
	{
		sz.StartElement("Entity ", true, counter);
		SerializeAllComponents(ent);
		sz.CloseElement();
	}

	void SerializationManager::SerializeAllComponents(const Entity& ent)
	{
		World& w = engine->world;

		if (w.CheckComponent<EntityComponent>(ent))
		{
			auto& comp = w.GetComponent<EntityComponent>(ent);
			SerializeComponent<EntityComponent>(comp);
		}

		if (w.CheckComponent<CameraComponent>(ent))
		{
			auto& comp = w.GetComponent<CameraComponent>(ent);
			SerializeComponent<CameraComponent>(comp);
		}

		if (w.CheckComponent<DirectionalLightComponent>(ent))
		{
			auto& comp = w.GetComponent<DirectionalLightComponent>(ent);
			SerializeComponent<DirectionalLightComponent>(comp);
		}

		if (w.CheckComponent<MaterialComponent>(ent))
		{
			auto& comp = w.GetComponent<MaterialComponent>(ent);
			SerializeComponent<MaterialComponent>(comp);
		}

		if (w.CheckComponent<PointLightComponent>(ent))
		{
			auto& comp = w.GetComponent<PointLightComponent>(ent);
			SerializeComponent<PointLightComponent>(comp);
		}

		if (w.CheckComponent<RenderComponent>(ent))
		{
			auto& comp = w.GetComponent<RenderComponent>(ent);
			SerializeComponent<RenderComponent>(comp);
		}

		if (w.CheckComponent<RigidBodyComponent>(ent))
		{
			auto& comp = w.GetComponent<RigidBodyComponent>(ent);
			SerializeComponent<RigidBodyComponent>(comp);
		}

		if (w.CheckComponent<SpotLightComponent>(ent))
		{
			auto& comp = w.GetComponent<SpotLightComponent>(ent);
			SerializeComponent<SpotLightComponent>(comp);
		}

		if (w.CheckComponent<TransformComponent>(ent))
		{
			auto& comp = w.GetComponent<TransformComponent>(ent);
			SerializeComponent<TransformComponent>(comp);
		}
	}

	void SerializationManager::SerializeAllEntity(const char* fullpath)
	{
		std::vector<Entity>& entities = engine->editorManager->EntityHierarchyList_;
		std::filesystem::path p(fullpath);

		sz.StartElement(p.filename().replace_extension("").string());
		sz.StartElement("Entities");
		sz.AddAttributeToElement("Size", entities.size());
		size_t counter = 0;
		for (auto const& ent : entities)
		{
			SerializeEntity(ent, counter++);
		}
		sz.CloseElement();
		sz.CloseElement();
	}

	SerializationManager::~SerializationManager()
	{
		SerializeAllEntity();
	}
}