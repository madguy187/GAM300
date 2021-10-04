#pragma once
#include "Global.h"
#include "Serializer.h"
#include "Deserializer.h"
#include "Reflection/Variant/RefVariant.h"
#include "ECS//ComponentManager/Components/PrefabComponent.h"

namespace Eclipse
{
	class SerializationManager
	{
		class Backup
		{
			TiXmlDocument _backup;
			std::string backUpPath;
		public:
			void SaveBackup(Serializer&);
			void LoadBackup(Deserializer&);
		};
		Backup backup;

		/*
		inline void SerializeData() {}

		void LogError(const std::string& msg);

		template <typename T, typename... Args>
		inline void SerializeData(const char* name, T element, Args... elements)
		{
			sz.StartElement(name);
			sz.AddAttributeToElement("value", element);
			sz.CloseElement();
			SerializeData(elements...);
		}
		template <>
		inline void SerializeComponentData<EntityComponent>(const EntityComponent& data)
		{
			SerializeData(
				"Name", data.Name,
				"Tag", data.Tag,
				"IsActive", data.IsActive
			);
		}

		template <>
		inline void SerializeComponentData<TransformComponent>(const TransformComponent& data)
		{
			SerializeData(
				"Position", data.position,
				"Rotation", data.rotation,
				"Scale", data.scale
			);
		}

		template <>
		inline void SerializeComponentData<MeshComponent>(const MeshComponent& data)
		{
			SerializeData(
				//"Key", data.Key,
				"Color", data.color,
				"TextureIdx", data.textureIdx,
				"IsQuad", data.isQuad,
				"ModelNDC_XForm", data.modelNDC_xform,
				"ModelRef", data.modelRef,
				"ShaderRef", data.shaderRef,
				//"ID", data.ID,
				//"Name", data.name,
				//"NewLayer", data.newLayer,
				//"LayerNum", data.layerNum,
				"Transparency", data.transparency
				//"LightColor", data.lightColor
			);
		}

		template <>
		inline void SerializeComponentData<RigidBodyComponent>(const RigidBodyComponent& data)
		{
			SerializeData(
				"Velocity", data.velocity,
				"AngVelocity", data.Angvelocity,
				"Forces", data.forces,
				"Mass", data.mass,
				"Drag", data.drag,
				"AngDrag", data.angdrag,
				"IsKinematic", data._Kinematic,
				"IsEnableGravity", data.enableGravity,
				"IsEnableRotation", data.enableRotation,
				"InScene", data.inScene
			);
		}

		template <>
		inline void SerializeComponentData<MaterialComponent>(const MaterialComponent& data)
		{
			SerializeData(
				"Modeltype", data.Modeltype,
				"Ambient", data.ambient,
				"Diffuse", data.diffuse,
				"Specular", data.specular,
				//"HighlightColor", data.HighlightColour,
				"Shininess", data.shininess,
				"MaximumShininess", data.MaximumShininess,
				//"RegisterForHighlight", data.RegisterForHighlight,
				"Highlight", data.Highlight,
				"NoTextures", data.NoTextures
			);
		}

		template <>
		inline void SerializeComponentData<AABBComponent>(const AABBComponent& data)
		{
			SerializeData(
				"Center", data.center,
				"Min", data.Min,
				"Max", data.Max
			);
		}

		template <>
		inline void SerializeComponentData<CameraComponent>(const CameraComponent& data)
		{
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
		}

		template <>
		inline void SerializeComponentData<SpotLightComponent>(const SpotLightComponent& data)
		{
			SerializeData(
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
		}
		
		template <>
		inline void SerializeComponentData<PointLightComponent>(const PointLightComponent& data)
		{
			SerializeData(
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
		}

		template <>
		inline void SerializeComponentData<DirectionalLightComponent>(const DirectionalLightComponent& data)
		{
			SerializeData(
				"Counter", data.Counter,
				"Visible", data.visible,
				"AffectsWorld", data.AffectsWorld,
				"EnableBlinnPhong", data.EnableBlinnPhong,
				"LightColor", data.lightColor,
				"Direction", data.Direction,
				"Ambient", data.ambient,
				"Diffuse", data.diffuse,
				"Specular", data.specular,
				"Color", data.Color
			);
		}

		template <>
		inline void SerializeComponentData<TextureComponent>(const TextureComponent& data)
		{
			SerializeData(
				"TextureRef", data.textureRef
			);
		}
		
		template <>
		inline void SerializeComponentData<LightComponent>(const LightComponent& data)
		{
		}
		
		inline bool DeserializeData() { return true; }

		template <typename T, typename... Args>
		inline bool DeserializeData(const std::string& ele_name, T& data, Args&... elements)
		{
			bool isSuccess = false;
			if (dsz.StartElement(ele_name))
			{
				dsz.ReadAttributeFromElement("value", data);
				dsz.CloseElement();
				isSuccess = true;
			}

			if (isSuccess)
			{
				isSuccess = DeserializeData(elements...);
			}

			return isSuccess;
		}

		template<>
		inline bool DeserializeComponentData<EntityComponent>(const Entity& ent, EntityComponent& comp)
		{
			bool isSuccess = DeserializeData(
				"Name", comp.Name,
				"Tag", comp.Tag
			);

			return isSuccess;
		}

		template<>
		inline bool DeserializeComponentData<TransformComponent>(const Entity& ent, TransformComponent& comp)
		{
			bool isSuccess = DeserializeData(
				"Position", comp.position,
				"Rotation", comp.rotation,
				"Scale", comp.scale
			);

			return isSuccess;
		}

		template<>
		inline bool DeserializeComponentData<MeshComponent>(const Entity& ent, MeshComponent& comp)
		{
			bool isSuccess = DeserializeData(
				"Color", comp.color,
				"TextureIdx", comp.textureIdx,
				"IsQuad", comp.isQuad,
				"ModelNDC_XForm", comp.modelNDC_xform,
				"ModelRef", comp.modelRef,
				"ShaderRef", comp.shaderRef,
				//"Name", comp.name,
				//"NewLayer", comp.newLayer,
				//"LayerNum", comp.layerNum,
				"Transparency", comp.transparency
				//"LightColor", comp.lightColor
			);

			//comp.ID = ent;

			return isSuccess;
		}
		
		template<>
		inline bool DeserializeComponentData<MaterialComponent>(const Entity& ent, MaterialComponent& comp)
		{
			bool isSuccess = DeserializeData(
				"Modeltype", comp.Modeltype,
				"Ambient", comp.ambient,
				"Diffuse", comp.diffuse,
				"Specular", comp.specular,
				//"HighlightColor", comp.HighlightColour,
				"Shininess", comp.shininess,
				"MaximumShininess", comp.MaximumShininess,
				//"RegisterForHighlight", comp.RegisterForHighlight,
				"Highlight", comp.Highlight,
				"NoTextures", comp.NoTextures
			);

			return isSuccess;
			comp.ComboIndex = static_cast<size_t>(comp.Modeltype);
		}
		
		template<>
		inline bool DeserializeComponentData<AABBComponent>(const Entity& ent, AABBComponent& comp)
		{
			bool isSuccess = DeserializeData(
				"Center", comp.center,
				"Min", comp.Min,
				"Max", comp.Max
			);

			return isSuccess;
		}
		
		template<>
		inline bool DeserializeComponentData<RigidBodyComponent>(const Entity& ent, RigidBodyComponent& comp)
		{
			bool isSuccess = DeserializeData(
				"Velocity", comp.velocity,
				"AngVelocity", comp.Angvelocity,
				"Forces", comp.forces,
				"Mass", comp.mass,
				"Drag", comp.drag,
				"AngDrag", comp.angdrag,
				"IsKinematic", comp._Kinematic,
				"IsEnableGravity", comp.enableGravity,
				"IsEnableRotation", comp.enableRotation,
				"InScene", comp.inScene
			);

			return isSuccess;
		}
		
		template<>
		inline bool DeserializeComponentData<CameraComponent>(const Entity& ent, CameraComponent& comp)
		{
			bool isSuccess = DeserializeData(
				"EyeAlpha", comp.eyeAlpha,
				"EyeBeta", comp.eyeBeta,
				"FOV", comp.fov,
				"NearPlane", comp.nearPlane,
				"FarPlane", comp.farPlane,
				"Aspect", comp.aspect,
				"EyePos", comp.eyePos,
				"EyeFront", comp.eyeFront,
				"UpVec", comp.upVec,
				"RightVec", comp.rightVec,
				"WorldUp", comp.worldUp,
				"ViewMtx", comp.viewMtx,
				"ProjMtx", comp.projMtx,
				"CameraSpeed", comp.cameraSpeed,
				"CameraType", comp.camType,
				"ProjectionType", comp.projType
			);

			return isSuccess;
		}

		template<>
		inline bool DeserializeComponentData<SpotLightComponent>(const Entity& ent, SpotLightComponent& comp)
		{
			bool isSuccess = DeserializeData(
				"Counter", comp.Counter,
				"LightColor", comp.lightColor,
				"Direction", comp.direction,
				"Ambient", comp.ambient,
				"Diffuse", comp.diffuse,
				"Specular", comp.specular,
				"Color", comp.Color,
				"IntensityStrength", comp.IntensityStrength,
				"Radius", comp.radius,
				"CutOff", comp.cutOff,
				"OuterCutOff", comp.outerCutOff,
				"Constant", comp.constant,
				"Linear", comp.linear,
				"Quadratic", comp.quadratic,
				"AttenuationLevel", comp.AttenuationLevel,
				"SurroundingAttenuationLevel", comp.SurroundingAttenuationLevel,
				"EnableBlinnPhong", comp.EnableBlinnPhong,
				"HasTexture", comp.hasTexture,
				"Visible", comp.visible,
				"AffectsWorld", comp.AffectsWorld
			);

			return isSuccess;
		}

		template<>
		inline bool DeserializeComponentData<PointLightComponent>(const Entity& ent, PointLightComponent& comp)
		{
			bool isSuccess = DeserializeData(
				"Counter", comp.Counter,
				"Ambient", comp.ambient,
				"Diffuse", comp.diffuse,
				"Specular", comp.specular,
				"LightColor", comp.lightColor,
				"Color", comp.Color,
				"IntensityStrength", comp.IntensityStrength,
				"Constant", comp.constant,
				"Linear", comp.linear,
				"Quadratic", comp.quadratic,
				"Radius", comp.radius,
				"AttenuationLevel", comp.AttenuationLevel,
				"EnableBlinnPhong", comp.EnableBlinnPhong,
				"HasTexture", comp.hasTexture,
				"Visible", comp.visible,
				"AffectsWorld", comp.AffectsWorld
			);

			return isSuccess;
		}

		template<>
		inline bool DeserializeComponentData<DirectionalLightComponent>(const Entity& ent, DirectionalLightComponent& comp)
		{
			bool isSuccess = DeserializeData(
				"Counter", comp.Counter,
				"Visible", comp.visible,
				"AffectsWorld", comp.AffectsWorld,
				"EnableBlinnPhong", comp.EnableBlinnPhong,
				"LightColor", comp.lightColor,
				"Direction", comp.Direction,
				"Ambient", comp.ambient,
				"Diffuse", comp.diffuse,
				"Specular", comp.specular,
				"Color", comp.Color
			);

			return isSuccess;
		}
		
		template<>
		inline bool DeserializeComponentData<TextureComponent>(const Entity& ent, TextureComponent& comp)
		{
			bool isSuccess = DeserializeData(
				"TextureType", comp.Type,
				"TextureKey", comp.TextureKey,
				"HasTexture", comp.hasTexture,
				"TextureRef", comp.textureRef
			);

			comp.ID = ent;

			return isSuccess;
		}
	
		template<>
		inline bool DeserializeComponentData<LightComponent>(const Entity& ent, LightComponent& comp)
		{
			return true;
		}
		*/
		
