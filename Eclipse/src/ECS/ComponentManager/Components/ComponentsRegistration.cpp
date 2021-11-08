#include "pch.h"
#include "AabbComponent.h"
#include "CameraComponent.h"
#include "CollisionComponent.h"
#include "DirectionalLightComponent.h"
#include "EntityComponent.h"
#include "LightComponent.h"
#include "MaterialComponent.h"
#include "MeshComponent.h"
#include "ModelComponent.h"
#include "PointLightComponent.h"
#include "RigidBodyComponent.h"
#include "SpotLightComponent.h"
#include "TextureComponent.h"
#include "TransformComponent.h"
#include "AudioComponent.h"
#include "CollisionComponent.h"
#include "AIComponent.h"
#include "PrefabComponent.h"
#include "ScriptComponent.h"
#include "ChildComponent.h"
#include "ParentComponent.h"

namespace Eclipse
{
    /*************************************************************************/
    /*                      REGISTERING DATA MEMBERS                         */
    /*************************************************************************/
    DEFINE_META(AABBComponent)
    {
        ADD_MEMBER(center);
        ADD_MEMBER(Min);
        ADD_MEMBER(Max);
    }

    DEFINE_META(AudioComponent)
    {
        ADD_MEMBER(AudioPath);
        ADD_MEMBER(ChannelID);
        ADD_MEMBER(Volume);
        ADD_MEMBER(Pitch);
        ADD_MEMBER(Speed);
        ADD_MEMBER(IsLooping);
        ADD_MEMBER(Is3D);
        ADD_MEMBER(IsMuted);
        ADD_MEMBER(InnerConeAngle);
        ADD_MEMBER(OuterConeAngle);
        ADD_MEMBER(OuterVolume);
        ADD_MEMBER(Min);
        ADD_MEMBER(Max);
        ADD_MEMBER(HasRadioEffect);
        ADD_MEMBER(RadioDistortionLevel);
        ADD_MEMBER(HasEchoEffect);
        ADD_MEMBER(EchoDelayTime);
    }

    DEFINE_META(CameraComponent)
    {
        ADD_MEMBER(eyeAlpha);
        ADD_MEMBER(eyeBeta);
        ADD_MEMBER(fov);
        ADD_MEMBER(nearPlane);
        ADD_MEMBER(farPlane);
        ADD_MEMBER(aspect);
        ADD_MEMBER(eyePos);
        ADD_MEMBER(eyeFront);
        ADD_MEMBER(upVec);
        ADD_MEMBER(rightVec);
        ADD_MEMBER(worldUp);
        ADD_MEMBER(viewMtx);
        ADD_MEMBER(projMtx);
        ADD_MEMBER(cameraSpeed);
        ADD_MEMBER(camType);
        ADD_MEMBER(projType);
    }

    DEFINE_META(CollisionComponent)
    {
        ADD_MEMBER(shape);
    }

    DEFINE_META(AIComponent)
    {
        ADD_MEMBER(waypoints);
        ADD_MEMBER(target);
        ADD_MEMBER(MinDisttoChange);
        ADD_MEMBER(PatrolSpeed);
        ADD_MEMBER(patrolling);
    }

    DEFINE_META(DirectionalLightComponent)
    {
        ADD_MEMBER(Counter);
        ADD_MEMBER(visible);
        ADD_MEMBER(AffectsWorld);
        ADD_MEMBER(EnableBlinnPhong);
        ADD_MEMBER(lightColor);
        ADD_MEMBER(Direction);
        ADD_MEMBER(ambient);
        ADD_MEMBER(diffuse);
        ADD_MEMBER(specular);
        ADD_MEMBER(Color);
        ADD_MEMBER(AmbientSettings);
    }

    DEFINE_META(EntityComponent)
    {
        ADD_MEMBER(Tag);
        ADD_MEMBER(Name);
        ADD_MEMBER(LayerIndex);
        ADD_MEMBER(hightLightChild);
        ADD_MEMBER(ImguiIndentValue);
    }

    DEFINE_META(ScriptComponent)
    {
        ADD_MEMBER(scriptList);
        ADD_MEMBER(LayerMask);
    }

    DEFINE_META(LightComponent)
    {
    }

    DEFINE_META(MaterialComponent)
    {
        ADD_MEMBER(Modeltype);
        ADD_MEMBER(ambient);
        ADD_MEMBER(diffuse);
        ADD_MEMBER(specular);
        ADD_MEMBER(NoTextures);
        ADD_MEMBER(shininess);
        ADD_MEMBER(MaximumShininess);
        ADD_MEMBER(IsNormalMap);
        ADD_MEMBER(ComboIndex);
        ADD_MEMBER(TextureComboIndex);
        ADD_MEMBER(Type);
        ADD_MEMBER(hasTexture);
        ADD_MEMBER(TextureRef);
        ADD_MEMBER(TextureKey);
        ADD_MEMBER(HoldingTextures);
        ADD_MEMBER(HasMaterialIstance);
        ADD_MEMBER(MaterialInstanceName);
    }

    DEFINE_META(MeshComponent)
    {
        ADD_MEMBER(MeshName);
        ADD_MEMBER(color);
        ADD_MEMBER(textureIdx);
        ADD_MEMBER(isQuad);
        ADD_MEMBER(modelNDC_xform);
        ADD_MEMBER(modelRef);
        ADD_MEMBER(shaderRef);
        ADD_MEMBER(transparency);
        ADD_MEMBER(ENV_MAP);
        ADD_MEMBER(ENV_TYPE);
    }

