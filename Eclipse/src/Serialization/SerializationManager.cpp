#include "pch.h"
#include "SerializationManager.h"

namespace Eclipse
{
	SerializationManager::SerializationManager() {}

	void SerializationManager::SerializeEntity(const Entity& ent, const size_t& counter)
	{
		sz.StartElement("Entity_", true, counter);
		sz.AddAttributeToElement("ID", ent);
		SerializeAllComponents(ent);
		sz.CloseElement();
	}

	void SerializationManager::DeserializeEntity(const size_t& counter)
	{
		if (dsz.StartElement("Entity_", true, counter))
		{
			DeserializeAllComponents(2);
			dsz.CloseElement();
		}
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

		if (w.CheckComponent<AabbComponent>(ent))
		{
			auto& comp = w.GetComponent<AabbComponent>(ent);
			SerializeComponent<AabbComponent>(comp);
		}

	}

	void SerializationManager::DeserializeAllComponents(const Entity& ent)
	{
		if (dsz.StartElement("EntityComponent"))
		{
			DeserializeComponent<EntityComponent>(ent);
			dsz.CloseElement();
		}

		if (dsz.StartElement("TransformComponent"))
		{
			DeserializeComponent<TransformComponent>(ent);
			dsz.CloseElement();
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

	void SerializationManager::DeserializeAllEntity(const char* fullpath)
	{
		std::filesystem::path p(fullpath);

		if (dsz.StartElement(p.filename().replace_extension("").string()))
		{
			if (dsz.StartElement("Entities"))
			{
				size_t size = 0;
				dsz.ReadAttributeFromElement("Size", size);

				for (size_t i = 0; i < size; ++i)
				{
					DeserializeEntity(i);
				}

				//dsz.IterateChildrenOfElement();
			}
			dsz.CloseElement();
		}
		dsz.CloseElement();
	}

	void SerializationManager::SaveFile(const char* fullpath)
	{
		sz.SaveXML(fullpath);
	}

	bool SerializationManager::LoadFile(const char* fullpath)
	{
		return dsz.LoadXML(fullpath);
	}

	SerializationManager::~SerializationManager()
	{
	}
}