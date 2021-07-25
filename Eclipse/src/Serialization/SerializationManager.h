#pragma once
#include "Serializer.h"
#include "Deserializer.h"
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
		Deserializer dsz;

		void SerializeData() {}

		template <typename T, typename... Args>
		void SerializeData(const char* name, T element, Args... elements)
		{
			sz.StartElement(name);
			sz.AddAttributeToElement(name, element);
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

		template <>
		void SerializeComponent<RenderComponent>(const RenderComponent& data)
		{
			sz.StartElement("RenderComponent");
			SerializeData(
				"Color", data.color,
				"TextureIdx", data.textureIdx,
				"HasTexture", data.hasTexture,
				"IsQuad", data.isQuad,
				"ModelNDC_XForm", data.modelNDC_xform,
				"ModelRef", data.modelRef->first,
				"TextureRef", data.hasTexture ? data.textureRef->first : "",
				"ShaderRef", data.shaderRef->first,
				"ID", data.ID,
				"Name", data.name,
				"NewLayer", data.newLayer,
				"LayerNum", data.layerNum,
				"Transparency", data.transparency,
				"LightColor", data.lightColor
			);
			sz.CloseElement();
		}

		template <>
		void SerializeComponent<RigidBodyComponent>(const RigidBodyComponent& data)
		{
			sz.StartElement("RenderComponent");
			SerializeData(
				"IsStatic", data._Static,
				"IsKinematic", data._Kinematic,
				"IsEnableGravity", data.enableGravity,
				"IsEnableRotation", data.enableRotation,
				"Mass", data.mass,
				"Drag", data.drag,
				"AngDrag", data.angdrag,
				"Velocity", data.velocity,
				"Forces", data.forces
			);
			sz.CloseElement();
		}

		template <>
		void SerializeComponent<MaterialComponent>(const MaterialComponent& data)
		{
			sz.StartElement("RenderComponent");
			SerializeData(
				"ID", data.ID,
				"Ambient", data.ambient,
				"Diffuse", data.diffuse,
				"Specular", data.specular,
				"Shininess", data.shininess,
				"Color", data.Color,
				"HasTexture", data.hasTexture,
				"IsVisible", data.visible
			);
			sz.CloseElement();
		}

		template <>
		void SerializeComponent<CameraComponent>(const CameraComponent& data)
		{
			sz.StartElement("RenderComponent");
			SerializeData(
				"EyeAlpha", data.eyeAlpha,
				"EyeBeta", data.eyeBeta,
				"FOV", data.fov,
				"NearPlane", data.nearPlane,
				"FarPlane", data.farPlane,
				"Aspect", data.aspect,
				"EyePos", data.eyePos,
				"EyeFront", data.eyeFront,
				"UpVec", data.upVec,
				"RightVec", data.rightVec,
				"WorldUp", data.worldUp,
				"ViewMtx", data.viewMtx,
				"ProjMtx", data.projMtx,
				"CameraSpeed", data.cameraSpeed,
				"CamType", static_cast<int>(data.camType)
			);
			sz.CloseElement();
		}


		void SerializeEntity(const Entity& ent, const size_t& counter);

		void DeserializeEntity(const size_t& counter);

		void SerializeAllComponents(const Entity& ent);

	public:
		SerializationManager();

		void SerializeAllEntity(const char* fullpath = "Data/Temp/Temp.xml");

		void DeserializeAllEntity(const char* fullpath = "Data/Temp/Temp.xml");

		void SaveFile(const char* fullpath = "Data/Temp/Temp.xml");

		bool LoadFile(const char* fullpath = "Data/Temp/Temp.xml");

		~SerializationManager();
	};
}