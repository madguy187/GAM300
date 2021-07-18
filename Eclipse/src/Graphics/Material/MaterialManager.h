#ifndef MATERIAL_H
#define MATERIAL_H
#include "ECS/ComponentManager/Components/MaterialComponent.h"

namespace Eclipse
{
    typedef std::map<int, MaterialComponent*> MaterialContainer;
    using MtIT = std::map<int, MaterialComponent*>::iterator;

    struct Material
    {
    public:
        ECVec3 Ambient;
        ECVec3 Diffuse;
        ECVec3 Specular;
        float Shininess;

        static Material Emerald;
        static Material Jade;
        static Material Obsidian;
        static Material Pearl;
        static Material Ruby;
        static Material Turquoise;
        static Material Brass;
        static Material Bronze;
        static Material Chrome;
        static Material Copper;
        static Material Gold;
        static Material Silver;
        static Material Black_plastic;
        static Material Cyan_plastic;
        static Material Green_plastic;
        static Material Red_plastic;
        static Material White_plastic;
        static Material Yellow_plastic;
        static Material Black_rubber;
        static Material Cyan_rubber;
        static Material Green_rubber;
        static Material Red_rubber;
        static Material White_rubber;
        static Material Yellow_rubber;

        static Material mix(Material m1, Material m2, float mix = 0.5f);
    };

    class MaterialManager
    {
    private:
        void CheckUnniformLocation(Shader& in, MaterialComponent& inside);

    public:
        bool EnableHighlight = false;

        float GetCurrentShininess(MaterialComponent& in);
        void UpdateShininess(MaterialComponent& in);
        void Highlight(unsigned int FrameBufferID, GLenum Mode);
    };
}

#endif