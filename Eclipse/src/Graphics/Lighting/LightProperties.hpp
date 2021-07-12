#include "Graphics/Lighting/LightManager.h"

template <typename TypeOfLight, typename TYPE>
void Lights::SetColor(TypeOfLight& OBJ, TYPE val)
{
    if (constexpr(has_Color<TypeOfLight>::value == true))
    {
        OBJ.Color.setX(val);
        OBJ.Color.setY(val);
        OBJ.Color.setZ(val);
    }
}

template <typename TypeOfLight>
void Lights::SetColor(TypeOfLight& OBJ, ECVec4 in)
{
    if (constexpr(has_Color<TypeOfLight>::value == true))
    {
        OBJ.Color.setX(in.x);
        OBJ.Color.setY(in.y);
        OBJ.Color.setZ(in.z);
    }
}

template <typename TypeOfLight>
void Lights::SetLightColor(TypeOfLight& OBJ, ECVec4 in)
{
    if (constexpr(has_LightColor<TypeOfLight>::value == true))
    {
        OBJ.lightColor.setX(in.x);
        OBJ.lightColor.setY(in.y);
        OBJ.lightColor.setZ(in.z);
    }
}

template<typename TypeOfLight>
inline void Eclipse::Lights::SetAmbient(TypeOfLight& OBJ, ECVec3 in)
{
    if (constexpr(has_LightColor<TypeOfLight>::value == true))
    {
        OBJ.ambient.setX(in.x);
        OBJ.ambient.setY(in.y);
        OBJ.ambient.setZ(in.z);
    }
}

template<typename TypeOfLight>
inline void Eclipse::Lights::SetDiffuse(TypeOfLight& OBJ, ECVec3 in)
{
    if (constexpr(has_LightColor<TypeOfLight>::value == true))
    {
        OBJ.diffuse.setX(in.x);
        OBJ.diffuse.setY(in.y);
        OBJ.diffuse.setZ(in.z);
    }
}

template<typename TypeOfLight>
inline void Eclipse::Lights::SetAttenuation(TypeOfLight& OBJ, unsigned int in)
{
    if (constexpr(has_attenuation<TypeOfLight>::value == true))
    {
        OBJ.constant = AttenuationLevels[in].second.Constant_;
        OBJ.linear = AttenuationLevels[in].second.Linear_;
        OBJ.quadratic = AttenuationLevels[in].second.Quadratic_;
    }
}

template<typename TypeOfLight>
inline void Eclipse::Lights::SetSpecular(TypeOfLight& OBJ, ECVec3 in)
{
    if (constexpr(has_Specular<TypeOfLight>::value == true))
    {
        OBJ.specular.setX(in.x);
        OBJ.specular.setY(in.y);
        OBJ.specular.setZ(in.z);
    }
}
