#pragma once
#include "ECS/ComponentManager/Components/RenderComponent.h"

namespace Eclipse
{
  class GraphicsManager
  {
  public:
      OpenGL_Context mRenderContext;
      unsigned int createdID;

  public:
    void pre_render();
    void post_render();
    void end();
    void unload();
    void GlobalFrmeBufferDraw();
    void GlobalFrameBufferBind();
    void DrawBuffers(unsigned int FrameBufferID, RenderComponent* _spritecomponent, GLenum mode);
    void DrawIndexed(RenderComponent* in, GLenum mode);
    void CheckTexture(RenderComponent* in);
    void CreatePrimitives(Entity ID, int ModelType);

  private:
    void CheckUniformLoc(RenderComponent& sprite, unsigned int id, unsigned int framebufferID);
    void UpdateFrameBuffer();
    void FrameBufferDraw();
  };
}