#include "pch.h"
#include "AabbComponent.h"
#include "CameraComponent.h"
#include "CollisionComponent.h"
#include "DirectionalLightComponent.h"
#include "EntityComponent.h"
#include "LightComponent.h"
#include "MaterialComponent.h"
#include "MeshComponent.h"
#include "ModeLInfoComponent.h"
#include "ParentChildComponent.h"
#include "PointLightComponent.h"
#include "RigidBodyComponent.h"
#include "SpotLightComponent.h"
#include "TextureComponent.h"
#include "TransformComponent.h"

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
        ADD_MEMBER(modelNDC_xform);
    }

    DEFINE_META(EntityComponent)
    {
        ADD_MEMBER(Tag);
        ADD_MEMBER(Name);
        ADD_MEMBER(IsActive);
        ADD_MEMBER(Child);
        ADD_MEMBER(Parent);
        ADD_MEMBER(IsAChild);
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
        ADD_MEMBER(HighlightColour);
        ADD_MEMBER(Highlight);
        ADD_MEMBER(NoTextures);
        ADD_MEMBER(shininess);
        ADD_MEMBER(MaximumShininess);
        ADD_MEMBER(Thickness);
        ADD_MEMBER(ScaleUp);
        ADD_MEMBER(ComboIndex);
        ADD_MEMBER(hasTexture);
        ADD_MEMBER(TextureRef);
    }

    DEFINE_META(MeshComponent)
    {
        ADD_MEMBER(color);
        ADD_MEMBER(textureIdx);
        ADD_MEMBER(isQuad);
        ADD_MEMBER(modelNDC_xform);
        ADD_MEMBER(modelRef);
        ADD_MEMBER(shaderRef);
        ADD_MEMBER(transparency);
    }

    DEFINE_META(ModeLInforComponent)
    {
        ADD_MEMBER(NameOfModel);
        ADD_MEMBER(Directory);
        ADD_MEMBER(type);
        ADD_MEMBER(ComboIndex);
    }

    DEFINE_META(ParentChildComponent)
    {
        ADD_MEMBER(parentIndex);
        ADD_MEMBER(offset);
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
    }

    DEFINE_META(RigidBodyComponent)
    {
        ADD_MEMBER(velocity);
        ADD_MEMBER(Angvelocity);
        ADD_MEMBER(forces);
        ADD_MEMBER(mass);
        ADD_MEMBER(drag);
        ADD_MEMBER(angdrag);
        ADD_MEMBER(_Static);
        ADD_MEMBER(_Kinematic);
        ADD_MEMBER(enableGravity);
        ADD_MEMBER(enableRotation);
        ADD_MEMBER(inScene);
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
    //DEFINE_META_POD(std::vector<std::string>);
    //DEFINE_META_POD(std::vector<Mesh>);
    //DEFINE_META_POD(std::vector<Texture>);
    //DEFINE_META_POD(std::map<unsigned int, std::vector<Texture>>);
    DEFINE_META_POD(std::vector<Entity>);
    DEFINE_META_POD(ECVec2);
    DEFINE_META_POD(ECVec3);
    DEFINE_META_POD(ECVec4);
    //DEFINE_META_POD(Mesh);
    //DEFINE_META_POD(Texture);
    DEFINE_META_POD(glm::vec3);
    DEFINE_META_POD(glm::mat4);
    DEFINE_META_POD(CameraComponent::CameraType);
    DEFINE_META_POD(CameraComponent::ProjectionType);
    DEFINE_META_POD(EntityType);
    DEFINE_META_POD(TextureType);
    DEFINE_META_POD(MaterialModelType);
}