#ifndef GRAPHICS_MANAGER_H
#define GRAPHICS_MANAGER_H

#include "ECS/ComponentManager/Components/RenderComponent.h"
#include "AssimpModel/AssimpModel.h"
#include "Graphics/InputHandler/InputWrapper.h"
#include "Graphics/ModelFactory/Sky/CubeMap.h"
#include "Graphics/Grid/Grid.h"
#include "Graphics/Grid/Box.h"

namespace Eclipse
{
    class GraphicsManager
    {
    public:
        OpenGL_Context mRenderContext;
        std::vector<AssimpModel*> ModelContainer;
        InputWrapper InputHandler;
        GLenum GlobalMode = GL_FILL;
        std::unique_ptr<CubeMap> Sky;
        std::unique_ptr<Grid> GridManager;
        AABB_ AllAABBs;

        unsigned int createdID;
        unsigned int SkyCount = 0;
        bool CheckRender = true;
        bool EnableGammaCorrection = true;
        bool DrawSky = false;

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
        void CheckUniformLoc(RenderComponent& sprite, unsigned int id, unsigned int framebufferID);

    private:
        float GammaCorrection = 1.0f;
        void UpdateFrameBuffer();
        void FrameBufferDraw();
    };
}

#endif