		template <typename CompType>
		inline void SerializeComponent(World& w, const Entity& ent)
		{
			if (w.CheckComponent<CompType>(ent))
			{
				auto index = static_cast<unsigned int>(w.GetComponentType<CompType>());
				auto name = w.GetAllComponentNames()[index];
				auto& comp = w.GetComponent<CompType>(ent);
				sz.StartElement(name);
				SerializeComponentData(RefVariant{ comp });
				sz.CloseElement();
			}
		}

		template <typename CompType>
		inline bool DeserializeComponent(World& w, const Entity& ent)
		{
			auto index = static_cast<unsigned int>(w.GetComponentType<CompType>());
			auto name = w.GetAllComponentNames()[index];
			bool isSuccess = true;

			if (dsz.StartElement(name))
			{
				CompType comp;
				if (DeserializeComponentData(RefVariant{ comp }))
				{
					w.AddComponent(ent, comp);
				}
				else
				{
					isSuccess = false;
				}
				dsz.CloseElement();
			}

			return isSuccess;
		}

		void SerializeEntity(World& w, const Entity& ent, const size_t& counter);

		Entity DeserializeEntity(World& w, const size_t& counter, bool PrefabUse = false);

		void SerializeAllComponents(World& w, const Entity& ent);

		bool DeserializeAllComponents(World& w, const Entity& ent, bool PrefabUse = false);

