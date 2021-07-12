#pragma once
#include <map>
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

    class Lights
    {
    public:
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
        struct has_attenuation< TypeOfLight, decltype(std::declval<TypeOfLight>().constant, std::declval<TypeOfLight>().linear , std::declval<TypeOfLight>().quadratic, void())> : std::true_type {};

        template<typename TypeOfLight>
        struct has_Specular< TypeOfLight, decltype(std::declval<TypeOfLight>().specular, void())> : std::true_type {};

    private:
        PointLight _allpointlights;
        DirectionalLight _DirectionalLights;
        SpotLight _allspotlights;
        std::vector <std::pair<unsigned int, AttenuationValues>> AttenuationLevels;
        std::vector <std::pair<unsigned int, Angles>> SpotLightAngles;

    public:
        void init();
        void Update();
        void CreateLights(TypesOfLights in, unsigned int CreatedID);

        // Draws =============================
        void DrawPointLights(PointLightComponent* in, unsigned int framebufferID, unsigned int indexID, GLenum mode);
        void DrawDirectionalLight(DirectionalLightComponent* in, unsigned int framebufferID, unsigned int indexID, GLenum mode);
        void DrawSpotLight(SpotLightComponent* in, unsigned int framebufferID, unsigned int indexID, GLenum mode);

        // Get Containers  =============================
        PointLightContainer GetPointLightsContainer();
        DirectionalLightContainer GetDirectionalLightContainer();
        SpotLightContainer GetSpotLightsContainer();

        // LightProperties =============================
        void CreateAttenuationLevels();

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
    };

#include "Graphics/Lighting/LightProperties.hpp"
}