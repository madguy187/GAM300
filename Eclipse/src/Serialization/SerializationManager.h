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
#include "../ECS/ComponentManager/Components/TextureComponent.h"
#include "../ECS/ComponentManager/Components/MeshComponent3D.h"

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
				"IsQuad", data.isQuad,
				"ModelNDC_XForm", data.modelNDC_xform,
				"ModelRef", data.modelRef,
				"ShaderRef", data.shaderRef,
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
				"Modeltype", data.Modeltype,
				"RegisterForHighlight", data.RegisterForHighlight,
				"Highlight", data.Highlight,
				"NoTextures", data.NoTextures,
				"Thickness", data.Thickness,
				"ScaleUp", data.ScaleUp
			);
			sz.CloseElement();
		}

		template <>
		inline void SerializeComponent<AABBComponent>(const AABBComponent& data)
		{
			sz.StartElement("AABBComponent");
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
				"CameraType", data.camType,
				"ProjectionType", data.projType
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
				"Visible", data.visible,
				"AffectsWorld", data.AffectsWorld
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
				"Visible", data.visible,
				"AffectsWorld", data.AffectsWorld
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

		template <>
		inline void SerializeComponent<TextureComponent>(const TextureComponent& data)
		{
			sz.StartElement("TextureComponent");
			SerializeData(
				"ID", data.ID,
				"TextureType", data.Type,
				"TextureKey", data.TextureKey,
				"HasTexture", data.hasTexture,
				"TextureRef", data.textureRef
			);

			sz.CloseElement();
		}
		
		template <>
		inline void SerializeComponent<MeshComponent3D>(const MeshComponent3D& data)
		{
			sz.StartElement("MeshComponent3D");
			SerializeData(
				"ID", data.ID,
				"Key", data.Key
			);

			sz.CloseElement();
		}

		template <typename T>
		inline void DeserializeComponent(const Entity& ent, T& comp)
		{
			std::string msg = typeid(T).name();
			msg += " is invalid as a component.";
			ENGINE_CORE_ERROR(false, msg.c_str());
		}

		template<>
		inline void DeserializeComponent<EntityComponent>(const Entity& ent, EntityComponent& comp)
		{
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
		}

		template<>
		inline void DeserializeComponent<TransformComponent>(const Entity& ent, TransformComponent& comp)
		{
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
		}

		template<>
		inline void DeserializeComponent<RenderComponent>(const Entity& ent, RenderComponent& comp)
		{
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
		}
		
		template<>
		inline void DeserializeComponent<MaterialComponent>(const Entity& ent, MaterialComponent& comp)
		{
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
			
			if (dsz.StartElement("NoTextures"))
			{
				dsz.ReadAttributeFromElement("value", comp.NoTextures);
				dsz.CloseElement();
			}
			
			if (dsz.StartElement("ScaleUp"))
			{
				dsz.ReadAttributeFromElement("value", comp.ScaleUp);
				dsz.CloseElement();
			}
		}
		
		template<>
		inline void DeserializeComponent<AABBComponent>(const Entity& ent, AABBComponent& comp)
		{
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
		}
		
		template<>
		inline void DeserializeComponent<RigidBodyComponent>(const Entity& ent, RigidBodyComponent& comp)
		{			
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
		}
		
		template<>
		inline void DeserializeComponent<CameraComponent>(const Entity& ent, CameraComponent& comp)
		{
			if (dsz.StartElement("EyeAlpha"))
			{
				dsz.ReadAttributeFromElement("value", comp.eyeAlpha);
				dsz.CloseElement();
			}
			
			if (dsz.StartElement("EyeBeta"))
			{
				dsz.ReadAttributeFromElement("value", comp.eyeBeta);
				dsz.CloseElement();
			}

			if (dsz.StartElement("FOV"))
			{
				dsz.ReadAttributeFromElement("value", comp.fov);
				dsz.CloseElement();
			}
			
			if (dsz.StartElement("NearPlane"))
			{
				dsz.ReadAttributeFromElement("value", comp.nearPlane);
				dsz.CloseElement();
			}
			
			if (dsz.StartElement("FarPlane"))
			{
				dsz.ReadAttributeFromElement("value", comp.farPlane);
				dsz.CloseElement();
			}
			
			if (dsz.StartElement("Aspect"))
			{
				dsz.ReadAttributeFromElement("value", comp.aspect);
				dsz.CloseElement();
			}

			if (dsz.StartElement("EyePos"))
			{
				dsz.ReadAttributeFromElement(comp.eyePos);
				dsz.CloseElement();
			}
			
			if (dsz.StartElement("EyeFront"))
			{
				dsz.ReadAttributeFromElement(comp.eyeFront);
				dsz.CloseElement();
			}

			if (dsz.StartElement("UpVec"))
			{
				dsz.ReadAttributeFromElement(comp.upVec);
				dsz.CloseElement();
			}

			if (dsz.StartElement("RightVec"))
			{
				dsz.ReadAttributeFromElement(comp.rightVec);
				dsz.CloseElement();
			}

			if (dsz.StartElement("WorldUp"))
			{
				dsz.ReadAttributeFromElement(comp.worldUp);
				dsz.CloseElement();
			}
			
			if (dsz.StartElement("ViewMtx"))
			{
				dsz.ReadAttributeFromElement(comp.viewMtx);
				dsz.CloseElement();
			}
			
			if (dsz.StartElement("ProjMtx"))
			{
				dsz.ReadAttributeFromElement(comp.projMtx);
				dsz.CloseElement();
			}
			
			if (dsz.StartElement("CameraSpeed"))
			{
				dsz.ReadAttributeFromElement("value", comp.cameraSpeed);
				dsz.CloseElement();
			}
			
			if (dsz.StartElement("CameraType"))
			{
				dsz.ReadAttributeFromElement("value", comp.camType);
				dsz.CloseElement();
			}
			
			if (dsz.StartElement("ProjectionType"))
			{
				dsz.ReadAttributeFromElement("value", comp.projType);
				dsz.CloseElement();
			}
		}

		template<>
		inline void DeserializeComponent<SpotLightComponent>(const Entity& ent, SpotLightComponent& comp)
		{
			comp.ID = ent;

			if (dsz.StartElement("Counter"))
			{
				dsz.ReadAttributeFromElement("value", comp.Counter);
				dsz.CloseElement();
			}
			
			if (dsz.StartElement("LightColor"))
			{
				dsz.ReadAttributeFromElement(comp.lightColor);
				dsz.CloseElement();
			}

			if (dsz.StartElement("Direction"))
			{
				dsz.ReadAttributeFromElement(comp.direction);
				dsz.CloseElement();
			}
			
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

			if (dsz.StartElement("Color"))
			{
				dsz.ReadAttributeFromElement(comp.Color);
				dsz.CloseElement();
			}
			
			if (dsz.StartElement("IntensityStrength"))
			{
				dsz.ReadAttributeFromElement("value", comp.IntensityStrength);
				dsz.CloseElement();
			}

			if (dsz.StartElement("Radius"))
			{
				dsz.ReadAttributeFromElement("value", comp.radius);
				dsz.CloseElement();
			}

			if (dsz.StartElement("CutOff"))
			{
				dsz.ReadAttributeFromElement("value", comp.cutOff);
				dsz.CloseElement();
			}

			if (dsz.StartElement("OuterCutOff"))
			{
				dsz.ReadAttributeFromElement("value", comp.outerCutOff);
				dsz.CloseElement();
			}
			
			if (dsz.StartElement("Constant"))
			{
				dsz.ReadAttributeFromElement("value", comp.constant);
				dsz.CloseElement();
			}
			
			if (dsz.StartElement("Linear"))
			{
				dsz.ReadAttributeFromElement("value", comp.linear);
				dsz.CloseElement();
			}
			
			if (dsz.StartElement("Quadratic"))
			{
				dsz.ReadAttributeFromElement("value", comp.quadratic);
				dsz.CloseElement();
			}

			if (dsz.StartElement("AttenuationLevel"))
			{
				dsz.ReadAttributeFromElement("value", comp.AttenuationLevel);
				dsz.CloseElement();
			}
			
			if (dsz.StartElement("SurroundingAttenuationLevel"))
			{
				dsz.ReadAttributeFromElement("value", comp.SurroundingAttenuationLevel);
				dsz.CloseElement();
			}

			if (dsz.StartElement("EnableBlinnPhong"))
			{
				dsz.ReadAttributeFromElement("value", comp.EnableBlinnPhong);
				dsz.CloseElement();
			}
			
			if (dsz.StartElement("HasTexture"))
			{
				dsz.ReadAttributeFromElement("value", comp.hasTexture);
				dsz.CloseElement();
			}
			
			if (dsz.StartElement("Visible"))
			{
				dsz.ReadAttributeFromElement("value", comp.visible);
				dsz.CloseElement();
			}

			if (dsz.StartElement("AffectsWorld"))
			{
				dsz.ReadAttributeFromElement("value", comp.AffectsWorld);
				dsz.CloseElement();
			}
		}

		template<>
		inline void DeserializeComponent<PointLightComponent>(const Entity& ent, PointLightComponent& comp)
		{
			comp.ID = ent;

			if (dsz.StartElement("Counter"))
			{
				dsz.ReadAttributeFromElement("value", comp.Counter);
				dsz.CloseElement();
			}

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

			if (dsz.StartElement("LightColor"))
			{
				dsz.ReadAttributeFromElement(comp.lightColor);
				dsz.CloseElement();
			}

			if (dsz.StartElement("Color"))
			{
				dsz.ReadAttributeFromElement(comp.Color);
				dsz.CloseElement();
			}

			if (dsz.StartElement("IntensityStrength"))
			{
				dsz.ReadAttributeFromElement("value", comp.IntensityStrength);
				dsz.CloseElement();
			}

			if (dsz.StartElement("Constant"))
			{
				dsz.ReadAttributeFromElement("value", comp.constant);
				dsz.CloseElement();
			}

			if (dsz.StartElement("Linear"))
			{
				dsz.ReadAttributeFromElement("value", comp.linear);
				dsz.CloseElement();
			}

			if (dsz.StartElement("Quadratic"))
			{
				dsz.ReadAttributeFromElement("value", comp.quadratic);
				dsz.CloseElement();
			}

			if (dsz.StartElement("Radius"))
			{
				dsz.ReadAttributeFromElement("value", comp.radius);
				dsz.CloseElement();
			}

			if (dsz.StartElement("AttenuationLevel"))
			{
				dsz.ReadAttributeFromElement("value", comp.AttenuationLevel);
				dsz.CloseElement();
			}

			if (dsz.StartElement("EnableBlinnPhong"))
			{
				dsz.ReadAttributeFromElement("value", comp.EnableBlinnPhong);
				dsz.CloseElement();
			}

			if (dsz.StartElement("HasTexture"))
			{
				dsz.ReadAttributeFromElement("value", comp.hasTexture);
				dsz.CloseElement();
			}

			if (dsz.StartElement("Visible"))
			{
				dsz.ReadAttributeFromElement("value", comp.visible);
				dsz.CloseElement();
			}

			if (dsz.StartElement("AffectsWorld"))
			{
				dsz.ReadAttributeFromElement("value", comp.AffectsWorld);
				dsz.CloseElement();
			}
		}

		template<>
		inline void DeserializeComponent<DirectionalLightComponent>(const Entity& ent, DirectionalLightComponent& comp)
		{
			comp.ID = ent;

			if (dsz.StartElement("Counter"))
			{
				dsz.ReadAttributeFromElement("value", comp.Counter);
				dsz.CloseElement();
			}

			if (dsz.StartElement("Visible"))
			{
				dsz.ReadAttributeFromElement("value", comp.visible);
				dsz.CloseElement();
			}
			
			if (dsz.StartElement("AffectsWorld"))
			{
				dsz.ReadAttributeFromElement("value", comp.AffectsWorld);
				dsz.CloseElement();
			}

			if (dsz.StartElement("EnableBlinnPhong"))
			{
				dsz.ReadAttributeFromElement("value", comp.EnableBlinnPhong);
				dsz.CloseElement();
			}

			if (dsz.StartElement("LightColor"))
			{
				dsz.ReadAttributeFromElement(comp.lightColor);
				dsz.CloseElement();
			}
			
			if (dsz.StartElement("Direction"))
			{
				dsz.ReadAttributeFromElement(comp.Direction);
				dsz.CloseElement();
			}

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

			if (dsz.StartElement("Color"))
			{
				dsz.ReadAttributeFromElement(comp.Color);
				dsz.CloseElement();
			}

			if (dsz.StartElement("ModelNDC_xform"))
			{
				dsz.ReadAttributeFromElement(comp.modelNDC_xform);
				dsz.CloseElement();
			}
		}
		
		template<>
		inline void DeserializeComponent<TextureComponent>(const Entity& ent, TextureComponent& comp)
		{
			comp.ID = ent;

			if (dsz.StartElement("TextureType"))
			{
				dsz.ReadAttributeFromElement("value", comp.Type);
				dsz.CloseElement();
			}

			if (dsz.StartElement("TextureKey"))
			{
				dsz.ReadAttributeFromElement("value", comp.TextureKey);
				dsz.CloseElement();
			}
			
			if (dsz.StartElement("HasTexture"))
			{
				dsz.ReadAttributeFromElement("value", comp.hasTexture);
				dsz.CloseElement();
			}

			if (dsz.StartElement("TextureRef"))
			{
				dsz.ReadAttributeFromElement("value", comp.textureRef);
				dsz.CloseElement();
			}
		}
		
		template<>
		inline void DeserializeComponent<MeshComponent3D>(const Entity& ent, MeshComponent3D& comp)
		{
			comp.ID = ent;

			if (dsz.StartElement("Key"))
			{
				dsz.ReadAttributeFromElement("value", comp.Key);
				dsz.CloseElement();
			}
		}

		void SerializeEntity(const Entity& ent, const size_t& counter);

		void DeserializeEntity(const size_t& counter);

		void SerializeAllComponents(const Entity& ent);

		void DeserializeAllComponents(const Entity& ent);

		void SerializeAllEntity(const char* fullpath);

		void DeserializeAllEntity(const char* fullpath);

		void SaveFile(const char* fullpath);

		bool LoadFile(const char* fullpath);

	public:
		SerializationManager();

		~SerializationManager();

		void SaveSceneFile(const char* fullpath = "Data/Temp/Temp.xml");

		void LoadSceneFile(const char* fullpath = "Data/Temp/Temp.xml");
	};
}