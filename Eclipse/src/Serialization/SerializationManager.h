#pragma once
#include "Serializer.h"
#include "../ECS/ComponentManager/Components/EntityComponent.h"
#include "../ECS/ComponentManager/Components/CameraComponent.h"
#include "../ECS/ComponentManager/Components/DirectionalLightComponent.h"
#include "../ECS/ComponentManager/Components/MaterialComponent.h"
#include "../ECS/ComponentManager/Components/PointLightComponent.h"
#include "../ECS/ComponentManager/Components/RenderComponent.h"
#include "../ECS/ComponentManager/Components/RigidBodyComponent.h"
#include "../ECS/ComponentManager/Components/SpotLightComponent.h"
#include "../ECS/ComponentManager/Components/TransformComponent.h"

namespace Eclipse
{

	class SerializationManager
	{
		Serializer sz;


		void SerializeData() {}

		template <typename T, typename... Args>
		void SerializeData(const char* name, T element, Args... elements)
		{
			sz.StartElement(name);
			sz.AddAttributeToElement(name, true);
			sz.CloseElement();
			SerializeData(elements...);
		}

		template <typename CompType>
		void SerializeComponent(const CompType& data)
		{
			sz.StartElement(typeid(data).name());
			sz.AddAttributeToElement("Unavailable", true);
			sz.CloseElement();
		}

		template <>
		void SerializeComponent<EntityComponent>(const EntityComponent& data)
		{
			sz.StartElement("EntityComponent");
			SerializeData(
				"Name", data.Name,
				"Tag", static_cast<int>(data.Tag),
				"IsActive", data.IsActive
			);
			sz.CloseElement();
		}

		template <>
		void SerializeComponent<TransformComponent>(const TransformComponent& data)
		{
			sz.StartElement("TransformComponent");
			SerializeData(
				"Position", data.position,
				"Rotation", data.rotation,
				"Scale", data.scale
			);
			sz.CloseElement();
		}


		void SerializeEntity(const Entity& ent, const size_t& counter);

		void SerializeAllComponents(const Entity& ent);

	public:
		SerializationManager();

		void SerializeAllEntity(const char* fullpath = "Data/Temp/temp.xml");

		~SerializationManager();
	};
}