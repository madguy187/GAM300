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
#include "../ECS/ComponentManager/Components/MeshComponent.h"
#include "../ECS/ComponentManager/Components/RigidBodyComponent.h"
#include "../ECS/ComponentManager/Components/SpotLightComponent.h"
#include "../ECS/ComponentManager/Components/TransformComponent.h"
//#include "../ECS/ComponentManager/Components/TextureComponent.h"
#include "../ECS/ComponentManager/Components/LightComponent.h"

namespace Eclipse
{

    class SerializationManager
    {
        Serializer sz;
        Deserializer dsz;

        inline void SerializeData() {}

        template <typename T, typename... Args>
        inline void SerializeData(const char* name, T element, Args... elements)
        {
            sz.StartElement(name);
            sz.AddAttributeToElement("value", element);
            sz.CloseElement();
            SerializeData(elements...);
        }

        template <typename CompType>
        inline void SerializeComponentData(const CompType& data)
        {
            sz.AddAttributeToElement("Unavailable", true);
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
                "IsStatic", data._Static,
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
                "HighlightColor", data.HighlightColour,
                "Shininess", data.shininess,
                "MaximumShininess", data.MaximumShininess,
                //"RegisterForHighlight", data.RegisterForHighlight,
                "Highlight", data.Highlight,
                "NoTextures", data.NoTextures,
                "Thickness", data.Thickness,
                "ScaleUp", data.ScaleUp
            );
        }

        template <>
        inline void SerializeComponentData<AABBComponent>(const AABBComponent& data)
        {
            SerializeData(
                "Center", data.center,
                "Min", data.min,
                "Max", data.max
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
        }

        template <>
        inline void SerializeComponentData<PointLightComponent>(const PointLightComponent& data)
        {
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
        }

        template <>
        inline void SerializeComponentData<DirectionalLightComponent>(const DirectionalLightComponent& data)
        {
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
        }

        //template <>
        //inline void SerializeComponentData<TextureComponent>(const TextureComponent& data)
        //{
        //	SerializeData(
        //		//"ID", data.ID,
        //		"TextureType", data.Type,
        //		//"TextureKey", data.TextureKey,
        //		"HasTexture", data.hasTexture,
        //		"TextureRef", data.textureRef
        //	);
        //}

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

        template <typename T>
        inline bool DeserializeComponentData(const Entity& ent, T& comp)
        {
            std::string msg = typeid(T).name();
            msg += " is an invalid component.";
            //EDITOR_LOG_WARN(false, msg.c_str());

            return false;
        }

        template<>
        inline bool DeserializeComponentData<EntityComponent>(const Entity& ent, EntityComponent& comp)
        {
            bool isSuccess = DeserializeData(
                "Name", comp.Name,
                "Tag", comp.Tag,
                "IsActive", comp.IsActive
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
                "ShaderRef", comp.shaderRef
                //"Name", comp.name,
                //"NewLayer", comp.newLayer,
                //"LayerNum", comp.layerNum,
                //"Transparency", comp.transparency,
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
                "HighlightColor", comp.HighlightColour,
                "Shininess", comp.shininess,
                "MaximumShininess", comp.MaximumShininess,
                //"RegisterForHighlight", comp.RegisterForHighlight,
                "Highlight", comp.Highlight,
                "Thickness", comp.Thickness,
                "NoTextures", comp.NoTextures,
                "ScaleUp", comp.ScaleUp
            );

            return isSuccess;
            comp.ComboIndex = static_cast<size_t>(comp.Modeltype);
        }

        template<>
        inline bool DeserializeComponentData<AABBComponent>(const Entity& ent, AABBComponent& comp)
        {
            bool isSuccess = DeserializeData(
                "Center", comp.center,
                "Min", comp.min,
                "Max", comp.max
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
                "IsStatic", comp._Static,
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

            comp.ID = ent;

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

            comp.ID = ent;

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
                "Color", comp.Color,
                "ModelNDC_xform", comp.modelNDC_xform
            );

            comp.ID = ent;

            return isSuccess;
        }

        //template<>
        //inline bool DeserializeComponentData<TextureComponent>(const Entity& ent, TextureComponent& comp)
        //{
        //	bool isSuccess = DeserializeData(
        //		"TextureType", comp.Type,
        //		//"TextureKey", comp.TextureKey,
        //		"HasTexture", comp.hasTexture,
        //		"TextureRef", comp.textureRef
        //	);

        //	//comp.ID = ent;

        //	return isSuccess;
        //}

        template<>
        inline bool DeserializeComponentData<LightComponent>(const Entity& ent, LightComponent& comp)
        {
            return true;
        }

        template <typename CompType>
        inline void SerializeComponent(World& w, const Entity& ent)
        {
            if (w.CheckComponent<CompType>(ent))
            {
                auto index = static_cast<unsigned int>(w.GetComponentType<CompType>());
                auto name = w.GetAllComponentNames()[index];
                auto& comp = w.GetComponent<CompType>(ent);
                sz.StartElement(name);
                SerializeComponentData<CompType>(comp);
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
                if (DeserializeComponentData<CompType>(ent, comp))
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

        void SerializeEntity(const Entity& ent, const size_t& counter);

        void DeserializeEntity(const size_t& counter);

        void SerializeAllComponents(const Entity& ent);

        bool DeserializeAllComponents(const Entity& ent);

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