#pragma once
#include "ECS/ComponentManager/Components/RenderComponent.h"

namespace Eclipse
{
  class GraphicsManager
  {
  public:
    void pre_render();
    void post_render();
    void end();
    void unload();
    void FrameBufferDraw();

    // to be removed
    void ImguiRender();
    void UpdateFrameBuffer();
    OpenGL_Context mRenderContext;
    unsigned int createdID;
    void DrawBuffers(unsigned int FrameBufferID, RenderComponent* _spritecomponent, GLenum mode);

    void L_CheckUniformLoc(RenderComponent& sprite, unsigned int id, unsigned int framebufferID);

  private:
    void ShowTestWidgets();
    void CreatePrimitives(GLint model);
    void CheckUniformLoc(RenderComponent& sprite, unsigned int id, unsigned int framebufferID);

  public:
    void ShowTestWidgets(unsigned int id, unsigned int createdId);
  };
}