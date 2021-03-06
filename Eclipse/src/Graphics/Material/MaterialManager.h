#ifndef MATERIAL_H
#define MATERIAL_H

#include "ECS/ComponentManager/Components/MaterialComponent.h"
#include "Graphics/ModelFactory/AssimpModel/AssimpModel.h"
#include "ECS/ComponentManager/Components/MeshComponent.h"

namespace Eclipse
{
    typedef std::unordered_map<int, MaterialComponent*> MaterialContainer;
    using MtIT = std::unordered_map<int, MaterialComponent*>::iterator;

    typedef std::unordered_map<int, MaterialComponent*> HighlightingContainer;
    using HighlightIT = std::unordered_map<int, MaterialComponent*>::iterator;

    typedef std::unordered_map<int, AssimpModel*> ModelHighLight;
    using HighlightModelIT = std::unordered_map<int, AssimpModel*>::iterator;

    typedef std::unordered_map<unsigned int, MeshComponent*> MeshHighLight;
    using MeshHighlightModelIT = std::unordered_map<int, MeshComponent*>::iterator;

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
    public:
        // First parameter is for init , should we add highlight Component into everyone of them?
        // Second parameter is enabling highlight
        MaterialManager(bool InitRegisterHighlight_, bool EnableHighlight_);
        bool InitRegisterHighlight = false;
        bool EnableHighlight = false;
        ModelHighLight ModelHighlightContainer;
        MeshHighLight MeshHighLightContainer;

        ///////////////////////////////////////////////////////////////////////////////////////////
        // Highlighting
        ///////////////////////////////////////////////////////////////////////////////////////////
        float Thickness = 0.02f; // Check Highlighting
        float ScaleUp = 1.02f;   // Only for 3D Primitives
        ECVec3 HighlightColour{ 1.0f,1.0f,0.0f };

    public:
        float GetCurrentShininess(MaterialComponent& in);
        void UpdateShininess(unsigned int EntityID);

        void DoNotUpdateStencil();
        void StencilBufferClear();
        void CheckUniformLoc(Shader& _shdrpgm, CameraComponent& _camera, unsigned int ModelID);
        void MeshHighlight(unsigned int FrameBufferID, GLenum Mode);
        void UpdateMaterial(MaterialComponent& in);
        void Highlight3DModels(FrameBufferMode, unsigned int ModelID, GLenum mode);
        void UpdateStencilWith_Outline(unsigned int ID);
        void UpdateStencilWithActualObject(unsigned int ID);
        void HighlightBasicPrimitives(unsigned int EntityId, FrameBufferMode Mode);
        void Highlight3DModels(unsigned int EntityId, FrameBufferMode);
        bool HighlightClick(unsigned int ModelID);
        bool UnHighlight(unsigned int ModelID);

        ///////////////////////////////////////////////////////////////////////////////////////////
        // For Jian Herng
        ///////////////////////////////////////////////////////////////////////////////////////////
        void ClearContainer();

    private:
        void CheckUnniformLocation(Shader& in, MaterialComponent& inside);
        void CheckUniformLoc(MeshComponent& sprite, Shader& in, unsigned int id, FrameBufferMode);
        void Highlight(FrameBufferMode FrameBufferID, unsigned int ModelID, GLenum mode);
    };
}

#endif