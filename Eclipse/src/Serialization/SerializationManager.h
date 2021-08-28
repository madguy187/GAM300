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
			sz.AddAttributeToElement("value", element);
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
				"HighlightColor", data.HighlightColour,
				"Shininess", data.shininess,
				"MaximumShininess", data.MaximumShininess,
				"RegisterForHighlight", data.RegisterForHighlight,
				"Highlight", data.Highlight,
				"Modeltype", data.Modeltype,
				"Thickness", data.Thickness,
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
			EntityComponent comp;

			if (dsz.StartElement("Name"))
			{
				dsz.ReadAttributeFromElement("value", comp.Name);
				dsz.CloseElement();
			}

			if (dsz.StartElement("Tag"))
			{
				dsz.ReadAttributeFromElement("value", comp.Tag);
				dsz.CloseElement();
			}

			if (dsz.StartElement("IsActive"))
			{
				dsz.ReadAttributeFromElement("value", comp.IsActive);
				dsz.CloseElement();
			}

			engine->world.AddComponent<EntityComponent>(ent, comp);
		}

		template<>
		inline void DeserializeComponent<TransformComponent>(const Entity& ent)
		{
			TransformComponent comp;

			if (dsz.StartElement("Position"))
			{
				dsz.ReadAttributeFromElement(comp.position);
				dsz.CloseElement();
			}

			if (dsz.StartElement("Rotation"))
			{
				dsz.ReadAttributeFromElement(comp.rotation);
				dsz.CloseElement();
			}

			if (dsz.StartElement("Scale"))
			{
				dsz.ReadAttributeFromElement(comp.scale);
				dsz.CloseElement();
			}

			engine->world.AddComponent<TransformComponent>(ent, comp);
		}

		template<>
		inline void DeserializeComponent<RenderComponent>(const Entity& ent)
		{
			RenderComponent comp;

			if (dsz.StartElement("Color"))
			{
				dsz.ReadAttributeFromElement(comp.color);
				dsz.CloseElement();
			}

			if (dsz.StartElement("TextureIdx"))
			{
				dsz.ReadAttributeFromElement(comp.textureIdx);
				dsz.CloseElement();
			}

			if (dsz.StartElement("HasTexture"))
			{
				dsz.ReadAttributeFromElement("value", comp.hasTexture);
				dsz.CloseElement();
			}

			if (dsz.StartElement("IsQuad"))
			{
				dsz.ReadAttributeFromElement("value", comp.isQuad);
				dsz.CloseElement();
			}

			if (dsz.StartElement("ModelNDC_XForm"))
			{
				dsz.ReadAttributeFromElement(comp.modelNDC_xform);
				dsz.CloseElement();
			}

			if (dsz.StartElement("ModelRef"))
			{
				dsz.ReadAttributeFromElement("value", comp.modelRef);
				dsz.CloseElement();
			}

			if (dsz.StartElement("TextureRef") && comp.hasTexture)
			{
				dsz.ReadAttributeFromElement("value", comp.textureRef);
				dsz.CloseElement();
			}

			if (dsz.StartElement("ShaderRef"))
			{
				dsz.ReadAttributeFromElement("value", comp.shaderRef);
				dsz.CloseElement();
			}

			comp.ID = ent;

			if (dsz.StartElement("Name"))
			{
				dsz.ReadAttributeFromElement("value", comp.name);
				dsz.CloseElement();
			}
			
			if (dsz.StartElement("NewLayer"))
			{
				dsz.ReadAttributeFromElement("value", comp.newLayer);
				dsz.CloseElement();
			}
			
			if (dsz.StartElement("LayerNum"))
			{
				dsz.ReadAttributeFromElement("value", comp.layerNum);
				dsz.CloseElement();
			}
			
			if (dsz.StartElement("Transparency"))
			{
				dsz.ReadAttributeFromElement("value", comp.transparency);
				dsz.CloseElement();
			}
			
			if (dsz.StartElement("LightColor"))
			{
				dsz.ReadAttributeFromElement(comp.lightColor);
				dsz.CloseElement();
			}

			engine->world.AddComponent<RenderComponent>(ent, comp);
		}
		
		template<>
		inline void DeserializeComponent<MaterialComponent>(const Entity& ent)
		{
			MaterialComponent comp;

			comp.ID = ent;

			if (dsz.StartElement("Ambient"))
			{
				dsz.ReadAttributeFromElement(comp.ambient);
				dsz.CloseElement();
			}

			if (dsz.StartElement("Diffuse"))
			{
				dsz.ReadAttributeFromElement(comp.diffuse);
				dsz.CloseElement();
			}

			if (dsz.StartElement("Specular"))
			{
				dsz.ReadAttributeFromElement(comp.specular);
				dsz.CloseElement();
			}

			if (dsz.StartElement("HighlightColor"))
			{
				dsz.ReadAttributeFromElement(comp.HighlightColour);
				dsz.CloseElement();
			}

			if (dsz.StartElement("Shininess"))
			{
				dsz.ReadAttributeFromElement("value", comp.shininess);
				dsz.CloseElement();
			}

			if (dsz.StartElement("MaximumShininess"))
			{
				dsz.ReadAttributeFromElement("value", comp.MaximumShininess);
				dsz.CloseElement();
			}

			if (dsz.StartElement("RegisterForHighlight"))
			{
				dsz.ReadAttributeFromElement("value", comp.RegisterForHighlight);
				dsz.CloseElement();
			}

			if (dsz.StartElement("Highlight"))
			{
				dsz.ReadAttributeFromElement("value", comp.Highlight);
				dsz.CloseElement();
			}
			
			if (dsz.StartElement("Modeltype"))
			{
				dsz.ReadAttributeFromElement("value", comp.Modeltype);
				dsz.CloseElement();
			}

			if (dsz.StartElement("Thickness"))
			{
				dsz.ReadAttributeFromElement("value", comp.Thickness);
				dsz.CloseElement();
			}
			
			if (dsz.StartElement("ScaleUp"))
			{
				dsz.ReadAttributeFromElement("value", comp.ScaleUp);
				dsz.CloseElement();
			}

			engine->world.AddComponent<MaterialComponent>(ent, comp);
		}
		
		template<>
		inline void DeserializeComponent<AabbComponent>(const Entity& ent)
		{
			AabbComponent comp;

			if (dsz.StartElement("Center"))
			{
				dsz.ReadAttributeFromElement(comp.center);
				dsz.CloseElement();
			}

			if (dsz.StartElement("Min"))
			{
				dsz.ReadAttributeFromElement(comp.min);
				dsz.CloseElement();
			}

			if (dsz.StartElement("Max"))
			{
				dsz.ReadAttributeFromElement(comp.max);
				dsz.CloseElement();
			}

			engine->world.AddComponent<AabbComponent>(ent, comp);
		}
		
		template<>
		inline void DeserializeComponent<RigidBodyComponent>(const Entity& ent)
		{
			RigidBodyComponent comp;
			
			if (dsz.StartElement("IsStatic"))
			{
				dsz.ReadAttributeFromElement("value", comp._Static);
				dsz.CloseElement();
			}
			
			if (dsz.StartElement("IsKinematic"))
			{
				dsz.ReadAttributeFromElement("value", comp._Kinematic);
				dsz.CloseElement();
			}
			
			if (dsz.StartElement("IsEnableGravity"))
			{
				dsz.ReadAttributeFromElement("value", comp.enableGravity);
				dsz.CloseElement();
			}
			
			if (dsz.StartElement("IsEnableGravity"))
			{
				dsz.ReadAttributeFromElement("value", comp.enableGravity);
				dsz.CloseElement();
			}
			
			if (dsz.StartElement("IsEnableRotation"))
			{
				dsz.ReadAttributeFromElement("value", comp.enableRotation);
				dsz.CloseElement();
			}
			
			if (dsz.StartElement("Mass"))
			{
				dsz.ReadAttributeFromElement("value", comp.mass);
				dsz.CloseElement();
			}
			
			if (dsz.StartElement("Drag"))
			{
				dsz.ReadAttributeFromElement("value", comp.drag);
				dsz.CloseElement();
			}
			
			if (dsz.StartElement("AngDrag"))
			{
				dsz.ReadAttributeFromElement("value", comp.angdrag);
				dsz.CloseElement();
			}

			if (dsz.StartElement("Velocity"))
			{
				dsz.ReadAttributeFromElement(comp.velocity);
				dsz.CloseElement();
			}

			if (dsz.StartElement("Forces"))
			{
				dsz.ReadAttributeFromElement(comp.forces);
				dsz.CloseElement();
			}

			engine->world.AddComponent<RigidBodyComponent>(ent, comp);
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