    DEFINE_META(ModelComponent)
    {
        ADD_MEMBER(NameOfModel);
        ADD_MEMBER(Directory);
        ADD_MEMBER(type);
        ADD_MEMBER(ComboIndex);
    }

    DEFINE_META(PointLightComponent)
    {
        ADD_MEMBER(Counter);
        ADD_MEMBER(ambient);
        ADD_MEMBER(diffuse);
        ADD_MEMBER(specular);
        ADD_MEMBER(lightColor);
        ADD_MEMBER(Color);
        ADD_MEMBER(IntensityStrength);
        ADD_MEMBER(constant);
        ADD_MEMBER(linear);
        ADD_MEMBER(quadratic);
        ADD_MEMBER(radius);
        ADD_MEMBER(AttenuationLevel);
        ADD_MEMBER(EnableBlinnPhong);
        ADD_MEMBER(hasTexture);
        ADD_MEMBER(visible);
        ADD_MEMBER(AffectsWorld);
        ADD_MEMBER(RGBColor);
    }

    DEFINE_META(RigidBodyComponent)
    {
        ADD_MEMBER(velocity);
        ADD_MEMBER(Angvelocity);
        ADD_MEMBER(forces);
        ADD_MEMBER(MaxVelocity);
        ADD_MEMBER(mass);
        ADD_MEMBER(drag);
        ADD_MEMBER(angdrag);
        ADD_MEMBER(_Kinematic);
        ADD_MEMBER(enableGravity);
        ADD_MEMBER(enableRotation);
    }

    DEFINE_META(SpotLightComponent)
    {
        ADD_MEMBER(Counter);
        ADD_MEMBER(lightColor);
        ADD_MEMBER(direction);
        ADD_MEMBER(ambient);
        ADD_MEMBER(diffuse);
        ADD_MEMBER(specular);
        ADD_MEMBER(Color);
        ADD_MEMBER(IntensityStrength);
        ADD_MEMBER(radius);
        ADD_MEMBER(cutOff);
        ADD_MEMBER(outerCutOff);
        ADD_MEMBER(constant);
        ADD_MEMBER(linear);
        ADD_MEMBER(quadratic);
        ADD_MEMBER(AttenuationLevel);
        ADD_MEMBER(SurroundingAttenuationLevel);
        ADD_MEMBER(EnableBlinnPhong);
        ADD_MEMBER(hasTexture);
        ADD_MEMBER(visible);
        ADD_MEMBER(AffectsWorld);
        ADD_MEMBER(RGBColor);
    }

    DEFINE_META(TextureComponent)
    {
    	  ADD_MEMBER(textureRef);
    }

    DEFINE_META(TransformComponent)
    {
        ADD_MEMBER(position);
        ADD_MEMBER(rotation);
        ADD_MEMBER(scale);
    }

    DEFINE_META(PrefabComponent)
    {
        ADD_MEMBER(IsInstance);
        ADD_MEMBER(IsChild);
        ADD_MEMBER(CompChanges);
        ADD_MEMBER(PrefabID);
    }
    
    DEFINE_META(ChildComponent)
    {
        ADD_MEMBER(parentIndex);
        ADD_MEMBER(PosOffset);
        ADD_MEMBER(RotOffset);
        ADD_MEMBER(ScaleOffset);
        ADD_MEMBER(hasParent);
        ADD_MEMBER(IsAChild);
    }

    DEFINE_META(ParentComponent)
    {
        ADD_MEMBER(child);
        ADD_MEMBER(model);
    }

    /*************************************************************************/
    /*               REGISTERING DATA TYPE - NO NEED TOUCH                   */
    /*************************************************************************/
    DEFINE_META_POD(int);
    DEFINE_META_POD(int*);
    DEFINE_META_POD(unsigned int);
    DEFINE_META_POD(char);
    DEFINE_META_POD(char*);
    DEFINE_META_POD(float);
    DEFINE_META_POD(bool);
    DEFINE_META_POD(double);
    DEFINE_META_POD(size_t);
    DEFINE_META_POD(std::string);
    DEFINE_META_POD(std::vector<Texture>);
    DEFINE_META_POD(std::vector<Entity>);
    DEFINE_META_POD(std::vector<MonoScript>);
    DEFINE_META_POD(MeshComponent::MeshNameType);
    DEFINE_META_POD(ECVec2);
    DEFINE_META_POD(ECVec3);
    DEFINE_META_POD(ECVec4);
    DEFINE_META_POD(Texture);
    DEFINE_META_POD(glm::vec3);
    DEFINE_META_POD(glm::vec4);
    DEFINE_META_POD(glm::mat4);
    DEFINE_META_POD(CameraComponent::CameraType);
    DEFINE_META_POD(CameraComponent::ProjectionType);
    DEFINE_META_POD(EntityType);
    DEFINE_META_POD(TextureType);
    DEFINE_META_POD(MaterialModelType);
    DEFINE_META_POD(MeshComponent::MapType);
    DEFINE_META_POD(ModelType);
    DEFINE_META_POD(PxShapeType);
    DEFINE_META_POD(EC_Shape);
    DEFINE_META_POD(Signature);
    DEFINE_META_POD(std::bitset<20>);
    //DEFINE_META_POD(MonoScript);
}
