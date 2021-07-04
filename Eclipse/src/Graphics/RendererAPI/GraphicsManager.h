#pragma once
#include "ECS/ComponentManager/Components/RenderComponent.h"
#include "AssimpModel/AssimpModel.h"
#include "Graphics/InputHandler/InputWrapper.h"

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

  private:
    void CheckUniformLoc(RenderComponent& sprite, unsigned int id, unsigned int framebufferID);
    void UpdateFrameBuffer();
    void FrameBufferDraw();
  };
}