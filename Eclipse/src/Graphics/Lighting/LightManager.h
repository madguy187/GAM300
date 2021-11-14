#ifndef LIGHT_MANAGER_H
#define LIGHT_MANAGER_H
#include "Graphics/Lighting/PointLight.h"
#include "Graphics/Lighting/DirectionalLight.h"
#include "Graphics/Lighting/SpotLight.h"

namespace Eclipse
{
    enum class TypesOfLights
    {
        NONE,
        POINTLIGHT,
        DIRECTIONAL,
        SPOTLIGHT,
        MAXCOUNT
    };

    class AttenuationValues
    {
    public:
        float Constant_ = 0.0f;
        float Linear_ = 0.0f;
        float Quadratic_ = 0.0f;

    public:
        AttenuationValues(float constant, float linear, float quadratic) :
            Constant_(constant),
            Linear_(linear),
            Quadratic_(quadratic)
        {

        }
    };

    class Angles
    {
    public:
        float InnerAngle = 0.0f;
        float OuterAngle = 0.0f;

    public:
        Angles(float inn, float out) :
            InnerAngle(inn),
            OuterAngle(out)
        {

        }
    };

    class LightManager
    {
    public:
        Entity Hand = MAX_ENTITY;

        // Base Classes =============================
        template<typename T, typename = void>
        struct has_Color : std::false_type {};

        template<typename T, typename = void>
        struct has_LightColor : std::false_type {};

        template<typename T, typename = void>
        struct has_Ambient : std::false_type {};

        template<typename T, typename = void>
        struct has_Diffuse : std::false_type {};

        template<typename T, typename = void>
        struct has_attenuation : std::false_type {};

        template<typename T, typename = void>
        struct has_Specular : std::false_type {};

        template<typename T, typename = void>
        struct has_EnableBlinnPhong : std::false_type {};

        template<typename T, typename = void>
        struct has_RGBLightColor : std::false_type {};

        // Check if T has the variable , if have , will enter template =============================
        template<typename TypeOfLight>
        struct has_Color< TypeOfLight, decltype(std::declval<TypeOfLight>().Color, void())> : std::true_type {};

        template<typename TypeOfLight>
        struct has_LightColor< TypeOfLight, decltype(std::declval<TypeOfLight>().lightColor, void())> : std::true_type {};

        template<typename TypeOfLight>
        struct has_Ambient< TypeOfLight, decltype(std::declval<TypeOfLight>().ambient, void())> : std::true_type {};

        template<typename TypeOfLight>
        struct has_Diffuse< TypeOfLight, decltype(std::declval<TypeOfLight>().diffuse, void())> : std::true_type {};

        template<typename TypeOfLight>
        struct has_attenuation< TypeOfLight, decltype(std::declval<TypeOfLight>().constant, std::declval<TypeOfLight>().linear, std::declval<TypeOfLight>().quadratic, void())> : std::true_type {};

        template<typename TypeOfLight>
        struct has_Specular< TypeOfLight, decltype(std::declval<TypeOfLight>().specular, void())> : std::true_type {};

        template<typename TypeOfLight>
        struct has_EnableBlinnPhong< TypeOfLight, decltype(std::declval<TypeOfLight>().EnableBlinnPhong, void())> : std::true_type {};

        template<typename TypeOfLight>
        struct has_RGBLightColor< TypeOfLight, decltype(std::declval<TypeOfLight>().RGBColor, void())> : std::true_type {};

    private:
        PointLight _allpointlights;
        DirectionalLight _DirectionalLights;
        SpotLight _allspotlights;
        std::vector <std::pair<unsigned int, AttenuationValues>> AttenuationLevels;
        std::vector <std::pair<unsigned int, Angles>> SpotLightAngles;
        bool EnableBlinnPhong = true;

    public:
        bool ApplyLighting = true;
        Entity DirectionLightID = 0;
        bool EnableShadows = false;

        void Init();
        void CreateLights(TypesOfLights in, unsigned int CreatedID);
        void SetApplyLightingFlag(bool in);
        bool CheckApplyLighting();

        // Draws =============================
        void DrawPointLights(unsigned int EntityId, PointLightComponent* in, FrameBufferMode, unsigned int indexID, GLenum mode);
        void DrawDirectionalLight(unsigned int EntityId, DirectionalLightComponent* in, FrameBufferMode, unsigned int indexID, GLenum mode);
        void DrawSpotLight(unsigned int EntityId, SpotLightComponent* in, FrameBufferMode, unsigned int indexID, GLenum mode);

        // LightProperties =============================
        void CreateAttenuationLevels();

        void GlobalUniformsUpdate();

        void DestroyLight(Entity ID);

        template <typename TypeOfLight, typename TYPE>
        void SetColor(TypeOfLight& OBJ, TYPE val);

        template <typename TypeOfLight>
        void SetColor(TypeOfLight& OBJ, ECVec4 in);

        template <typename TypeOfLight>
        void SetLightColor(TypeOfLight& OBJ, ECVec4 in);

        template <typename TypeOfLight>
        void SetAmbient(TypeOfLight& OBJ, ECVec3 in);

        template <typename TypeOfLight>
        void SetDiffuse(TypeOfLight& OBJ, ECVec3 in);

        template <typename TypeOfLight>
        void SetAttenuation(TypeOfLight& OBJ, unsigned int in);

        template <typename TypeOfLight>
        void SetSpecular(TypeOfLight& OBJ, ECVec3 in);

        template <typename TypeOfLight>
        void SetBlinnPhong(TypeOfLight& OBJ, bool& in);

        template <typename TypeOfLight>
        void SetRGBLightColor(TypeOfLight& OBJ, ECVec4 in);
    };

#include "Graphics/Lighting/LightProperties.hpp"
}

#endif /* LIGHT_MANAGER_H */