#ifndef GRAPHICS_MANAGER_H
#define GRAPHICS_MANAGER_H

#include "ECS/ComponentManager/Components/RenderComponent.h"
#include "AssimpModel/AssimpModel.h"
#include "Graphics/InputHandler/InputWrapper.h"
#include "Graphics/ModelFactory/Sky/CubeMap.h"

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

    public:
        void pre_render();
        void post_render();
        void end();
        void unload();
        void GlobalFrmeBufferDraw();
        void GlobalFrameBufferBind();
        void Draw(unsigned int FrameBufferID, RenderComponent* _spritecomponent, GLenum mode);
        void DrawIndexed(RenderComponent* in, GLenum mode);
        void CheckTexture(RenderComponent* in);
        void CreatePrimitives(Entity ID, int ModelType);
        void CreateSky(std::string _Dir);
        void RenderSky(unsigned int FrameBufferID);

    private:
        void CheckUniformLoc(RenderComponent& sprite, unsigned int id, unsigned int framebufferID);
        void UpdateFrameBuffer();
        void FrameBufferDraw();
    };
}

#endif