		void SerializeAllEntity();

		void DeserializeAllEntity();

		void SaveFile(const char* fullpath);

		bool LoadFile(const char* fullpath);

		void SavePrefab(long long unsigned int  prefabID, std::vector<Entity>& prefabContents);

		long long unsigned int LoadPrefab(Entity& dszEnt);

		void SavePrefabWorld(const std::set<Entity>& entities);

	public:
		static Serializer sz;
		static Deserializer dsz;

		SerializationManager();

		~SerializationManager();

		void SaveBackupFile();

		void LoadBackupFile();

		void SavePrefabFile(unsigned long long int prefabID, std::vector<Entity>& prefabContents, const char* path);

		long long unsigned int LoadPrefabFile(Entity& dszEnt, const char* fullpath);

		void SaveSceneFile(const char* fullpath = "Data/Temp/Temp.scn");

		void LoadSceneFile(const char* fullpath = "Data/Temp/Temp.scn");

		void SavePrefabWorldFile(const std::set<Entity>& entities);

		template <typename T>
		inline static void TestSerialize(const char* name, RefVariant refv)
		{
			//if (typeid(T) == typeid(ECVec3))
			//{
			//	std::cout << name << std::endl;
			//	std::cout << refv.ValueRegistry<ECVec3>().getX() << " " << refv.ValueRegistry<ECVec3>().getY() << " " << refv.ValueRegistry<ECVec3>().getZ() << std::endl;
			//}
			//else if constexpr (std::is_fundamental<T>::value /*|| std::is_enum<T>::value*/)
			//{
			//	std::cout << refv.ValueRegistry<RemTypeQual<T>::type>() << std::endl;
			//}
			//else if constexpr (std::is_enum<T>::value)
			//{
			//}
			//std::cout << refv.ValueRegistry<RemTypeQual<T>::type>() << std::endl;

			sz.StartElement(name);
			sz.AddAttributeToElement("value", refv.ValueRegistry<RemTypeQual<T>::type>());
			sz.CloseElement();
		}

