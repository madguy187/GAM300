#pragma once
#include "Graphics/ModelFactory/BasicPrimitives/Quad.h"

namespace Eclipse
{
    // GridScale % InnerRatio Should be 0 as it gives u the number of boxes inside a grid

    class Grid
    {
    private:
        unsigned int GridID;
        ECVec3 GridColour{ 0.3f,0.3f,0.3f };
        Quad* WholeGrid = nullptr;
        Shader* ShaderRef = nullptr;
        std::string ShaderName;
        int GridScale = 10;
        int InnerRatio = 2;
        int SingleXAxisLineThickness = 5;
        int SingleZAxisLineThickness = 5;
        float XAxisColour = 1.0f;
        float ZAxisColour = 1.0f;

        // Not used yet
        float Fading = 0.0f;
        float Transparency = 1.0f;

    public:
        bool Visible = true;

        Quad* GetModelReference();
        Shader* GetShaderReference();
        ECVec3 GetGridColour();
        float GetTransparency();
        float GetGridScale();
        unsigned int GetGridID();
        int GetInnerRatio();
        int GetSingleZ_Thickness();
        int GetSingleX_Thickness();
        float GetXAxisColour();
        float GetZAxisColour();

        void SetTransparency(float in);
        void SetGridToShow(bool in);
        void SetGridScale(float in);
        void SetInnerRatio(int& in);
        void SetGridColour(ECVec3& in);
        void SetSingleZ_Thickness(int& in);
        void SetSingleX_Thickness(int& in);
        void SetXAxisColour(float& in);
        void SetZAxisColour(float& in);

        void DebugPrint();
        bool CheckShowGrid();
        void UseFrameBuffer(unsigned int FramebufferID);
        void CheckUniformLocation(unsigned int GridID);
        void Init();
        void DrawGrid(unsigned int FrameBufferID);

        ~Grid();

        // Useless Stuffs for Now
        //IModel* modelRef;
    };
}