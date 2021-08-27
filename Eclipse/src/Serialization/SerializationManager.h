#pragma once
#include "Global.h"
#include "Serializer.h"
#include "Deserializer.h"
#include "../ECS/ComponentManager/Components/EntityComponent.h"
#include "../ECS/ComponentManager/Components/AabbComponent.h"
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
		inline void SerializeData(const char* name, T element, Args... elements)
		{
			sz.StartElement(name);
			sz.AddAttributeToElement(name, element);
			sz.CloseElement();
			SerializeData(elements...);
		}

		template <typename CompType>
		inline void SerializeComponent(const CompType& data)
		{
			sz.StartElement(typeid(data).name());
			sz.AddAttributeToElement("Unavailable", true);
			sz.CloseElement();
		}

		template <>
		inline void SerializeComponent<EntityComponent>(const EntityComponent& data)
		{
			sz.StartElement("EntityComponent");
			SerializeData(
				"Name", data.Name,
				"Tag", data.Tag,
				"IsActive", data.IsActive
			);
			sz.CloseElement();
		}

		template <>
		inline void SerializeComponent<TransformComponent>(const TransformComponent& data)
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
		inline void SerializeComponent<RenderComponent>(const RenderComponent& data)
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
		inline void SerializeComponent<RigidBodyComponent>(const RigidBodyComponent& data)
		{
			sz.StartElement("RigidBodyComponent");
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
		inline void SerializeComponent<MaterialComponent>(const MaterialComponent& data)
		{
			sz.StartElement("MaterialComponent");
			SerializeData(
				"ID", data.ID,
				"Ambient", data.ambient,
				"Diffuse", data.diffuse,
				"Specular", data.specular,
				"Shininess", data.shininess,
				"MaximumShininess", data.MaximumShininess,
				"RegisterForHighlight", data.RegisterForHighlight,
				"Highlight", data.Highlight,
				"Thickness", data.Thickness,
				"HighlightColor", data.HighlightColour,
				"ScaleUp", data.ScaleUp
			);
			sz.CloseElement();
		}

		template <>
		inline void SerializeComponent<AabbComponent>(const AabbComponent& data)
		{
			sz.StartElement("AabbComponent");
			SerializeData(
				"Center", data.center,
				"Min", data.min,
				"Max", data.max
			);
			sz.CloseElement();
		}

		template <>
		inline void SerializeComponent<CameraComponent>(const CameraComponent& data)
		{
			sz.StartElement("CameraComponent");
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
				"CameraType", lexical_cast<std::string>(data.camType),
				"ProjectionType", lexical_cast<std::string>(data.projType)
			);
			sz.CloseElement();
		}

		template <>
		inline void SerializeComponent<SpotLightComponent>(const SpotLightComponent& data)
		{
			sz.StartElement("SpotLightComponent");
			SerializeData(
				"ID", data.ID,
				"Counter", data.Counter,
				"LightColor", data.lightColor,
				"Direction", data.direction,
				"Ambient", data.ambient,
				"Diffuse", data.diffuse,
				"Specular", data.specular,
				"Color", data.Color,
				"IntensityStrength", data.IntensityStrength,
				"Radius", data.radius,
				"CutOff", data.cutOff,
				"OuterCutOff", data.outerCutOff,
				"Constant", data.constant,
				"Linear", data.linear,
				"Quadratic", data.quadratic,
				"AttenuationLevel", data.AttenuationLevel,
				"SurroundingAttenuationLevel", data.SurroundingAttenuationLevel,
				"EnableBlinnPhong", data.EnableBlinnPhong,
				"HasTexture", data.hasTexture,
				"Visible", data.visible
			);

			sz.CloseElement();
		}
		
		template <>
		inline void SerializeComponent<PointLightComponent>(const PointLightComponent& data)
		{
			sz.StartElement("PointLightComponent");
			SerializeData(
				"ID", data.ID,
				"Counter", data.Counter,
				"Ambient", data.ambient,
				"Diffuse", data.diffuse,
				"Specular", data.specular,
				"LightColor", data.lightColor,
				"Color", data.Color,
				"IntensityStrength", data.IntensityStrength,
				"Constant", data.constant,
				"Linear", data.linear,
				"Quadratic", data.quadratic,
				"Radius", data.radius,
				"AttenuationLevel", data.AttenuationLevel,
				"EnableBlinnPhong", data.EnableBlinnPhong,
				"HasTexture", data.hasTexture,
				"Visible", data.visible
			);

			sz.CloseElement();
		}

		template <>
		inline void SerializeComponent<DirectionalLightComponent>(const DirectionalLightComponent& data)
		{
			sz.StartElement("DirectionalLightComponent");
			SerializeData(
				"ID", data.ID,
				"Counter", data.Counter,
				"Visible", data.visible,
				"AffectsWorld", data.AffectsWorld,
				"EnableBlinnPhong", data.EnableBlinnPhong,
				"LightColor", data.lightColor,
				"Direction", data.Direction,
				"Ambient", data.ambient,
				"Diffuse", data.diffuse,
				"Specular", data.specular,
				"Color", data.Color,
				"ModelNDC_xform", data.modelNDC_xform
			);

			sz.CloseElement();
		}

		template <typename T>
		inline void DeserializeComponent(const Entity& ent)
		{
			std::string msg = typeid(T).name();
			msg += " is invalid as a component.";
			ENGINE_CORE_ERROR(false, msg.c_str());
		}

		template<>
		inline void DeserializeComponent<EntityComponent>(const Entity& ent)
		{
			EntityComponent test;

			if (dsz.StartElement("Name"))
			{
				dsz.ReadAttributeFromElement("Name", test.Name);
				dsz.CloseElement();
			}

			if (dsz.StartElement("Tag"))
			{
				dsz.ReadAttributeFromElement("Tag", test.Tag);
				dsz.CloseElement();
			}

			if (dsz.StartElement("IsActive"))
			{
				dsz.ReadAttributeFromElement("IsActive", test.IsActive);
				dsz.CloseElement();
			}
		}

		template<>
		inline void DeserializeComponent<TransformComponent>(const Entity& ent)
		{
			TransformComponent test;

			if (dsz.StartElement("Position"))
			{
				dsz.ReadAttributeFromElement(test.position);
				dsz.CloseElement();
			}

			if (dsz.StartElement("Rotation"))
			{
				dsz.ReadAttributeFromElement(test.rotation);
				dsz.CloseElement();
			}

			if (dsz.StartElement("Scale"))
			{
				dsz.ReadAttributeFromElement(test.scale);
				dsz.CloseElement();
			}
			std::cout << test.position << std::endl;
			std::cout << test.rotation << std::endl;
			std::cout << test.scale << std::endl;
		}

		void SerializeEntity(const Entity& ent, const size_t& counter);

		void DeserializeEntity(const size_t& counter);

		void SerializeAllComponents(const Entity& ent);

		void DeserializeAllComponents(const Entity& ent);

	public:
		SerializationManager();

		void SerializeAllEntity(const char* fullpath = "Data/Temp/Temp.xml");

		void DeserializeAllEntity(const char* fullpath = "Data/Temp/Temp.xml");

		void SaveFile(const char* fullpath = "Data/Temp/Temp.xml");

		bool LoadFile(const char* fullpath = "Data/Temp/Temp.xml");

		~SerializationManager();
	};
}