		template <typename T>
		inline static bool TestDeserialize(const char* name, RefVariant refv)
		{
			bool isSuccess = false;

			if (dsz.StartElement(name))
			{
				if constexpr (!std::is_pointer<T>::value)
					dsz.ReadAttributeFromElement("value", refv.ValueRegistry<RemTypeQual<T>::type>());
				dsz.CloseElement();
				isSuccess = true;
			}

			return isSuccess;

			//if (typeid(T) == typeid(ECVec3))
			//{
			//	refv.ValueRegistry<RemTypeQual<ECVec3>::type>().setX(1000.0f);
			//	//std::cout << name << std::endl;
			//	//std::cout << refv.ValueRegistry<ECVec3>().getX() << " " << refv.ValueRegistry<ECVec3>().getY() << " " << refv.ValueRegistry<ECVec3>().getZ() << std::endl;
			//}
		}

		/*inline static void TestSerializeCompData(RefVariant refv)
		{
			const MetaData* meta = refv.Meta();
			void* data = refv.Data();

			assert(meta->HasMembers());

			const Member* mem = meta->Members();

			while (mem)
			{
				void* offsetData = PTR_ADD(refv.Data(), mem->Offset());
				mem->Meta()->Serialize(mem->GetName().c_str(), RefVariant(mem->Meta(), offsetData));
				mem = mem->Next();
			}
		}

		inline static void TestDeserializeCompData(RefVariant refv)
		{
			const MetaData* meta = refv.Meta();
			void* data = refv.Data();

			assert(meta->HasMembers());

			const Member* mem = meta->Members();

			while (mem)
			{
				void* offsetData = PTR_ADD(refv.Data(), mem->Offset());
				mem->Meta()->Deserialize(mem->GetName().c_str(), RefVariant(mem->Meta(), offsetData));
				mem = mem->Next();
			}
		}*/

		inline static void SerializeComponentData(RefVariant data)
		{
			const MetaData* meta = data.Meta();

			if (meta->HasMembers())
			{
				const Member* mem = meta->Members();

				while (mem)
				{
					void* offsetData = PTR_ADD(data.Data(), mem->Offset());
					mem->Meta()->Serialize(mem->GetName().c_str(), RefVariant(mem->Meta(), offsetData));
					mem = mem->Next();
				}
			}
		}

		inline static bool DeserializeComponentData(RefVariant data)
		{
			bool IsSuccess = false;
			const MetaData* meta = data.Meta();

			if (meta->HasMembers())
			{
				const Member* mem = meta->Members();

				while (mem)
				{
					void* offsetData = PTR_ADD(data.Data(), mem->Offset());
					IsSuccess = mem->Meta()->Deserialize(mem->GetName().c_str(), RefVariant(mem->Meta(), offsetData));

					if (!IsSuccess)
						return false;

					mem = mem->Next();
				}
			}

			return true;
		}
	};
}