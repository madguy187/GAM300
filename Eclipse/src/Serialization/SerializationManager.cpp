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
			if (DeserializeAllComponents(ent))
			{
				engine->editorManager->RegisterExistingEntity(ent);
			}
			dsz.CloseElement();
		}
	}

	void SerializationManager::SerializeAllComponents(const Entity& ent)
	{
		World& w = engine->world;

		SerializeComponent<EntityComponent>(w, ent);

		SerializeComponent<CameraComponent>(w, ent);
		
		SerializeComponent<DirectionalLightComponent>(w, ent);
		
		SerializeComponent<MaterialComponent>(w, ent);
		
		SerializeComponent<PointLightComponent>(w, ent);
		
		SerializeComponent<MeshComponent>(w, ent);

		SerializeComponent<RigidBodyComponent>(w, ent);

		SerializeComponent<SpotLightComponent>(w, ent);

		SerializeComponent<TransformComponent>(w, ent);

		SerializeComponent<AABBComponent>(w, ent);

		SerializeComponent<TextureComponent>(w, ent);

		SerializeComponent<LightComponent>(w, ent);
	}

	bool SerializationManager::DeserializeAllComponents(const Entity& ent)
	{
		bool isSuccess = false;
		World& w = engine->world;

		if (DeserializeComponent<EntityComponent>(w, ent) &&
			DeserializeComponent<TransformComponent>(w, ent) &&
			DeserializeComponent<MeshComponent>(w, ent) &&
			DeserializeComponent<MaterialComponent>(w, ent) &&
			DeserializeComponent<AABBComponent>(w, ent) &&
			DeserializeComponent<RigidBodyComponent>(w, ent) &&
			DeserializeComponent<CameraComponent>(w, ent) &&
			DeserializeComponent<PointLightComponent>(w, ent) &&
			DeserializeComponent<SpotLightComponent>(w, ent) &&
			DeserializeComponent<DirectionalLightComponent>(w, ent) &&
			DeserializeComponent<TextureComponent>(w, ent) &&
			DeserializeComponent<LightComponent>(w, ent))
		{
			if (w.CheckComponent<CameraComponent>(ent))
			{
				auto& camera = w.GetComponent<CameraComponent>(ent);
				engine->gCamera.ReInitCameraList(camera.camType, ent);
			}
			isSuccess = true;
		}
		else
		{
			if (w.CheckComponent<EntityComponent>(ent))
			{
				auto& entity = w.GetComponent<EntityComponent>(ent);
				std::string msg = "Entity \"";
				msg += entity.Name + "\" cannot be deserialized due to corrupted component.";
				EDITOR_LOG_WARN(msg.c_str())
			}
			else
			{
				std::string msg = "Unknown entity cannot be deserialized due to corrupted component.";
				EDITOR_LOG_WARN(msg.c_str())
			}
			w.DestroyEntity(ent);
		}
		return isSuccess;
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