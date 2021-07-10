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