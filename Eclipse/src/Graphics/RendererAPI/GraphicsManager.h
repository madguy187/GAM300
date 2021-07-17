#ifndef GRAPHICS_MANAGER_H
#define GRAPHICS_MANAGER_H

#include "ECS/ComponentManager/Components/RenderComponent.h"
#include "AssimpModel/AssimpModel.h"
#include "Graphics/InputHandler/InputWrapper.h"
#include "Graphics/ModelFactory/Sky/CubeMap.h"
#include "Graphics/Grid/Grid.h"

namespace Eclipse
{
    class GraphicsManager
    {
    public:
        OpenGL_Context mRenderContext;
        std::vector<AssimpModel*> ModelContainer;
        unsigned int createdID;
        InputWrapper InputHandler;
        GLenum GlobalMode = GL_FILL;
        std::unique_ptr<CubeMap> Sky;
        unsigned int SkyCount = 0;
        std::unique_ptr<Grid> GridManager;
        bool CheckRender = true;
        bool EnableHighlight = true;
        float OutlineThickness = 0.05f;
        bool EnableGammaCorrection = true;
        bool DrawSky = true;

    public:
        void Pre_Render();
        void Post_Render();
        void End();
        void Unload();
        void GlobalFrmeBufferDraw();
        void GlobalFrameBufferBind();
        void Draw(unsigned int FrameBufferID, RenderComponent* _spritecomponent, GLenum mode);
        void DrawIndexed(RenderComponent* in, GLenum mode);
        void CheckTexture(RenderComponent* in);
        void CreatePrimitives(Entity ID, int ModelType);
        void CreateSky(std::string _Dir);
        void RenderSky(unsigned int FrameBufferID);
        void DebugPrintFrameBuffers();
        float GetGammaCorrection();
        void SetGammaCorrection(float in);
        void UploadGlobalUniforms();
        void StencilBufferClear();
        void OutlinePreparation1();
        void OutlinePreparation2();
        bool CheckIfHighlight();
        void SetIfHighlight(bool in);

    private:
        float GammaCorrection = 2.2f;

        void CheckUniformLoc(RenderComponent& sprite, unsigned int id, unsigned int framebufferID);
        void UpdateFrameBuffer();
        void FrameBufferDraw();
    };
}

#endif