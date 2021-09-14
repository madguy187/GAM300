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
			Entity ent = engine->world.CreateEntity();
			DeserializeAllComponents(ent);
			engine->editorManager->RegisterExistingEntity(ent);
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

		if (w.CheckComponent<MeshComponent>(ent))
		{
			auto& comp = w.GetComponent<MeshComponent>(ent);
			SerializeComponent<MeshComponent>(comp);
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

		if (w.CheckComponent<AABBComponent>(ent))
		{
			auto& comp = w.GetComponent<AABBComponent>(ent);
			SerializeComponent<AABBComponent>(comp);
		}

		if (w.CheckComponent<TextureComponent>(ent))
		{
			auto& comp = w.GetComponent<TextureComponent>(ent);
			SerializeComponent<TextureComponent>(comp);
		}
	}

	void SerializationManager::DeserializeAllComponents(const Entity& ent)
	{
		if (dsz.StartElement("EntityComponent"))
		{
			EntityComponent comp;
			DeserializeComponent<EntityComponent>(ent, comp);
			engine->world.AddComponent(ent, comp);
			dsz.CloseElement();
		}

		if (dsz.StartElement("TransformComponent"))
		{
			TransformComponent comp;
			DeserializeComponent<TransformComponent>(ent, comp);
			engine->world.AddComponent(ent, comp);
			dsz.CloseElement();
		}

		if (dsz.StartElement("MeshComponent"))
		{
			MeshComponent comp;
			DeserializeComponent<MeshComponent>(ent, comp);
			engine->world.AddComponent(ent, comp);
			dsz.CloseElement();
		}

		if (dsz.StartElement("MaterialComponent"))
		{
			MaterialComponent comp;
			DeserializeComponent<MaterialComponent>(ent, comp);
			engine->world.AddComponent(ent, comp);
			dsz.CloseElement();
		}

		if (dsz.StartElement("AABBComponent"))
		{
			AABBComponent comp;
			DeserializeComponent<AABBComponent>(ent, comp);
			engine->world.AddComponent(ent, comp);
			dsz.CloseElement();
		}

		if (dsz.StartElement("RigidBodyComponent"))
		{
			RigidBodyComponent comp;
			DeserializeComponent<RigidBodyComponent>(ent, comp);
			engine->world.AddComponent(ent, comp);
			dsz.CloseElement();
		}

		if (dsz.StartElement("CameraComponent"))
		{
			CameraComponent comp;
			DeserializeComponent<CameraComponent>(ent, comp);
			engine->world.AddComponent(ent, comp);
			engine->gCamera.ReInitCameraList(comp.camType, ent);
			dsz.CloseElement();
		}

		if (dsz.StartElement("PointLightComponent"))
		{
			PointLightComponent comp;
			DeserializeComponent<PointLightComponent>(ent, comp);
			engine->world.AddComponent(ent, comp);
			dsz.CloseElement();
		}
		
		if (dsz.StartElement("SpotLightComponent"))
		{
			SpotLightComponent comp;
			DeserializeComponent<SpotLightComponent>(ent, comp);
			engine->world.AddComponent(ent, comp);
			dsz.CloseElement();
		}
		
		if (dsz.StartElement("DirectionalLightComponent"))
		{
			DirectionalLightComponent comp;
			DeserializeComponent<DirectionalLightComponent>(ent, comp);
			engine->world.AddComponent(ent, comp);
			dsz.CloseElement();
		}

		if (dsz.StartElement("TextureComponent"))
		{
			TextureComponent comp;
			DeserializeComponent<TextureComponent>(ent, comp);
			engine->world.AddComponent(ent, comp);
			dsz.CloseElement();
		}
	}

	void SerializationManager::SerializeAllEntity(const char* fullpath)
	{
		const std::vector<Entity>& entities = engine->editorManager->GetEntityListByConstRef();
		std::filesystem::path p(fullpath);

		sz.StartElement("Entities");
		sz.AddAttributeToElement("Size", entities.size());
		size_t counter = 0;
		for (auto const& ent : entities)
		{
			SerializeEntity(ent, counter++);
		}
		sz.CloseElement();
	}

	void SerializationManager::DeserializeAllEntity(const char* fullpath)
	{
		std::filesystem::path p(fullpath);

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

	void SerializationManager::SaveSceneFile(const char* fullpath)
	{
		SerializeAllEntity(fullpath);
		SaveFile(fullpath);
	}

	void SerializationManager::LoadSceneFile(const char* fullpath)
	{
		if (LoadFile(fullpath))
		{
			DeserializeAllEntity(fullpath);
		}
	}

}